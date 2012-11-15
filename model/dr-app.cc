/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Uniandes (unregistered)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Juanmalk <jm.aranda121@uniandes.edu.co> 
 */

#include "ns3/log.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "dr-header.h"
#include "dc-app.h"
#include "dr-app.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DemandResponseApplication");
NS_OBJECT_ENSURE_REGISTERED (DemandResponseApplication);

TypeId 
DemandResponseApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DemandResponseApplication")
    .SetParent<Application> ()
    .AddConstructor<DemandResponseApplication> ()
    ;
  return tid;
}

DemandResponseApplication::DemandResponseApplication ()
{
  // For debugging purposes
  //NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s DrApp created!");
  m_nextTimeRequest = Seconds (0.0);
  m_meterData = 0;
  m_partialMeterData = 0;
  m_length = 0;
  m_sendEvent = EventId ();
  m_requestEvent = EventId ();
  m_commandEvent = EventId ();
  m_demandResponseMechanismEvent = EventId ();
}

DemandResponseApplication::~DemandResponseApplication ()
{
  
}

void 
DemandResponseApplication::Send (Ptr<Packet> packet, Address currentDcAddress)
{
  NS_ASSERT (m_sendEvent.IsExpired ());
  Simulator::Cancel (m_sendEvent);

  if (Ipv4Address::IsMatchingType(currentDcAddress) == true)
    {
      // Connect Control Center to the current remote Data Concentrator
      m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom (currentDcAddress), 5050));

      // Call SEND Udp function (through UdpSocket)
      m_socket->Send (packet); 
      NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s CC (" 
                                               << Ipv4Address::ConvertFrom (m_localAddress) 
                                               << ") sent a request (" << packet->GetSize () << "B) to DC ("
                                               << Ipv4Address::ConvertFrom (currentDcAddress) << ")");
    } 
}	

void 
DemandResponseApplication::Recv (Ptr<Socket> socket)
{
  Ptr<Packet> pkt;
  Address from;
  TypeMessage typeHdr;
  // Retreive the packet sent by the Smart Grid Center
  pkt = socket->RecvFrom (from);  
 
  if (InetSocketAddress::IsMatchingType (from))
    {
     // Remove and copy the Demand Reponse Message Header from the packet
     pkt->RemoveHeader (typeHdr);
     MessageType typeMessage = typeHdr.GetMessageType (); 
   
     if (typeMessage == METER_POLL_RES_NORMAL)
       { 
         // Record the received information
         MeterPollResponseNormalMessageHeader hdr;
         pkt->RemoveHeader (hdr);
         m_meterData = hdr.GetMeterData (); //---> check it when the CC is attending more than one DC: can m_meterData being overwritten?
         NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s CC (" 
                                                   << Ipv4Address::ConvertFrom (m_localAddress) 
                                                   << ") received " << hdr.GetLength () << "B of data from DC ("
                                                   << InetSocketAddress::ConvertFrom (from).GetIpv4 () << ")");

         // Event: Call Demand Response Mechanism to analyse the received data
         Address currentDcAddress = Address (InetSocketAddress::ConvertFrom (from).GetIpv4 ());
         m_demandResponseMechanismEvent = Simulator::Schedule (Seconds (0.0), &DemandResponseApplication::DemandResponseMechanism, this, m_meterData, currentDcAddress);
       }

     if (typeMessage == METER_POLL_RES_BLOCK)
       { 
         // Record the received information
         MeterPollResponseBlockMessageHeader hdr;
         pkt->RemoveHeader (hdr);

         if (hdr.GetLastBlock () == 0)
           {
             m_partialMeterData += hdr.GetMeterData (); //---> check it when the CC is attending more than one DC
              
              if (hdr.GetBlockNumber () == 1)
                m_length = hdr.GetLength ();

             NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s CC (" << Ipv4Address::ConvertFrom (m_localAddress) 
                                                          << ") received block (n = " << hdr.GetBlockNumber ()
                                                          << ") " << m_partialMeterData << "B of " <<  m_length << "B from DC ("
                                                          << InetSocketAddress::ConvertFrom (from).GetIpv4 () << ")");
             
             // Event: Request the next block of data
             m_requestNextBlockEvent = Simulator::Schedule (Seconds (0.0), &DemandResponseApplication::RequestNextBlock, this, hdr.GetBlockNumber ());

           }
         else
           {
             m_partialMeterData += hdr.GetMeterData ();  //---> check it when the CC is attending more than one DC
             m_meterData = m_partialMeterData;
             m_partialMeterData = 0;

             NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s CC (" 
                                                   << Ipv4Address::ConvertFrom (m_localAddress) 
                                                   << ") received block (LAST) " <<  m_meterData << "B of " 
                                                   <<  m_length << "B from DC ("
                                                   << InetSocketAddress::ConvertFrom (from).GetIpv4 () << ")");
              
             

             // Event: Call Demand Response Mechanism to analyse the received data
             Address currentDcAddress = Address (InetSocketAddress::ConvertFrom (from).GetIpv4 ());
             m_demandResponseMechanismEvent = Simulator::Schedule (Seconds (0.0), &DemandResponseApplication::DemandResponseMechanism, this, m_meterData, currentDcAddress);
            } 
         }
      }
}

