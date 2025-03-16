#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <functional>

// librrp
#include <librrp/physical/lora_sim_physical_layer.h>
#include <librrp/datalink/tdma.h>

// librnp
#include <librnp/rnp_networkmanager.h>
#include <librnp/default_packets/simplecommandpacket.h>

// libriccore
#include <libriccore/platform/millis.h>
#include <libriccore/platform/riccorethread_types.h>

#include "../SimNode.h"

void runNode(SimNode<TDMARadio<LoRaSimPhysicalLayer>>* simNode, int nodeNum) {
	int driftPPM = (nodeNum == 0) ? -1000 : 1000;
	setClockDriftPPM(driftPPM);
	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10000));
    for (;;) {
        simNode->update();
    }
}

int main()
{
    int numNodes = 2;

   // create nodes and physical layers
   std::vector<std::unique_ptr<SimNode<TDMARadio<LoRaSimPhysicalLayer>>>> simNodes;
   std::vector<SimPhysicalLayerBase*> physicalNodesList;
   std::vector<std::unique_ptr<RnpNetworkManager>> networkManagers;

   // mutex shared between all nodes
   std::shared_ptr<std::mutex> mtx = std::make_shared<std::mutex>();

   // LoRa params
   float freq = 868e6;
   float bw = 250e3;
   float sf = 7;

   for (int i = 0; i < numNodes; ++i) {
		networkManagers.emplace_back(std::make_unique<RnpNetworkManager>(100 + i));

		// create and initialize a new node
		auto simNode = std::make_unique<SimNode<TDMARadio<LoRaSimPhysicalLayer>>>(*networkManagers[i], mtx, freq, bw, sf, false);
		simNode->setup();

		// add LoRaSimPhysicalLayer instance to the physical nodes list
		physicalNodesList.push_back(static_cast<LoRaSimPhysicalLayer*>(simNode->getPhysicalLayer()));

		// add the node to the list of simulated data link layer nodes
		simNodes.push_back(std::move(simNode));
   }

   // set the physical nodes list for each physical node
   for (int i = 0; i < numNodes; ++i) {
	   simNodes[i]->getPhysicalLayer()->setNodesList(physicalNodesList);
   }

   // create threads for each node
   std::vector<std::thread> threads;
   for (int i = 0; i < numNodes; i++){	//auto& node : simNodes) {
	   threads.emplace_back(runNode, simNodes[i].get(), i);
   }

   for (auto& thread : threads) {
	   if (thread.joinable()) {
		   thread.join();
	   }
   }
	
    return 0;
}