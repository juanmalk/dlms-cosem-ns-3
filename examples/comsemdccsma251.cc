/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/udp-cosem-client-helper.h"
#include "ns3/udp-cosem-server-helper.h"
#include "ns3/data-concentrator-helper.h"
#include "ns3/demand-response-application-helper.h"

// Default Network Topology
//
// (SG)   10.1.1.0   (DC) (SM1) (SM2) ... (SM252)
//  n0 -------------- n1   n2   n3    ...  n252
//       fiber (p2p)   |    |    |          |
//                     ====================== (CSMA Channel)
//                         LAN 10.1.2.0


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CosemCsmadcExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 251;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {      
      LogComponentEnable ("CosemApplicationLayerClient", LOG_LEVEL_INFO);
      LogComponentEnable ("CosemApplicationLayerServer", LOG_LEVEL_INFO);
      LogComponentEnable ("CosemApplicationsProcessClient", LOG_LEVEL_INFO);
      LogComponentEnable ("CosemApplicationsProcessServer", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpCosemWrapperLayerClient", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpCosemWrapperLayerServer", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpCosemServerHelper", LOG_LEVEL_INFO);
      LogComponentEnable ("DataConcentratorApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("DemandResponseApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("DataConcentratorApplicationHelper", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpCosemClientHelper", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma);
 
  PointToPointHelper pointToPoint; // fiber
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("622Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (p2pNodes.Get (0));
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  Ipv4InterfaceContainer dcCsmaInterfaces;
  dcCsmaInterfaces.Add (address.Assign (csmaDevices.Get (0)));

  for (uint32_t i = 1; i < csmaNodes.GetN (); ++i)
     {
        csmaInterfaces.Add (address.Assign (csmaDevices.Get (i)));
     }

  //csmaInterfaces = address.Assign (csmaDevices);

  // Cosem & DataConcentrator & RequestingSG Applications

  // Cosem Applications Servers 
  UdpCosemServerHelper cosemServer (csmaInterfaces);
  ApplicationContainer serverApps;
  for (uint32_t i = 1; i < csmaNodes.GetN (); ++i)
    {
       serverApps.Add (cosemServer.Install (csmaNodes.Get (i)));
    }

  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (100.0));

  // Cosem Application Clients  
  // (request data to Smart Meters every 1 min + delay of polling all smart meters)
  UdpCosemClientHelper cosemClient (serverApps, dcCsmaInterfaces, Seconds (60.0)); 
  ApplicationContainer clientApps = cosemClient.Install (p2pNodes.Get (1));
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (900.0));

  // Data Concentrator Application
  DataConcentratorApplicationHelper dc (clientApps, p2pInterfaces.GetAddress (0), p2pInterfaces.GetAddress (1)); 
  ApplicationContainer dcApps = dc.Install (p2pNodes.Get (1));
  dcApps.Start (Seconds (1.001));
  dcApps.Stop (Seconds (900.0));

  // Demand Response Application on Control Center (request data to Data Concentrator every 3 min)
  DemandResponseApplicationHelper drHelper (dcApps, p2pInterfaces.GetAddress (0), Seconds (180.0), 60.0);
  ApplicationContainer drApps = drHelper.Install (p2pNodes.Get (0));
  drApps.Start (Seconds (0.0));
  drApps.Stop (Seconds (900.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
  Simulator::Stop (Seconds (950.0));
  
  pointToPoint.EnablePcapAll ("second");
  csma.EnablePcap ("second", csmaDevices.Get (1), true);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