void 
DemandResponseApplication::Request ()
{
  NS_ASSERT (m_requestEvent.IsExpired ());
  Simulator::Cancel (m_requestEvent);

  TypeMessage typeHdr; 
  // Create a Demand Response packet 
  Ptr<Packet> packet = Create<Packet> (); 

  // Build the Demand Response Message and add header into the packet
  MeterPollRequestMessageHeader hdr;
  hdr.SetReadingType (0); // Request the Total Average Active energy (kWh)
  hdr.SetReadingTime (m_readingTime); // Time to read the smart Meter by Data Concentrator 
  packet->AddHeader (hdr); // Add the Demand Response Message header into the packet 

  typeHdr.SetMessageType (METER_POLL_REQ); // Define the Message type 
  packet->AddHeader (typeHdr); 

  // Request of data to Data Concentrators at AMI network
  std::vector<Ptr<Application> >::const_iterator i;
  Time t = m_nextTimeRequest; // Next YY secs (established at script) 
  uint32_t j = 0;
  for (i = m_containerDcApp.Begin (); i != m_containerDcApp.End (); ++i)
    { 
      Ptr<Application> app = m_containerDcApp.Get (j++); 
      Ptr<DataConcentratorApplication> dcApp = app->GetObject<DataConcentratorApplication> (); 
      Address currentDcAddress = dcApp->GetLocalAddress ();
      // Event: Send the PDU to the lower layers ---> each event has independent argument (i.e. currentDcAddress)???
      m_sendEvent = Simulator::Schedule (t, &DemandResponseApplication::Send, this, packet, currentDcAddress);
      t += Seconds (0.001); // increase by 1ms 
    }
 
  // Event: Set a timer that permits to request new data to the AMI's Data Concentrators
  m_requestEvent = Simulator::Schedule (m_nextTimeRequest, &DemandResponseApplication::Request, this);
}

void 
DemandResponseApplication::RequestNextBlock (uint16_t blockNumber)
{
  NS_ASSERT (m_requestNextBlockEvent.IsExpired ());
  Simulator::Cancel (m_requestNextBlockEvent);

  TypeMessage typeHdr; 
  // Create a Demand Response packet 
  Ptr<Packet> packet = Create<Packet> (); 

  // Build the Demand Response Message and add header into the packet
  MeterPollRequestNextMessageHeader hdr;
  hdr.SetBlockNumber (blockNumber); // Number of the meter data block received before
  packet->AddHeader (hdr); // Add the Demand Response Message header into the packet 

  typeHdr.SetMessageType (METER_POLL_REQ_NEXT); // Define the Message type 
  packet->AddHeader (typeHdr); 

  // Request the next block of data to Data Concentrators at AMI network
  std::vector<Ptr<Application> >::const_iterator i;
  Time t = Seconds(0.0); // Next YY secs (established at script) 
  uint32_t j = 0;
  for (i = m_containerDcApp.Begin (); i != m_containerDcApp.End (); ++i)
    { 
      Ptr<Application> app = m_containerDcApp.Get (j++); 
      Ptr<DataConcentratorApplication> dcApp = app->GetObject<DataConcentratorApplication> (); 
      Address currentDcAddress = dcApp->GetLocalAddress ();
      // Event: Send the PDU to the lower layers ---> each event has independent argument (i.e. currentDcAddress)???
      m_sendEvent = Simulator::Schedule (t, &DemandResponseApplication::Send, this, packet, currentDcAddress);
      t += Seconds (0.001); // increase by 1ms 
    }
}

