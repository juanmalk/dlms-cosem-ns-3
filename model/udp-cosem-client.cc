/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 JMALK
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
#include "cosem-ap-server.h"
#include "cosem-al-client.h"
#include "cosem-ap-client.h"
#include "udp-cosem-client.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpCosemWrapperLayerClient");
NS_OBJECT_ENSURE_REGISTERED (UdpCosemWrapperClient);

TypeId 
UdpCosemWrapperClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpCosemWrapperClient")
    .SetParent<Object> ()
    .AddConstructor<UdpCosemWrapperClient> ()
    ;
  return tid;
}

UdpCosemWrapperClient::UdpCosemWrapperClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_socket = 0;
  m_wPortCap = 0;
  m_udpPort = 4056;
}

UdpCosemWrapperClient::~UdpCosemWrapperClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
UdpCosemWrapperClient::Recv (Ptr<Socket> socket)
{

}

void
UdpCosemWrapperClient::Send (Ptr<Packet> packet, Ptr<CosemApServer> cosemApServer)
{

}

void
UdpCosemWrapperClient::SetSocket (Ptr<Socket> socket)
{
  m_socket = socket;
}

Ptr<Socket>
UdpCosemWrapperClient::GetSocket ()
{
  return m_socket;
}

void 
UdpCosemWrapperClient::SetCosemAlClient (Ptr<CosemAlClient> cosemAlClient)
{
  m_cosemAlClient = cosemAlClient;
}

Ptr<CosemAlClient> 
UdpCosemWrapperClient::GetCosemAlClient ()
{
  return m_cosemAlClient;
}

void
UdpCosemWrapperClient::SetwPortClient (Ptr<CosemApClient> cosemApClient)
{
  cosemApClient->SetWport (m_wPortCap++);
}

uint16_t
UdpCosemWrapperClient::GetwPortClient ()
{
 return m_wPortCap;
}

void 
UdpCosemWrapperClient::SetUdpport (uint16_t udpPort)
{
  m_udpPort = udpPort ;
}

uint16_t 
UdpCosemWrapperClient::GetUdpport ()
{
  return m_udpPort;
}

void 
UdpCosemWrapperClient::SetLocalAddress (Address ip)
{
  m_localAddress = ip;
}

Address
UdpCosemWrapperClient::GetLocalAddress ()
{
  return m_localAddress;
}

void 
UdpCosemWrapperClient::SetRemoteAddress (Address ip)
{
  m_remoteAddress = ip;
}

Address
UdpCosemWrapperClient::GetRemoteAddress ()
{
  return m_remoteAddress;
}

} // namespace ns3
