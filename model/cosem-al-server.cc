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
