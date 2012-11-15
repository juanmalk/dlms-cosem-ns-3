/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Uniandes (unregistered)
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

#ifndef DR_APP_H
#define DR_APP_H

#include <map>
#include "ns3/application.h"
#include "ns3/application-container.h"
#include "ns3/address.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"

namespace ns3 {

class Socket;
class Packet;


/** 
 * Application for Demand Response (DR): Send/Receive Demand Response/Requesting/Consumption messages
 *
 */

class DemandResponseApplication : public Application
{
public:
  static TypeId GetTypeId (void);
  
  DemandResponseApplication ();
 
  virtual ~DemandResponseApplication ();

  // Send requesting/demand response packet to AMI netowrk's Data Concentrator 
  void Send (Ptr<Packet> packet, Address currentDcAddress);
	
  // Receive data (consumption signal) from AMI netowrk's Data Concentrator 
  void Recv (Ptr<Socket> socket);

  // Request of data to AMI netowrk's Data Concentrator 
  void Request ();

  // Request the next block of data to AMI netowrk's Data Concentrator 
  void RequestNextBlock (uint16_t blockNumber);

  // Build command message to be sent by the Control Center to AMI netowrk's Data Concentrator 
  void Command (uint8_t signalType, uint32_t command, Address currentDcAddress, uint32_t customerId);

  // Call Demand Response Mechanism
  void DemandResponseMechanism (uint32_t data, Address currentDcAddres);

  // Retrieve a reference of Application Container (DataConcentratorApplication) object
  void SetApplicationContainerDcApp (ApplicationContainer containerDcApp);

  // Set & GET the local Ip address
  void SetLocalAddress (Address ip);
  Address GetLocalAddress ();
  
  // Set & Get Next Time Request AMI netowrk's Data Concentrator  
  void SetNextTimeRequest (Time nextTimeRequest);
  Time GetNextTimeRequest ();

  // Set & Get Reading time to poll the meter data 
  void SetReadingTime (uint32_t readingTime);
  uint32_t GetReadingTime ();

  // Type of Demand Response command signal
  enum SignalType { S_CONTROL, S_PRICE }; 

protected:

  virtual void DoDispose (void);
 
private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  Ptr<Socket> m_socket;
  ApplicationContainer m_containerDcApp; // Container of DcApplication in the scenario
  Address m_localAddress;  // Local Ip address 
  Time m_nextTimeRequest; // Next time request of data to AMI netowrk's Data Concentrator 
  uint32_t m_readingTime; // Reading time to poll the meter data 
  uint32_t m_meterData; // Meter Data: Input to Demand Response Engine
  uint32_t m_partialMeterData; // Parcial Meter Data (block)
  uint16_t m_length;   // Length of the complete meter data block received by the Control Center

  // Helpers parameters
  EventId m_sendEvent;
  EventId m_requestEvent;
  EventId m_requestNextBlockEvent;
  EventId m_commandEvent;
  EventId m_demandResponseMechanismEvent;
};

} // namespace ns3

#endif /* DR_APP_H */
