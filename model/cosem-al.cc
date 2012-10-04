/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-al.cc
 * Implementation of the Class CosemAl
 * Base class: COSEM Application Layer (AL) - Based class
 * Created on: 25-Sept-2012
 * Modified on:
 * Original author: JUANMAL
 */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "cosem-al.h"
#include "cosem-ap-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosemApplicationLayer");
NS_OBJECT_ENSURE_REGISTERED (CosemAl);

TypeId 
CosemAl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CosemAl")
    .SetParent<Object> ()
    .AddConstructor<CosemAl> ()
    ;
  return tid;
}

CosemAl::CosemAl ()
{
  m_typeService = REQUEST;
  m_typeGet = GET_NORMAL;
  m_stateCf[0] = CF_INACTIVE;
  m_stateCf[1] = CF_INACTIVE;
}

CosemAl::~CosemAl ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CosemAl::CosemAcseOpen (int typeService, Ptr<CosemApServer> sap)
{
  NS_LOG_FUNCTION_NOARGS ();
};
	
void 
CosemAl::CosemAcseRelease (int typeService, Ptr<CosemApServer> sap)
{

};
	
void 
CosemAl::CosemXdlmsGet (int typeGet, int typeService, Ptr<CosemApServer> sap)
{

};
	
void 
CosemAl::CosemAcseApdu (int typeAcseService, int typeService, Ptr<CosemApServer> sap)
{
  if (typeService == REQUEST)
    {
      if (typeAcseService == OPEN)
        {
          // Create AARQ APDU
          Ptr<Packet> packet = Create<Packet> (13);

        }
      else
        {

        }
    }
  else if (typeService == RESPONSE)
    {
      if (typeAcseService == OPEN)
        {

        }
      else
        {

        }
    }
}
	
void 
CosemAl::CosemXdlmsApdu (int typeGet, int typeService, Ptr<CosemApServer> sap)
{

}

void 
CosemAl::RecvCosemApduTcp (int tcpsService, Ptr<Packet> packet)
{

};

void 
CosemAl::RecvCosemApduUdp (Ptr<Packet> packet)
{

};
	
void 
CosemAl::SetStateCf (int state, const char* type)
{
  if (state == CF_IDLE)
    {
      if (strcmp(type, "CLIENT") == 0)
        {
          m_stateCf[0] = state;
          NS_LOG_INFO ("CAL-->IDLE (C)");
        } 
    else if (strcmp(type, "SERVER") == 0)
        {
          m_stateCf[1] = state;
          NS_LOG_INFO ("SAL-->IDLE (C)");
        }
    }
  else if (state == CF_ASSOCIATION_PENDING)    
      {
        if (strcmp(type, "CLIENT") == 0)
          {
            m_stateCf[0] = state;
            NS_LOG_INFO ("CAL-->ASSOCIATION_PENDING (C)");
          } 
        else if (strcmp(type, "SERVER") == 0)
          {
            m_stateCf[1] = state;
            NS_LOG_INFO ("SAL-->ASSOCIATION_PENDING (C)");
          }
      }
  else if (state == CF_ASSOCIATED)    
      {
        if (strcmp(type, "CLIENT") == 0)
          {
            m_stateCf[0] = state;
            NS_LOG_INFO ("CAL-->ASSOCIATED (C)");
          } 
        else if (strcmp(type, "SERVER") == 0)
          {
            m_stateCf[1] = state;
            NS_LOG_INFO ("SAL-->ASSOCIATED (C)");
          }
      }
  else if (state == CF_ASSOCIATION_RELEASE_PENDING)    
      {
        if (strcmp(type, "CLIENT") == 0)
          {
            m_stateCf[0] = state;
            NS_LOG_INFO ("CAL-->ASSOCIATION_RELEASE_PENDING (C)");
          } 
        else if (strcmp(type, "SERVER") == 0)
          {
            m_stateCf[1] = state;
            NS_LOG_INFO ("SAL-->ASSOCIATION_RELEASE_PENDING (C)");
          }
      }
  else
    {
      NS_LOG_INFO ("Error: No change of stare AL");
    }
}

int 
CosemAl::GetStateCf (const char* type)
{
  if (strcmp(type, "CLIENT") == 0)
    {
      return m_stateCf[0];
    } 
  else if (strcmp(type, "SERVER") == 0)
    {
      return m_stateCf[1];
    }
  return 0;
}
	
int 
CosemAl::GetTypeService ()
{
  return m_typeService;
}

void 
CosemAl::SetTypeService (int typeService)
{
  m_typeService = typeService;
}
	
int 
CosemAl::GetTypeGet ()
{
  return m_typeGet;
}

void 
CosemAl::SetTypeGet (int typeGet)
{
  m_typeGet = typeGet;
}
  
void 
CosemAl::sendApdu ()
{

}

} // namespace ns3


