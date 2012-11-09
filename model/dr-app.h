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
class Time;


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

  // Build command message to be sent by the Control Center to AMI netowrk's Data Concentrator 
  void Command (uint8_t messageType, uint32_t command, Address currentDcAddress);

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

  // Type of Demand Response Messages 
  enum MessageType { CONTROL, PRICE }; 

protected:

  virtual void DoDispose (void);
 
private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  Ptr<Socket> m_socket;
  ApplicationContainer m_containerDcApp; // Container of DcApplication in the scenario
  Address m_localAddress;  // Local Ip address 
  Time m_nextTimeRequest; // Next time request of data to AMI netowrk's Data Concentrator 

  // Helpers parameters
  EventId m_sendEvent;
  EventId m_requestEvent;
  EventId m_commandEvent;
  EventId m_demandResponseMechanismEvent;
};

} // namespace ns3

#endif /* DR_APP_H */
