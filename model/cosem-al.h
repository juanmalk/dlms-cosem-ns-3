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

#ifndef COSEM_AL_H
#define COSEM_AL_H

#include "ns3/object.h"
#include "ns3/simulator.h"

namespace ns3 {

class Packet;
class CosemApServer;

/** 
 * COSEM Application Layer (AL) Control Function
 *
 */
class CosemAl : public Object
{
public:
  static TypeId GetTypeId (void);	

  CosemAl (); 

  virtual ~CosemAl ();	
  
  // COSEM-OPEN service of ACSE, implemented by the derived class
  virtual void CosemAcseOpen (int typeService, Ptr<CosemApServer> sap);
	
  // COSEM-RELEASE service of ACSE, implemented by the derived class
  virtual void CosemAcseRelease (int typeService, Ptr<CosemApServer> sap);
	
  // COSEM-GET service of xDLMS_ASE, implemented by the derived class
  virtual void CosemXdlmsGet (int typeGet, int typeService, Ptr<CosemApServer> sap);

  // Construct the APDUs of ACSE services (AARQ, AARE, RLRQ, RLRE)
  virtual void CosemAcseApdu (int typeAcseService, int typeService, Ptr<CosemApServer> sap);
	
  // Construct the APDUs of xDLMS_ASE services(GET-REQUEST, GET-RESPONSE)
  virtual void CosemXdlmsApdu (int typeGet, int typeService, Ptr<CosemApServer> sap);
	
  // Received the indication/confirmation of a TCP-DATA resquest
  virtual void RecvCosemApduTcp (int tcpsService, Ptr<Packet> packet);
 
  // Received the indication/confirmation of a UDP-DATA resquest
  virtual void RecvCosemApduUdp (Ptr<Packet> packet);
	
 /**
  * type: client (state_cf_[0];) or server CF (state_cf_[1]) 
  * Set the state of CF
  */
  void SetStateCf (int state, const char* type);

  // Get the state of CF
  int GetStateCf (const char* type);
	
  // Set the type of service
  int GetTypeService ();

  // Get the type of service
  void SetTypeService (int typeService);
	
  // Get the type of COSEM-GET service
  int GetTypeGet ();

  // Set the type of COSEM-GET service
  void SetTypeGet (int typeGet);
  
  // Send the APDU to the sub-layer Wrapper
  void sendApdu ();

  // Type of services
  enum typeService { REQUEST, INDICATION, RESPONSE, CONFIRM };
	
  // States machine of the Control Function
  enum stateCf { CF_INACTIVE, CF_IDLE, CF_ASSOCIATION_PENDING, CF_ASSOCIATED, CF_ASSOCIATION_RELEASE_PENDING };
	
  // Type of COSEM-GET service
  enum typeGet { GET_NORMAL, GET_WITH_LIST, GET_ONE_BLOCK, GET_LAST_BLOCK };

  // TCP service
  enum tcpService { CONNECT, DATA, DISCONNECT, ABORT };	

  // ACSE services
  enum typeAcseService { OPEN, RELEASE };

private:		
 
  int m_typeService;  // Type of service
  int m_typeGet;   // Type of COSEM-GET service
  int m_stateCf[2];  // State of CF

};

} // namespace ns3

#endif /* COSEM_AL_H */
