/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-al-server.h
 * Implementation of the Class CosemAlServer
 * Class that represents the COSEM Application Layer - Server side
 * Created on: 26-Sept-2012
 * Modified on:
 * Original author: JUANMALK
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
