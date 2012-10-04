/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * udp-cosem-client.h
 * Implementation of the Class UdpCosemWrapperClient
 * Class that represents the UDP COSEM Wrapper transport sub-layer - Client side
 * Created on: 28-Sept-2012
 * Modified on:
 * Original author: JUANMALK
 */

#ifndef UDP_COSEM_CLIENT_H
#define UDP_COSEM_CLIENT_H

#include "ns3/object.h"
#include "ns3/simulator.h"

namespace ns3 {

class Socket;
class Packet;
class CosemApServer;
class CosemAlClient;
class CosemApClient;

/** 
 * UDP COSEM Wrapper Transport sub-layer - Client
 *
 */

class UdpCosemWrapperClient : public Object
{
public:
  static TypeId GetTypeId (void);
  
  UdpCosemWrapperClient ();
 
  virtual ~UdpCosemWrapperClient ();

  // Recive ("read") the data from the socket and pass to the Cosem Application Layer
  void Recv (Ptr<Socket> socket);
	
  // Called when new packet is ready to be send
  void Send (Ptr<Packet> packet, Ptr<CosemApServer> cosemApServer);

  // Set & GET the pointer to a CosemAlClient object
  void SetCosemAlClient (Ptr<CosemAlClient> cosemAlClient);
  Ptr<CosemAlClient> GetCosemAlClient ();

  // Set & GET the pointer to a Socket object
  void SetSocket (Ptr<Socket> socket);
  Ptr<Socket> GetSocket ();

  // Assign a wPort number to the CAP created and attached to the CAL
  void SetwPortClient (Ptr<CosemApClient> cosemApClient);

  // Return the wPort number of the CAP connected to the CAL
  uint16_t GetwPortClient ();

  // Set & GET the Udp Port listening by the CAL
  void SetUdpport (uint16_t udpPort);
  uint16_t GetUdpport ();

  // Set & GET the local Ip address (Cap)
  void SetLocalAddress (Address ip);
  Address GetLocalAddress ();

  // Set the remote ip address (Sap)
  void SetRemoteAddress (Address ip); 
  Address GetRemoteAddress ();

  // Type of services
  enum typeService { REQUEST, INDICATION, CONFIRM };
  
private:

  Ptr<Socket> m_socket;

  Ptr<CosemAlClient> m_cosemAlClient; 

  uint16_t m_wPortCap;  // Wrapper Port Number assigned to the CAP

  uint16_t m_udpPort;  // Udp port

  Address m_localAddress;  // Local Ip address 

  Address m_remoteAddress;  // Ip Address of the remote SAP

};

} // namespace ns3

#endif /* UDP_COSEM_CLIENT_H */
