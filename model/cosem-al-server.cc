/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-al-server.cc
 * Implementation of the Class CosemAlServer
 * Class that represents the COSEM Application Layer - Server side
 * Created on: 26-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "cosem-al-server.h"
#include "cosem-ap-server.h"
#include "udp-cosem-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosemApplicationLayerServer");
NS_OBJECT_ENSURE_REGISTERED (CosemAlServer);

TypeId 
CosemAlServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CosemAlServer")
    .SetParent<CosemAl> ()
    .AddConstructor<CosemAlServer> ()
    ;
  return tid;
}

CosemAlServer::CosemAlServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_udpPort = 4056;
}

CosemAlServer::~CosemAlServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CosemAlServer::CosemAcseOpen (int typeService, Ptr<CosemApServer> sap)
{

}
	
void 
CosemAlServer::CosemAcseRelease (int typeService, Ptr<CosemApServer> sap)
{

}
	
void 
CosemAlServer::CosemXdlmsGet (int typeGet, int typeService, Ptr<CosemApServer> sap)
{

}
		
void 
CosemAlServer::RecvCosemApduTcp (int tcpsService, Ptr<Packet> packet)
{

}

void 
CosemAlServer::RecvCosemApduUdp (Ptr<Packet> packet)
{

}
	
void 
CosemAlServer::SetCosemApServer (Ptr<CosemApServer> cosemApServer)
{
  m_cosemApServer = cosemApServer;
}
	
Ptr<CosemApServer> 
CosemAlServer::GetCosemApServer ()
{
  return m_cosemApServer;
}

void 
CosemAlServer::SetCosemWrapperServer (Ptr<UdpCosemWrapperServer> cosemWrapperServer)
{
  m_cosemWrapperServer = cosemWrapperServer;
}
	
Ptr<UdpCosemWrapperServer> 
CosemAlServer::GetCosemWrapperServer ()
{
  return m_cosemWrapperServer;
}

void 
CosemAlServer::SetUdpport (uint16_t udpPort)
{
  m_udpPort = udpPort ;
}

uint16_t 
CosemAlServer::GetUdpport ()
{
  return m_udpPort;
}
	
} // namespace ns3
