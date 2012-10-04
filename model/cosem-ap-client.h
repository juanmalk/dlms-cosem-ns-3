/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * cosem-ap-client.h
 * Implementation of the Class CosemApClient
 * Class that represents the COSEM Client Application Process
 * Created on: 26-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#ifndef COSEM_AP_CLIENT_H
#define COSEM_AL_CLIENT_H

#include <map>
#include "ns3/application.h"
#include "ns3/ipv4-address.h"
#include "ns3/application-container.h"
#include "ns3/simulator.h"

namespace ns3 {

class CosemAlClient;
class CosemApServer;

/** 
 * COSEM Application Process (AP) - Client
 *
 */

class CosemApClient : public Application
{
public:
  static TypeId GetTypeId (void);
  
  CosemApClient ();
 
  virtual ~CosemApClient ();

  // Called when new packet received
  void Recv (int nbytes, int typeAcseService, int typeGet, int typeService);

  // Start the request of data to the SAP by the CAP
  void StartRequest ();

  // New request of data to the SAP by the CAP
  void NewRequest ();

  //Request the Release of AAs
  void RequestRelease ();

  // Compute and return the next time that the Client(DC) requests to the Servers (SmartMeter,SM)
  double NextTimeRequestSm ();

  // Store the AAs succesfully established
  void SaveActiveAa (uint16_t dstWport, Ptr<CosemApServer> sap);
	
  // Remove the AAs succesfully established before
  void RemoveActiveAa (Ptr<CosemApServer> sap);

  // Return the AAs succesfully established before
  Ptr<CosemApServer> ReturnActiveAa (uint16_t dstWport);

  // Set & GET the pointer to a CosemAlClient object
  void SetCosemAlClient (Ptr<CosemAlClient> cosemAlClient);
  Ptr<CosemAlClient> GetCosemAlClient ();

  // Set & GET the wPort
  void SetWport (uint16_t wPort);
  uint16_t GetWport ();

  // Set & GET the Udp Port listening by the CAL
  void SetUdpport (uint16_t udpPort);
  uint16_t GetUdpport ();

  // Set & GET the local Ip address (Cap)
  void SetLocalAddress (Address ip);
  Address GetLocalAddress ();

  // Retrieve a reference of ApplicationContainer object (Sap)
  void SetApplicationContainerSap (ApplicationContainer containerSap);

  // Set & Get the type of requesting mechanism
  void SetTypeRequesting (bool typeRequesting);
  bool GetTypeRequesting ();

  // Type of services
  enum typeService { REQUEST, INDICATION, RESPONSE, CONFIRM };

  // Type of COSEM-GET service
  enum typeGet { GET_NORMAL, GET_WITH_LIST, GET_ONE_BLOCK, GET_LAST_BLOCK };

  // ACSE services
  enum typeAcseService { OPEN, RELEASE }; 
	
protected:

  virtual void DoDispose (void);
 
private:

  Ptr<CosemAlClient> m_cosemAlClient;
	
  uint16_t m_wPort;  // CAP Wrapper Port Number (unique id)

  uint16_t m_udpPort;  // Udp port

  Address m_localAddress;  // Local Ip address 

  ApplicationContainer m_containerSap; // Container of Sap in the scenario

  bool m_typeRequesting;  // Type Requesting mechanism: TRUE = MULTICASTING (simultaneous); FALSE = SEQUENCIAL (Round Robin style)

  // Map container to store the AAs succesfully established 
  std::map<uint16_t, Ptr<CosemApServer> > m_activeAa;	
  std::map<uint16_t, Ptr<CosemApServer> >::iterator m_it;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 virtual void StartApplication (void);

  virtual void StopApplication (void);

  // Helpers parameters
  EventId m_startRequestEvent;
  std::vector<Ptr<Application> >::const_iterator m_itSap; // Iterator AppContainer
  uint32_t m_nSap;  // Number of Saps          

};

} // namespace ns3

#endif /* COSEM_AP_CLIENT_H */
