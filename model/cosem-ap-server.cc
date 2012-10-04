/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-ap-server.cc
 * Implementation of the Class CosemApServer
 * Class that represents the COSEM Server Application Process
 * Created on: 26-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#include "ns3/log.h"
#include "cosem-al-server.h"
#include "cosem-ap-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosemApplicationsProcessServer");
NS_OBJECT_ENSURE_REGISTERED (CosemApServer);

TypeId 
CosemApServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CosemApServer")
    .SetParent<Application> ()
    .AddConstructor<CosemApServer> ()
    ;
  return tid;
}

CosemApServer::CosemApServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_wPort = 0;
  m_udpPort = 0;
}

CosemApServer::~CosemApServer ()
{

}

void 
CosemApServer::Recv (int nbytes)
{

}

void 
CosemApServer::SetCosemAlServer (Ptr<CosemAlServer> cosemAlServer)
{
  m_cosemAlServer = cosemAlServer;
}

Ptr<CosemAlServer> 
CosemApServer::GetCosemAlServer ()
{
  return m_cosemAlServer;
}

void
CosemApServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
CosemApServer::SetWport (uint16_t wPort)
{
  m_wPort = wPort;
}

uint16_t 
CosemApServer::GetWport ()
{
  return m_wPort;
}

void 
CosemApServer::SetUdpport (uint16_t udpPort)
{
  m_udpPort = udpPort ;
}

uint16_t 
CosemApServer::GetUdpport ()
{
  return m_udpPort;
}

void 
CosemApServer::SetLocalAddress (Address ip)
{
  m_localAddress = ip;
}

Address
CosemApServer::GetLocalAddress ()
{
  return m_localAddress;
}


void 
CosemApServer::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CosemApServer::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}

} // namespace ns3



