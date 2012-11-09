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
                                               << ") sent " << packet->GetSize () << "B to DC ("
                                               << Ipv4Address::ConvertFrom (currentDcAddress) << ")");
    } 
}	

void 
DemandResponseApplication::Recv (Ptr<Socket> socket)
{
  Ptr<Packet> pkt;
  Address from;
  // Retreive the packet sent by the Smart Grid Center
  pkt = socket->RecvFrom (from);  

  if (InetSocketAddress::IsMatchingType (from))
    {
      // Record the received data
      /*CosemPollResponsePduHeader hdr
      packet->RemoveHeader (hdr);
      uint32_t data = hdr.GetData ();
      */
      uint32_t data = pkt->GetSize ();   
      NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s CC (" 
                                                   << Ipv4Address::ConvertFrom (m_localAddress) 
                                                   << ") received " << pkt->GetSize () << "B from DC ("
                                                   << InetSocketAddress::ConvertFrom (from).GetIpv4 () << ")");

      // Event: Call Demand Response Mechanism to analyse the received data
      Address currentDcAddress = Address (InetSocketAddress::ConvertFrom (from).GetIpv4 ());
      m_demandResponseMechanismEvent = Simulator::Schedule (Seconds (0.0), &DemandResponseApplication::DemandResponseMechanism, this, data, currentDcAddress);
    }
}

void 
DemandResponseApplication::Request ()
{
  NS_ASSERT (m_requestEvent.IsExpired ());
  Simulator::Cancel (m_requestEvent);

  // Create a Demand Response packet of 30 bytes
  Ptr<Packet> packet = Create<Packet> (30); 

  // Build the PDU and add header into the packet
  /* CosemPollRequestPduHeader hdr;
  hdr.SetCommad (command);  
  packet->AddHeader (hdr); // Copy the header into the packet
  TypeAPDU typeHdr;
  typeHdr.SetApduType ((PduType)hdr.GetIdPdu()); // Define the type of PDU
  packet->AddHeader (typeHdr); // Copy the header into the packet 
  */

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
DemandResponseApplication::Command (uint8_t messageType, uint32_t command, Address currentDcAddress)
{
  NS_ASSERT (m_commandEvent.IsExpired ());
  Simulator::Cancel (m_commandEvent);

  Time t; // auxiliary time variable  

  // Create a Demand Response packet of 50 bytes
  Ptr<Packet> packet = Create<Packet> (50); 
  
  if (messageType == CONTROL)
    {
      // Build the PDU and add header into the packet
      /* CosemControlPduHeader hdr;
      hdr.SetCommad (command);  
      packet->AddHeader (hdr); // Copy the header into the packet
      TypeAPDU typeHdr;

      typeHdr.SetApduType ((PduType)hdr.GetIdPdu()); // Define the type of PDU
      packet->AddHeader (typeHdr); // Copy the header into the packet 
      */

      t = Seconds (0.0); // Immediately
    } 

  if (messageType == PRICE)
    {
      // Build the PDU and add header into the packet
 
      /* CosemPricePduHeader hdr;

      hdr.SetCommad (command);  
      packet->AddHeader (hdr); // Copy the header into the packet
      TypeAPDU typeHdr;
      typeHdr.SetApduType ((PduType)hdr.GetIdPdu()); // Define the type of PDU

      packet->AddHeader (typeHdr); // Copy the header into the packet 
      */
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

  // HERE: Call the Demand Response Mechanism, which return a command or price response ("signal")
  uint8_t messageType = PRICE;
  uint32_t command = 0;

  // Event: Construct and send the command message thrown by the Demand Response Mechanism
  m_commandEvent = Simulator::Schedule (Seconds (0.0), &DemandResponseApplication::Command, this, messageType, command, currentDcAddress);
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
  Simulator::Cancel (m_commandEvent);
  Simulator::Cancel (m_demandResponseMechanismEvent);
}	

} // namespace ns3
