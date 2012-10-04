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
