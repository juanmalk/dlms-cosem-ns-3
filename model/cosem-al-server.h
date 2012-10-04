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

#ifndef COSEM_AL_SERVER_H
#define COSEM_AL_SERVER_H

#include "cosem-al.h"
#include "ns3/simulator.h"

namespace ns3 {

class CosemApServer;
class UdpCosemWrapperServer;

//class CosemWrapperServer;	

/** 
 * COSEM Application Layer (AL) Control Function - Server
 *
 */

class CosemAlServer : public CosemAl
{
public:
  static TypeId GetTypeId (void);
  
  CosemAlServer ();
 
  virtual ~CosemAlServer ();

  // COSEM-OPEN service of ACSE, implemented by the derived class
  void CosemAcseOpen (int typeService, Ptr<CosemApServer> sap);
	
  // COSEM-RELEASE service of ACSE, implemented by the derived class
  void CosemAcseRelease (int typeService, Ptr<CosemApServer> sap);
	
  // COSEM-GET service of xDLMS_ASE, implemented by the derived class
  void CosemXdlmsGet (int typeGet, int typeService, Ptr<CosemApServer> sap);
	
  // Received the indication/confirmation of a TCP-DATA resquest 
  void RecvCosemApduTcp (int tcpsService, Ptr<Packet> packet);

  // Received the indication/confirmation of a UDP-DATA resquest
  void RecvCosemApduUdp (Ptr<Packet> packet);

  // Set & GET the pointer to a CosemAlServer object
  void SetCosemApServer (Ptr<CosemApServer> cosemApServer);
  Ptr<CosemApServer> GetCosemApServer ();

  // Set & GET the pointer to a CosemWrapperServer object
  void SetCosemWrapperServer (Ptr<UdpCosemWrapperServer> cosemWrapperServer);
  Ptr<UdpCosemWrapperServer> GetCosemWrapperServer ();

  // Set & GET the Udp Port listening by the SAL
  void SetUdpport (uint16_t udpPort);
  uint16_t GetUdpport ();
	
  // Type of services
  enum typeService { REQUEST, INDICATION, RESPONSE, CONFIRM };
	
  // Type of COSEM-GET service
  enum typeGet { GET_NORMAL, GET_WITH_LIST, GET_ONE_BLOCK, GET_LAST_BLOCK };

  // TCP service
  enum tcpService { CONNECT, DATA, DISCONNECT, ABORT };	

  // ACSE services
  enum typeAcseService { OPEN, RELEASE };

protected:

  // A pointer to a CosemServer_AP object (cross-reference) 
  Ptr<CosemApServer> m_cosemApServer; 

  // A pointer to a Wrapper Server object
  Ptr<UdpCosemWrapperServer> m_cosemWrapperServer;

  uint16_t m_udpPort;  // Udp port

};

} // namespace ns3

#endif /* COSEM_AL_SERVER_H */
