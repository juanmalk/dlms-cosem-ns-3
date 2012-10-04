/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-al-client.cc
 * Implementation of the Class CosemAlServer
 * Class that represents the COSEM Application Layer - Client side
 * Created on: 26-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "cosem-al-client.h"
#include "cosem-ap-server.h"
#include "cosem-ap-client.h"
#include "cosem-al-server.h"
#include "udp-cosem-client.h"

//#include "tcp-cosem.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosemApplicationLayerClient");
NS_OBJECT_ENSURE_REGISTERED (CosemAlClient);

TypeId 
CosemAlClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CosemAlClient")
    .SetParent<CosemAl> ()
    .AddConstructor<CosemAlClient> ()
    ;
  return tid;
}

CosemAlClient::CosemAlClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_udpPort = 4056;
}

CosemAlClient::~CosemAlClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CosemAlClient::CosemAcseOpen (int typeService, Ptr<CosemApServer> sap)
{
  //.request
  if (typeService == REQUEST)
    {
      NS_LOG_INFO ("CAP-->OPEN.req (S)");	
      
      // Event: Change the state of CAL to ASSOCIATION_PENDING
      Simulator::Schedule (Seconds (0.0), &CosemAl::SetStateCf, this, CF_ASSOCIATION_PENDING, "CLIENT");

      // Event: Request an AA establishment: Construct the AARQ APDU of ACSE service 
      Simulator::Schedule (Seconds (0.0), &CosemAlClient::CosemAcseApdu, this, OPEN, REQUEST, sap);
    }

  //.confirm
  if (typeService == CONFIRM)
    {
      // Event: Change the state of CAL to ASSOCIATED
      Simulator::Schedule (Seconds (0.0), &CosemAl::SetStateCf, this, CF_ASSOCIATED, "CLIENT");
    
      // Event: Inform to the CAP that an remote SAP responses to its request for an establisment of an AA (OPEN.cnf(OK))
      Simulator::Schedule (Seconds (0.0), &CosemApClient::Recv, m_cosemApClient, -1, OPEN, -1, CONFIRM); // Check if it really works?????
    }
}
	
void 
CosemAlClient::CosemAcseRelease (int typeService, Ptr<CosemApServer> sap)
{

}
	
void 
CosemAlClient::CosemXdlmsGet (int typeGet, int typeService, Ptr<CosemApServer> sap)
{

}

void 
CosemAlClient::CosemAcseApdu (int typeAcseService, int typeService, Ptr<CosemApServer> sap)
{
  CosemAl::CosemAcseApdu (typeAcseService, typeService, sap);
}
	
void 
CosemAlClient::CosemXdlmsApdu (int typeGet, int typeService, Ptr<CosemApServer> sap)
{
  CosemAl::CosemXdlmsApdu (typeGet, typeService, sap);
}
	
void 
CosemAlClient::RecvCosemApduTcp (int tcpsService, Ptr<Packet> packet)
{

}

void 
CosemAlClient::RecvCosemApduUdp (Ptr<Packet> packet)
{

}
	
void 
CosemAlClient::SetCosemApClient (Ptr<CosemApClient> cosemApClient)
{
  m_cosemApClient = cosemApClient;
}
	
Ptr<CosemApClient> 
CosemAlClient::GetCosemApClient ()
{
  return m_cosemApClient;
}

void 
CosemAlClient::SetCosemWrapperClient (Ptr<UdpCosemWrapperClient> cosemWrapperClient)
{
  m_cosemWrapperClient = cosemWrapperClient;
}
	
Ptr<UdpCosemWrapperClient> 
CosemAlClient::GetCosemWrapperClient ()
{
  return m_cosemWrapperClient;
}

void 
CosemAlClient::SetUdpport (uint16_t udpPort)
{
  m_udpPort = udpPort ;
}

uint16_t 
CosemAlClient::GetUdpport ()
{
  return m_udpPort;
}
	
} // namespace ns3
