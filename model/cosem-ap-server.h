/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-ap-server.h
 * Implementation of the Class CosemApServer
 * Class that represents the COSEM Server Application Process
 * Created on: 26-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#ifndef COSEM_AP_SERVER_H
#define COSEM_AL_SERVER_H

#include "ns3/application.h"
#include "ns3/simulator.h"

namespace ns3 {

class CosemAlServer;

/** 
 * COSEM Application Process (AP) - Server
 *
 */

class CosemApServer : public Application
{
public:
  static TypeId GetTypeId (void);
  
  CosemApServer ();
 
  virtual ~CosemApServer ();

  // Called when new packet received
  void Recv (int nbytes);

  //Set & GET the pointer to a CosemServer_AL_CF object
  void SetCosemAlServer (Ptr<CosemAlServer> cosemAlServer);
  Ptr<CosemAlServer> GetCosemAlServer ();

  // Set & GET the pointer to a CosemAlClient object
  void SetWport (uint16_t wPort);
  uint16_t GetWport ();

  // Set & GET the Udp Port listening by the CAL
  void SetUdpport (uint16_t udpPort);
  uint16_t GetUdpport ();

  // Set & GET the local Ip address
  void SetLocalAddress (Address ip);
  Address GetLocalAddress ();

  // Type of services
  enum typeService { REQUEST, INDICATION, RESPONSE, CONFIRM };

  // Type of COSEM-GET service
  enum typeGet { GET_NORMAL, GET_WITH_LIST, GET_ONE_BLOCK, GET_LAST_BLOCK }; 
	
protected:

  virtual void DoDispose (void);

  Ptr<CosemAlServer> m_cosemAlServer;

  uint16_t m_wPort;  // SAP Wrapper Port Number (unique id)

  uint16_t m_udpPort;  // Udp port

  Address m_localAddress;  // Local Ip address
 
private:

  virtual void StartApplication (void);

  virtual void StopApplication (void);

};

} // namespace ns3

#endif /* COSEM_AP_SERVER_H */
