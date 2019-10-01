#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Second Script Example");

int main(int argc, char* argv[])
{
	int nCsma = 2;	
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
	NodeContainer p2pN;
	p2pN.Create(2);
	NodeContainer p2pN1;
	p2pN1.Create(2);
	NodeContainer csmaNodes;
	csmaNodes.Add(p2pN.Get(1));
	csmaNodes.Create(nCsma);
	csmaNodes.Add(p2pN1.Get(0));
	
	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
	p2p.SetChannelAttribute("Delay",StringValue("2ms"));
	CsmaHelper csma;
	csma.SetChannelAttribute("DataRate",StringValue("100Mbps"));
	csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));
	PointToPointHelper p2p1;
	p2p1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
	p2p1.SetChannelAttribute("Delay",StringValue("2ms"));
	
	NetDeviceContainer p2pDev;
	p2pDev = p2p.Install(p2pN);
	NetDeviceContainer csmaDev;
	csmaDev = csma.Install(csmaNodes);
	NetDeviceContainer p2pDev1;
	p2pDev1 = p2p1.Install(p2pN1);

	InternetStackHelper stack;
	stack.Install(p2pN.Get(0));
	stack.Install(csmaNodes);
	stack.Install(p2pN1.Get(1));

	Ipv4AddressHelper addr;
	addr.SetBase("10.1.1.0","255.255.255.0");
	Ipv4InterfaceContainer p2pIF;
	p2pIF = addr.Assign(p2pDev);
	addr.SetBase("10.1.2.0","255.255.255.0");
	Ipv4InterfaceContainer csmaIF;
	csmaIF = addr.Assign(csmaDev);
	addr.SetBase("10.1.3.0","255.255.255.0");
	Ipv4InterfaceContainer p2pIF1;
	p2pIF1 = addr.Assign(p2pDev1);
	
	UdpEchoServerHelper echoServer(9);
	ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));
	serverApps.Start(Seconds(1.0));
	serverApps.Stop(Seconds(10.0));
	UdpEchoServerHelper echoServer1(10);
	ApplicationContainer serverApps1 = echoServer1.Install(csmaNodes.Get(nCsma));
	serverApps1.Start(Seconds(1.0));
	serverApps1.Stop(Seconds(10.0));
	
	UdpEchoClientHelper echoClient(csmaIF.GetAddress(nCsma),9);
	echoClient.SetAttribute("MaxPackets",UintegerValue(1));
	echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
	echoClient.SetAttribute("PacketSize",UintegerValue(1024));
	ApplicationContainer clientApps = echoClient.Install(p2pN.Get(0));
	clientApps.Start(Seconds(2.0));
	clientApps.Stop(Seconds(10.0));
	
	UdpEchoClientHelper echoClient1(csmaIF.GetAddress(nCsma),10);
	echoClient1.SetAttribute("MaxPackets",UintegerValue(1));
	echoClient1.SetAttribute("Interval",TimeValue(Seconds(1.0)));
	echoClient1.SetAttribute("PacketSize",UintegerValue(512));
	ApplicationContainer clientApps1 = echoClient1.Install(p2pN1.Get(1));
	clientApps1.Start(Seconds(2.0));
	clientApps1.Stop(Seconds(10.0));
	
	Ipv4GlobalRoutingHelper::PopulateRoutingTables();
	p2p.EnablePcapAll("second");
	csma.EnablePcap("second",csmaDev.Get(1),true);
	p2p1.EnablePcapAll("second");
		
	Simulator::Run();
	Simulator::Destroy();
	return 0;
}
