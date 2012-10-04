/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * udp-cosem-Server.cc
 * Implementation of the Class UdpCosemWrapperServer
 * Class that represents the UDP COSEM Wrapper transport sub-layer - Server side
 * Created on: 28-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#include "ns3/log.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "cosem-al-server.h"
#include "cosem-ap-server.h"
#include "udp-cosem-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpCosemWrapperLayerServer");
NS_OBJECT_ENSURE_REGISTERED (UdpCosemWrapperServer);

TypeId 
UdpCosemWrapperServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpCosemWrapperServer")
    .SetParent<Object> ()
    .AddConstructor<UdpCosemWrapperServer> ()
    ;
  return tid;
}

UdpCosemWrapperServer::UdpCosemWrapperServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_socket = 0;
  m_wPortSap = 0;
  m_wPortCap = 0;
  m_udpPort = 4056;
}

UdpCosemWrapperServer::~UdpCosemWrapperServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
UdpCosemWrapperServer::Recv (Ptr<Socket> socket)
{

}

void
UdpCosemWrapperServer::Send (Ptr<Packet> packet, Ptr<CosemApServer> cosemApServer)
{

}

void
UdpCosemWrapperServer::SetSocket (Ptr<Socket> socket)
{
  m_socket = socket;
}

Ptr<Socket>
UdpCosemWrapperServer::GetSocket ()
{
  return m_socket;
}

void 
UdpCosemWrapperServer::SetCosemAlServer (Ptr<CosemAlServer> cosemAlServer)
{
  m_cosemAlServer = cosemAlServer;
}

Ptr<CosemAlServer> 
UdpCosemWrapperServer::GetCosemAlServer ()
{
  return m_cosemAlServer;
}

void
UdpCosemWrapperServer::SetwPortServer (Ptr<CosemApServer> cosemApServer)
{
  cosemApServer->SetWport (m_wPortSap++);
}

uint16_t
UdpCosemWrapperServer::GetwPortServer ()
{
 return m_wPortSap;
}

uint16_t
UdpCosemWrapperServer::GetwPortClient ()
{
 return m_wPortCap;
}

void 
UdpCosemWrapperServer::SetUdpport (uint16_t udpPort)
{
  m_udpPort = udpPort ;
}

uint16_t 
UdpCosemWrapperServer::GetUdpport ()
{
  return m_udpPort;
}

void 
UdpCosemWrapperServer::SetLocalAddress (Address ip)
{
  m_localAddress = ip;
}

Address
UdpCosemWrapperServer::GetLocalAddress ()
{
  return m_localAddress;
}

void 
UdpCosemWrapperServer::SetRemoteAddress (Address ip)
{
  m_remoteAddress = ip;
}

Address
UdpCosemWrapperServer::GetRemoteAddress ()
{
  return m_remoteAddress;
}

} // namespace ns3
