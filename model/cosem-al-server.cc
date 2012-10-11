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
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "cosem-header.h"
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
    .SetParent<Object> ()
    .AddConstructor<CosemAlServer> ()
    ;
  return tid;
}

CosemAlServer::CosemAlServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_typeService = REQUEST;
  m_typeGet = GET_NORMAL;
  m_stateCf = CF_INACTIVE;
  m_udpPort = 4056;
  m_changeStateEvent = EventId ();
  m_sendApduEvent = EventId ();
  m_invokeCosemServiceEvent = EventId ();
}

CosemAlServer::~CosemAlServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CosemAlServer::CosemAcseOpen (int typeService)
{
  if (typeService == INDICATION)
    {
      NS_ASSERT (m_invokeCosemServiceEvent.IsExpired ());
      Simulator::Cancel (m_invokeCosemServiceEvent);  // necessary?

      // Event: Inform to the SAP that a remote CAP has requested an establisment of an AA
      Simulator::Schedule (Seconds (0.0), &CosemApServer::Recv, m_cosemApServer, -1, OPEN, -1, typeService); // Check if it really works?????
    }
  else if (typeService == RESPONSE)
    {
      NS_LOG_INFO ("SAP-->OPEN.res (S)");
      // Event: Construct the AARE APDU of ACSE service 
      m_invokeCosemServiceEvent = Simulator::Schedule (Seconds (0.0), &CosemAlServer::CosemAcseApdu, this, OPEN, typeService);
    }
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
CosemAlServer::CosemAcseApdu (int typeAcseService, int typeService)
{

  NS_ASSERT (m_invokeCosemServiceEvent.IsExpired ());
  Simulator::Cancel (m_invokeCosemServiceEvent);  // necessary?

  if ((typeAcseService == OPEN) && (typeService == RESPONSE))
    {
      // Build an xDLMS-Initiate.res PDU and an AARE APDU
      CosemAareHeader hdr;
      hdr.SetApplicationContextName (0);  // {N/A}Application Context Name (rules that govern th exchange-parameters)
      hdr.SetResult (0);  // Result of the request AA, {0, accepted}
      hdr.SetResultSourceDiagnostic (0);  // Result of a rejection of the a request AA, {0, null}

      hdr.SetNegotiatedQualityOfService (0);  // Not used, {0, N/A}
      hdr.SetNegotiatedDlmsVersionNumber (6);  // Version number, {6}
      hdr.SetNegotiatedConformance (0x001010);  // {0x001010}, Based on the example in Annex C IEC 62056-53
      hdr.SetServerMaxReceivePduSize (0x1F4);   // Server_Max_Receive_PDU_Size,{0x1F4}: 500 bytes
      hdr.SetVaaName (0x0007);  // Dummy Value {0x0007}.Taken from page 98 IEC 62056-53
   
      Ptr<Packet> packet = Create<Packet> (hdr.GetSerializedSize ()); // Create the AARQ APDU packet
      packet->AddHeader (hdr); // Copy the header into the packet

      TypeAPDU typeHdr;
      typeHdr.SetApduType ((ApduType)hdr.GetIdApdu()); // Define the type of APDU
      packet->AddHeader (typeHdr); // Copy the header into the packet
    
      NS_LOG_INFO ("SAL--> AARE APDU (S)");

      // Event: Change the state of SAL to ASSOCIATED
      m_changeStateEvent = Simulator::Schedule (Seconds (0.0), &CosemAlServer::SetStateCf, this, CF_ASSOCIATED);

      // Event: Send the APDU to the sub-layer Wrapper (Invoke UDP-DATA.req (APDU))
      double t = (8*hdr.GetSerializedSize ())/(500000) + 2.235e-3;
      m_sendApduEvent = Simulator::Schedule (Seconds (t), &CosemAlServer::sendApdu, this, packet);
    }
  else if ((typeAcseService == RELEASE) && (typeService == RESPONSE))
    {
      // code     
    }
  else
    {
      NS_LOG_INFO ("Error: Undefined request Cosem service");     

    }
}
	
void 
CosemAlServer::CosemXdlmsApdu (int typeGet, int typeService)
{

}
		
void 
CosemAlServer::RecvCosemApduTcp (int tcpsService, Ptr<Packet> packet)
{

}

void 
CosemAlServer::RecvCosemApduUdp (Ptr<Packet> packet)
{
  TypeAPDU typeHdr;
  
  // Copy the Cosem APDU Header from the packet
  packet->RemoveHeader (typeHdr);
  ApduType typeAPDU = typeHdr.GetApduType (); 
 
  if (typeAPDU == AARQ)
    {
      // Extract Associate.ind and xDLMS-Initiate.req parameters
      CosemAarqHeader hdr;
      packet->RemoveHeader (hdr);
      // Event: Invoke COSEM-OPEN.ind service
      m_invokeCosemServiceEvent = Simulator::Schedule (Seconds (0.0), &CosemAlServer::CosemAcseOpen, this, INDICATION);
      // Event: Change the state of SAL to ASSOCIATION_PENDING
      m_changeStateEvent = Simulator::Schedule (Seconds (0.0), &CosemAlServer::SetStateCf, this, CF_ASSOCIATION_PENDING);
    }
}

void 
CosemAlServer::sendApdu (Ptr<Packet> packet)
{
  NS_ASSERT (m_sendApduEvent.IsExpired ());
  Simulator::Cancel (m_sendApduEvent);
  m_cosemWrapperServer->Send (packet);
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
CosemAlServer::SetStateCf (int state)
{
  NS_ASSERT (m_changeStateEvent.IsExpired ());
  Simulator::Cancel (m_changeStateEvent);  // necessary?

  if (state == CF_IDLE)
    {
      m_stateCf = state;
      NS_LOG_INFO ("SAL-->IDLE (C)");
    } 
  else if (state == CF_ASSOCIATION_PENDING)    
      {
        m_stateCf = state;
        NS_LOG_INFO ("SAL-->ASSOCIATION_PENDING (C)");
      }
  else if (state == CF_ASSOCIATED)    
      {
        m_stateCf = state;
        NS_LOG_INFO ("SAL-->ASSOCIATED (C)");
      } 
  else if (state == CF_ASSOCIATION_RELEASE_PENDING)    
      {
        m_stateCf = state;
        NS_LOG_INFO ("SAL-->ASSOCIATION_RELEASE_PENDING (C)");
      } 
  else
    {
      NS_LOG_INFO ("Error: No change of stare AL");
    }
}

int 
CosemAlServer::GetStateCf ()
{
  return m_stateCf;
}
	
int 
CosemAlServer::GetTypeService ()
{
  return m_typeService;
}

void 
CosemAlServer::SetTypeService (int typeService)
{
  m_typeService = typeService;
}
	
int 
CosemAlServer::GetTypeGet ()
{
  return m_typeGet;
}

void 
CosemAlServer::SetTypeGet (int typeGet)
{
  m_typeGet = typeGet;
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
