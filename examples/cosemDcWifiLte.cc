/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 * Copyright (c) 2012 Uniandes 
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
 * Author: Jaume Nin <jaume.nin@cttc.cat>
 *         Juanmalk <jm.aranda121@uniandes.edu.co> 
 */

#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
//#include "ns3/gtk-config-store.h"
#include "ns3/udp-cosem-client-helper.h"
#include "ns3/udp-cosem-server-helper.h"
#include "ns3/data-concentrator-helper.h"
#include "ns3/demand-response-application-helper.h"

// Default Network Topology
//
//      Wifi 10.1.3.0
//                          AP(Wifi)
//  *      *         *       *
//  |      |         |       |    7.0.0.0                                   1.0.0.0
// SM(1)  SM(0) ... SM(n)   DC ------------- eNB ================= PGW  =================  CC
//                           |   LTE channel (1)      EPC Network  (1)       Internet      (2)
//                           x                                           
//                          UE(LTE)

using namespace ns3;

/**
 * Sample simulation script for LTE+EPC. It instantiates several eNodeB,
 * attaches one UE per eNodeB starts a flow for each UE to  and from a remote host.
 * It also  starts yet another flow between each UE pair.
 */

NS_LOG_COMPONENT_DEFINE ("CosemDCWifiLteExample");

int
main (int argc, char *argv[])
{

  bool verbose = true;  // For COSEM
  uint16_t numberOfNodes = 2;  // LTE nodes
  uint32_t nWifi = 3; // Smart Meters
  double simTime = 30.0;
  double distance = 60.0;
  double interPacketInterval = 100;

  // Command line arguments
  CommandLine cmd;
  cmd.AddValue("numberOfNodes", "Number of eNodeBs + UE pairs", numberOfNodes);
  cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
  cmd.AddValue("distance", "Distance between eNBs [m]", distance);
  cmd.AddValue("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
  cmd.Parse(argc, argv);

  // Wifi
  if (nWifi > 18)
    {
      std::cout << "Number of wifi nodes " << nWifi << 
                   " specified exceeds the mobility bounding box" << std::endl;
      exit (1);
    }

  // For COSEM
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
    }
  // 
  
// -------------------------------------------------------------------------------
//  LTE + EPC CONFIGURATION
// -------------------------------------------------------------------------------

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<EpcHelper>  epcHelper = CreateObject<EpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults();

  // parse again so you can override default values from the command line
  cmd.Parse(argc, argv);

  Ptr<Node> pgw = epcHelper->GetPgwNode ();

   // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  // interface 0 is localhost, 1 is the p2p device
  // Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);
  
  // For COSEM
  //Ipv4InterfaceContainer controlCenterInterfaces;
  //controlCenterInterfaces.Add (ipv4h.Assign (internetDevices.Get (1)));
  //

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create(numberOfNodes);
  ueNodes.Create(numberOfNodes);

  // Install Mobility Model
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 0; i < numberOfNodes; i++)
    {
      positionAlloc->Add (Vector(distance * i, 0, 0));
    }
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(positionAlloc);
  mobility.Install(enbNodes);
  mobility.Install(ueNodes);

  // Install LTE Devices to the nodes
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
  NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);

  // Attach one UE per eNodeB
  for (uint16_t i = 0; i < numberOfNodes; i++)
      {
        lteHelper->Attach (ueLteDevs.Get(i), enbLteDevs.Get(i));
      }

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIface;
  ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));

  // Assign IP address to UEs, and install applications
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }
  lteHelper->ActivateEpsBearer (ueLteDevs, EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT), EpcTft::Default ());

  lteHelper->EnableTraces ();
  // Uncomment to enable PCAP tracing
  //p2ph.EnablePcapAll("lena-epc-first");


// -------------------------------------------------------------------------------
//  WIFI CONFIGURATION
// -------------------------------------------------------------------------------
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode = ueNodes.Get (0); // UE Node

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

  MobilityHelper mobilityWifi;

  mobilityWifi.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  //mobilityWifi.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
  //                           "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobilityWifi.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityWifi.Install (wifiStaNodes);

  mobilityWifi.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityWifi.Install (wifiApNode);

  //InternetStackHelper stack;
  //internet.Install (wifiApNode);
  internet.Install (wifiStaNodes);

  Ipv4AddressHelper address;
  Ipv4InterfaceContainer serverInterfaces;

  address.SetBase ("10.1.3.0", "255.255.255.0");
  // Ipv4InterfaceContainer wifiInterfaces;
  // address.Assign (staDevices);
 
  for (uint32_t i = 0; i < wifiStaNodes.GetN (); ++i)
     {
        serverInterfaces.Add (address.Assign (staDevices.Get (i)));
     }

  //Ipv4InterfaceContainer apInterfaces;
  //apInterfaces.Add (address.Assign (apDevices));

  Ipv4InterfaceContainer dcWifiInterfaces;
  dcWifiInterfaces.Add (address.Assign (apDevices));
 
// -------------------------------------------------------------------------------
//  COSEM & DATA CONCENTRATOR CONFIGURATION
// -------------------------------------------------------------------------------
 
 // Cosem Application

  UdpCosemServerHelper cosemServer (serverInterfaces);

  ApplicationContainer serverApps = cosemServer.Install (wifiStaNodes);
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (29.0));

  UdpCosemClientHelper cosemClient (serverApps, dcWifiInterfaces, Seconds (2.0)); 

  ApplicationContainer clientApps = cosemClient.Install (ueNodes.Get (0));
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (29.0));


  // Data Concentrator Application
  DataConcentratorApplicationHelper dc (clientApps, internetIpIfaces.GetAddress (1), ueIpIface.GetAddress (0)); 
  ApplicationContainer dcApps = dc.Install (ueNodes.Get (0));
  dcApps.Start (Seconds (1.001));
  dcApps.Stop (Seconds (29.0));

// -------------------------------------------------------------------------------
//  DEMAND RESPONSE APPLICATION CONFIGURATION (ON REMOT HOST)
// -------------------------------------------------------------------------------

  // Demand Response Application on Control Center (request data to Data Concentrator every 3 min)
  DemandResponseApplicationHelper drHelper (dcApps, internetIpIfaces.GetAddress (1), Seconds (5.0), 2.0);
  ApplicationContainer drApps = drHelper.Install (remoteHost);
  drApps.Start (Seconds (0.0));
  drApps.Stop (Seconds (29.0));

//  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
 
  Simulator::Stop(Seconds(simTime));

  phy.EnablePcap ("third", apDevices.Get (0));

  Simulator::Run();

  /*GtkConfigStore config;
  config.ConfigureAttributes();*/

  Simulator::Destroy();
  return 0;

}

