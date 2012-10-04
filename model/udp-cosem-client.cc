/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * udp-cosem-client.cc
 * Implementation of the Class UdpCosemWrapperClient
 * Class that represents the UDP COSEM Wrapper transport sub-layer - Client side
 * Created on: 28-Sept-2012
 * Modified on:
 * Original author: JUANMALK
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
