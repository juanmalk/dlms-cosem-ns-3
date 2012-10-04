/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-al-client.h
 * Implementation of the Class CosemAlClient
 * Class that represents the COSEM Application Layer - Client side
 * Created on: 26-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#ifndef COSEM_AL_CLIENT_H
#define COSEM_AL_CLIENT_H

#include <map> 
#include "cosem-al.h"
#include "ns3/ipv4-address.h"
#include "ns3/simulator.h"

namespace ns3 {

class Packet;
class CosemApServer;
class CosemApClient;
class UdpCosemWrapperClient;

//class CosemWrapperClient;	

/** 
 * COSEM Application Layer (AL) Control Function - Client
 *
 */

class CosemAlClient : public CosemAl
{
public:
  static TypeId GetTypeId (void);
  
  CosemAlClient ();
 
  virtual ~CosemAlClient();

  // COSEM-OPEN service of ACSE, implemented by the derived class
  void CosemAcseOpen (int typeService, Ptr<CosemApServer> sap);
	
  // COSEM-RELEASE service of ACSE, implemented by the derived class
  void CosemAcseRelease (int typeService, Ptr<CosemApServer> sap);
	
  // COSEM-GET service of xDLMS_ASE, implemented by the derived class
  void CosemXdlmsGet (int typeGet, int typeService, Ptr<CosemApServer> sap);

  // Construct the APDUs of ACSE services (AARQ, AARE, RLRQ, RLRE)
  virtual void CosemAcseApdu (int typeAcseService, int typeService, Ptr<CosemApServer> sap);
	
  // Construct the APDUs of xDLMS_ASE services(GET-REQUEST, GET-RESPONSE)
  virtual void CosemXdlmsApdu (int typeGet, int typeService, Ptr<CosemApServer> sap);
	
  // Received the indication/confirmation of a TCP-DATA resquest
  void RecvCosemApduTcp (int tcpsService, Ptr<Packet> packet);

  // Received the indication/confirmation of a UDP-DATA resquest
  void RecvCosemApduUdp (Ptr<Packet> packet);

  // Set & GET the pointer to a CosemAlServer object
  void SetCosemApClient (Ptr<CosemApClient> cosemApClient);
  Ptr<CosemApClient> GetCosemApClient ();
 
  // Set & GET the pointer to a CosemWrapperClient object
  void SetCosemWrapperClient (Ptr<UdpCosemWrapperClient> cosemWrapperClient);
  Ptr<UdpCosemWrapperClient> GetCosemWrapperClient ();

  // Set & GET the Udp Port listening by the CAL
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
  Ptr<CosemApClient> m_cosemApClient; 

  // A pointer to a Wrapper Client object
  Ptr<UdpCosemWrapperClient> m_cosemWrapperClient;

  uint16_t m_udpPort;  // Udp port

};

} // namespace ns3

#endif /* COSEM_AL_CLIENT_H */
