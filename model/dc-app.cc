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
#include "cosem-ap-client.h"
#include "dc-app.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DataConcentratorApplication");
NS_OBJECT_ENSURE_REGISTERED (DataConcentratorApplication);

TypeId 
DataConcentratorApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DataConcentratorApplication")
    .SetParent<Application> ()
    .AddConstructor<DataConcentratorApplication> ()
    ;
  return tid;
}

DataConcentratorApplication::DataConcentratorApplication ()
{
  // For debugging purposes
  //NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s DcApp created!");
  m_sendEvent = EventId ();
}

DataConcentratorApplication::~DataConcentratorApplication ()
{

}

void 
DataConcentratorApplication::Send (Ptr<Packet> packet)
{
  NS_ASSERT (m_sendEvent.IsExpired ());
  Simulator::Cancel (m_sendEvent);

  // Call SEND Udp function (through UdpSocket)
  m_socket->Send (packet); 
  NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s DC (" 
                                               << Ipv4Address::ConvertFrom (m_localAddress) 
                                               << ") sent " << packet->GetSize () << "B to CC ("
                                               << Ipv4Address::ConvertFrom (m_sgCenterAddress) << ")");
  // Reset Memory
  Simulator::Schedule (Seconds (0.0), &DcMemory::Reset, m_dcMemory);
}	

void 
DataConcentratorApplication::RecvSm (uint32_t sizeData)
{
  Simulator::Schedule (Seconds (0.0), &DcMemory::Access, m_dcMemory, sizeData, STORAGE); 
}

void 
DataConcentratorApplication::RecvSg (Ptr<Socket> socket)
{
  Ptr<Packet> pkt;
  Address from;
  // Retreive the packet sent by the Smart Grid Center
  pkt = socket->RecvFrom (from);  

  if (InetSocketAddress::IsMatchingType (from))
    {
    if (pkt->GetSize () != 50)  // i.e. different from PRICE or COntrol signal
      { 
        NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s DC (" 
                                                 << Ipv4Address::ConvertFrom (m_localAddress) 
                                                 << ") received a request (" << pkt->GetSize () << "B) from CC ("
                                                 << InetSocketAddress::ConvertFrom (from).GetIpv4 () << ")");
        // Check for data in memory
        uint32_t data = m_dcMemory->Access (0, RETRIEVE); 
        if (data == 0)
          {
            NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s Empty MEMORY! Requesting data to Smart Meters!");
                                                   
            // Doesn't exist data in the memory, request to the SMs
            Simulator::Schedule (Seconds (0.0), &CosemApClient::NewRequest, m_cosemApClient); 
          } 
        else
          {
            // Exist data in the memory, encapsulate them and send a packet to the SG Center 
            Ptr<Packet> packet = Create<Packet> (data);
            m_sendEvent = Simulator::Schedule (Seconds (0.0), &DataConcentratorApplication::Send, this, packet);
          }
      }
    else
      {
        NS_LOG_INFO (Simulator::Now ().GetSeconds () << "s DC (" 
                                                     << Ipv4Address::ConvertFrom (m_localAddress) 
                                                     << ") received a command signal (" << pkt->GetSize () << "B) from CC ("
                                                     << InetSocketAddress::ConvertFrom (from).GetIpv4 () << ")");
      }
   }
}

void 
DataConcentratorApplication::SetSgCenterAddress (Address ip)
{
  m_sgCenterAddress = ip;
}

Address 
DataConcentratorApplication::GetSgCenterAddress ()
{
  return m_sgCenterAddress;
}

void 
DataConcentratorApplication::SetLocalAddress (Address ip)
{
  m_localAddress = ip;
}

Address
DataConcentratorApplication::GetLocalAddress ()
{
  return m_localAddress;
}

void 
DataConcentratorApplication::SetCosemApClient (Ptr<CosemApClient> cosemApClient)
{
  m_cosemApClient = cosemApClient;
}

Ptr<CosemApClient> 
DataConcentratorApplication::GetCosemApClient ()
{
  return m_cosemApClient;
}

void 
DataConcentratorApplication::SetDcMemory (Ptr<DcMemory> dcMemory)
{
  m_dcMemory = dcMemory;
}

Ptr<DcMemory> 
DataConcentratorApplication::GetDcMemory ()
{
  return m_dcMemory;
}

void
DataConcentratorApplication::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
DataConcentratorApplication::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

 // Create the socket
  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::ConvertFrom(m_localAddress), 5050);
      m_socket->Bind (local);
      m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom (m_sgCenterAddress), 5050));
    }

   m_socket->SetRecvCallback (MakeCallback (&DataConcentratorApplication::RecvSg, this));

   // build callback instance which points to DataConcentratorApplication::RecvSm function
   m_cosemApClient->SetRecvCallback (MakeCallback (&DataConcentratorApplication::RecvSm, this)); 
}

void 
DataConcentratorApplication::StopApplication (void)
{

}

/*-----------------------------------------------------------------------------
 *  Data Concentrator's Memory
 *-----------------------------------------------------------------------------
 */

NS_OBJECT_ENSURE_REGISTERED (DcMemory);

TypeId 
DcMemory::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DcMemory")
    .SetParent<Object> ()
    .AddConstructor<DcMemory> ()
    ;
  return tid;
}

DcMemory::DcMemory ()
{
 NS_LOG_FUNCTION_NOARGS ();
 m_memory = 0;
}

DcMemory::~DcMemory ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

uint32_t
DcMemory::Access (uint32_t data, uint8_t memoryProcess)
{
  if (memoryProcess == STORAGE)
    {	
      // Store the data in the memory
      Storage (data);   
    }

  if (memoryProcess == RETRIEVE)
    {
      // Retreive the data in the memory
      uint32_t ndata = Retrieve ();
      return ndata;
    } 

 return -1;
}

void 
DcMemory::Storage (uint32_t data)
{
  // Store data (bytes)
  m_memory += data;
}

uint32_t 
DcMemory::Retrieve ()
{
  // Retrun data (bytes)
  return m_memory;
}

void 
DcMemory::Reset ()
{
  m_memory = 0;
  NS_LOG_INFO (Simulator::Now ().GetSeconds () << " DC's memory reset");
}
	

} // namespace ns3
