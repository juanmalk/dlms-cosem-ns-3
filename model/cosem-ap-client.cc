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
#include "ns3/ipv4-address.h"
#include "cosem-al-client.h"
#include "cosem-ap-server.h"
#include "cosem-ap-client.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosemApplicationsProcessClient");
NS_OBJECT_ENSURE_REGISTERED (CosemApClient);

TypeId 
CosemApClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CosemApClient")
    .SetParent<Application> ()
    .AddConstructor<CosemApClient> ()
    ;
  return tid;
}

CosemApClient::CosemApClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_wPort = 0;
  m_udpPort = 0;
  m_startRequestEvent = EventId ();
  m_typeRequesting = 0;
  m_nSap = 1;
}

CosemApClient::~CosemApClient ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CosemApClient::Recv (int nbytes, int typeAcseService, int typeGet, int typeService)
{
  NS_LOG_INFO ("CAL-->OPEN.cnf(Ok) (S)");
}

void 
CosemApClient::StartRequest ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_startRequestEvent.IsExpired ());
 //  uint32_t nSaps = m_containerSap.GetN ();      // Total number of Sap instances

  if (!m_typeRequesting)
    {
      NS_LOG_INFO ("Multicast Resquesting Mechanism");
      // do nothing
    }
  else
    {

      if (m_itSap == m_containerSap.Begin ())
        {
          NS_LOG_INFO ("Sequential Resquesting Mechanism");
          Ptr<Application> app = m_containerSap.Get (m_nSap);
          Ptr<CosemApServer> sap = app->GetObject<CosemApServer> ();  // Retrieve the i Saps pointer stored in AppContainer 
          m_itSap ++;  //Increase the value of "it" by one
          m_nSap ++;   //Increases by one the counter of Saps
          /* 
           * Invoke the COSEM-OPEN.req service implemented in CosemClient_AL_CF	
           * in order to establish an AA with a remote server (sap)
           */
          m_cosemAlClient->CosemAcseOpen (REQUEST, sap); 
        }
      else 
        {
          if (m_itSap != m_containerSap.End())
            {
              Ptr<Application> app = m_containerSap.Get (m_nSap);
              Ptr<CosemApServer> sap = app->GetObject<CosemApServer> ();  
              m_itSap ++;  
              m_nSap ++; 
              m_cosemAlClient->CosemAcseOpen (REQUEST, sap);         
            }
          else
            {
              m_nSap = 1;  
            }
        }
    }
}

void
CosemApClient::NewRequest ()
{

}

void 
CosemApClient::RequestRelease ()
{

}

double 
CosemApClient::NextTimeRequestSm ()
{
  return 0;
}

void 
CosemApClient::SaveActiveAa (uint16_t dstWport, Ptr<CosemApServer> sap)
{
  m_activeAa[dstWport] = sap;
}
	
void 
CosemApClient::RemoveActiveAa (Ptr<CosemApServer> sap)
{
  /*// Find the wPort of the current SAP
  m_it = activeAa.find(sap->get_cs_al_cf()->get_cwS()->get_dst_wport(sap));

  if (active_AAs_.end() != m_it)
    {
      delete m_it->second;	// Exists the connection
    } 
  else 
    {
      return;			// Not Exist the connection
    }*/
}

Ptr<CosemApServer> 
CosemApClient::ReturnActiveAa (uint16_t dstWport)
{
  // Find the wport of the current SAP
  m_it = m_activeAa.find(dstWport);
	
  if (m_activeAa.end() != m_it)
    {
      return m_it->second;	// Exists SAP
    } 
  else 
    {
      return 0;		// Not Exist SAP
    }
}

void 
CosemApClient::SetCosemAlClient (Ptr<CosemAlClient> cosemAlClient)
{
  m_cosemAlClient = cosemAlClient;
}

Ptr<CosemAlClient> 
CosemApClient::GetCosemAlClient ()
{
  return m_cosemAlClient;
}

void 
CosemApClient::SetWport (uint16_t wPort)
{
  m_wPort = wPort;
}

uint16_t 
CosemApClient::GetWport ()
{
  return m_wPort;
}

void 
CosemApClient::SetUdpport (uint16_t udpPort)
{
  m_udpPort = udpPort ;
}

uint16_t 
CosemApClient::GetUdpport ()
{
  return m_udpPort;
}

void 
CosemApClient::SetLocalAddress (Address ip)
{
  m_localAddress = ip;
}

Address
CosemApClient::GetLocalAddress ()
{
  return m_localAddress;
}

void 
CosemApClient::SetApplicationContainerSap (ApplicationContainer containerSap)
{
  m_containerSap = containerSap;
}

void 
CosemApClient::SetTypeRequesting (bool typeRequesting)
{
  m_typeRequesting = typeRequesting;
}

bool
CosemApClient::GetTypeRequesting ()
{
  return m_typeRequesting;
}


void
CosemApClient::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
CosemApClient::StartApplication (void)
{ 
  // Create the StartRequest Event
  m_startRequestEvent = Simulator::Schedule (Seconds (0.0), &CosemApClient::StartRequest, this);
  // Set the iterator at the begining of the container
  m_itSap = m_containerSap.Begin ();
}

void 
CosemApClient::StopApplication (void)
{

}

} // namespace ns3