void 
DemandResponseApplication::Command (uint8_t signalType, uint32_t command, Address currentDcAddress, uint32_t customerId)
{
  NS_ASSERT (m_commandEvent.IsExpired ());
  Simulator::Cancel (m_commandEvent);

  Time t; // auxiliary time variable  
  TypeMessage typeHdr; 
  // Create a Demand Response packet
  Ptr<Packet> packet = Create<Packet> (); 
  
  if (signalType == S_CONTROL)
    {
      // Build the Demand Response Message and add header into the packet
      ControlMessageHeader hdr;
      hdr.SetCommand (command); // the command paramenter "contains" the control or curtailment commands
      hdr.SetCustomerId (customerId);  // Assign the command to the Meter with the customer Id specified 
      packet->AddHeader (hdr); // Add the Demand Response Message header into the packet 

      typeHdr.SetMessageType (CONTROL); // Define the Message type 
      packet->AddHeader (typeHdr); 

      t = Seconds (0.0); // Immediately
    } 

  if (signalType == S_PRICE)
    {
      // Build the Demand Response Message and add header into the packet
      PriceMessageHeader hdr;
      hdr.SetPrice (command); // the command paramenter "contains" the updated price
      hdr.SetCustomerId (customerId); // Assign the command to the Meter with the customer Id specified 
      packet->AddHeader (hdr); // Add the Demand Response Message header into the packet 

      typeHdr.SetMessageType (PRICE); // Define the Message type 
      packet->AddHeader (typeHdr); 

      t = Seconds (0.0); // Immediately
    } 

  // Event: Send the PDU to the lower layers ---> each event has independent argument (i.e. currentDcAddress)???
  m_sendEvent = Simulator::Schedule (t, &DemandResponseApplication::Send, this, packet, currentDcAddress);
}

void 
DemandResponseApplication::DemandResponseMechanism (uint32_t data, Address currentDcAddress)
{
  NS_ASSERT (m_demandResponseMechanismEvent.IsExpired ());
  Simulator::Cancel (m_demandResponseMechanismEvent);

  // XX-HERE: Call the Demand Response Mechanism, which return a command or price response ("signal")
  //uint8_t signalType = S_PRICE;
  uint8_t signalType = S_CONTROL;
  uint32_t command = 0;
  uint32_t customerId = 0;
 
  // Event: Construct and send the command message thrown by the Demand Response Mechanism
  m_commandEvent = Simulator::Schedule (Seconds (0.0), &DemandResponseApplication::Command, this, signalType, command, currentDcAddress, customerId);
}

void 
DemandResponseApplication::SetApplicationContainerDcApp (ApplicationContainer containerDcApp)
{
 m_containerDcApp = containerDcApp;
}

void 
DemandResponseApplication::SetLocalAddress (Address ip)
{
  m_localAddress = ip;
}

Address
DemandResponseApplication::GetLocalAddress ()
{
  return m_localAddress;
}

void 
DemandResponseApplication::SetNextTimeRequest (Time nextTimeRequest)
{
  m_nextTimeRequest = nextTimeRequest;
}

Time 
DemandResponseApplication::GetNextTimeRequest ()
{
  return m_nextTimeRequest;
}

void
DemandResponseApplication::SetReadingTime (uint32_t readingTime)
{
  m_readingTime = readingTime;
}

uint32_t
DemandResponseApplication::GetReadingTime ()
{ 
  return m_readingTime;
}

void
DemandResponseApplication::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
DemandResponseApplication::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // Create the socket
  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::ConvertFrom(m_localAddress), 5050);
      m_socket->Bind (local);
    }

  m_socket->SetRecvCallback (MakeCallback (&DemandResponseApplication::Recv, this));

  // Event: Request data to AMI's Data Concentrator for the first time
  m_requestEvent = Simulator::Schedule (Seconds(0.0), &DemandResponseApplication::Request, this);
}

void 
DemandResponseApplication::StopApplication (void)
{
  // Cancel Events
  Simulator::Cancel (m_sendEvent);
  Simulator::Cancel (m_requestEvent);
  Simulator::Cancel (m_requestNextBlockEvent);
  Simulator::Cancel (m_commandEvent);
  Simulator::Cancel (m_demandResponseMechanismEvent);
}	

} // namespace ns3
