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
#include <librrp/datalink/turn_timeout.h>

// librnp
#include <librnp/rnp_networkmanager.h>
#include <librnp/default_packets/simplecommandpacket.h>

// libriccore
#include <libriccore/platform/millis.h>
#include <libriccore/platform/riccorethread_types.h>

#include "../SimNode.h"


void runNode(SimNode<TimeoutRadio<LoRaSimPhysicalLayer>>* simNode, int nodeNum) {

	int driftPPM = (nodeNum == 0) ? -10 : 10;
	setClockDriftPPM(driftPPM);

	// std::this_thread::sleep_for(std::chrono::milliseconds(1000 + (100 * nodeNum)));

    for (;;) {
        simNode->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

int main(int argc, char* argv[]) {
    int numNodes = 2;

    std::vector<std::unique_ptr<SimNode<TimeoutRadio<LoRaSimPhysicalLayer>>>> simNodes;
	std::vector<std::unique_ptr<RnpNetworkManager>> networkManagers;

    // LoRa params
    float freq = 868e6;
    float bw = 250e3;
    uint8_t sf = 7;

    for (int i = 0; i < numNodes; ++i) {
    	networkManagers.emplace_back(std::make_unique<RnpNetworkManager>(100, NODETYPE::LEAF, true));	// the address gets reset in simNode.setup()

    	auto simNode = std::make_unique<SimNode<TimeoutRadio<LoRaSimPhysicalLayer>>>(*networkManagers[i], freq, bw, sf, true);
        simNode->setup();

        simNodes.push_back(std::move(simNode));
    }

    // create threads for each node
    std::vector<std::thread> threads;
    for (int i = 0; i < numNodes; i++){
        threads.emplace_back(runNode, simNodes[i].get(), i);
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
