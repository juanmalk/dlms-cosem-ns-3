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

#include "ns3/application-container.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/demand-response-application-helper.h"
#include "ns3/dr-app.h"

NS_LOG_COMPONENT_DEFINE ("DemandResponseApplicationHelper");

namespace ns3 {

DemandResponseApplicationHelper::DemandResponseApplicationHelper ()
{
}

DemandResponseApplicationHelper::DemandResponseApplicationHelper (ApplicationContainer dataConcentratorAppContainer, Address centerIp, Time interval, uint32_t readingTime)
{
  m_dataConcentratorApplicationContainer = dataConcentratorAppContainer;
  m_centerIpAddress = Address (centerIp);
  m_interval = interval;
  m_readingTime = readingTime;;
}

void
DemandResponseApplicationHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DemandResponseApplicationHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      // Retreive the pointer of the i-node storaged in the NodeContainer
      Ptr<Node> node = *i;  
     
      // Create a DemandResponseApplication Object
      Ptr<DemandResponseApplication> demandResponseApplication = CreateObject<DemandResponseApplication> ();
      // Set the SmatGrid Control Center Address
      demandResponseApplication->SetLocalAddress (m_centerIpAddress);
      // Retreive the pointer of ApplicationContainer
      demandResponseApplication->SetApplicationContainerDcApp (m_dataConcentratorApplicationContainer);
      // Set interval for requesting data to remote Data Doncentrator and the Reading time to poll the meter data 
      demandResponseApplication->SetNextTimeRequest (m_interval);
      demandResponseApplication->SetReadingTime (m_readingTime);
      // Add the DataConcentratorApplication created to the Node
      node->AddApplication (demandResponseApplication);
      // Add the DataConcentratorApplication created to the ApplicationContainer
      apps.Add (demandResponseApplication);   
    }
  return apps;
}

} // namespace ns3
