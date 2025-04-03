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

int numNodes = 2;
std::mutex nodeMutex;
std::vector<std::unique_ptr<SimNode<TDMARadio<LoRaSimPhysicalLayer>>>> simNodes(numNodes);
std::vector<std::atomic<bool>> nodeRunning(numNodes);
std::vector<std::thread> nodeThreads(numNodes);


void runNode(SimNode<TDMARadio<LoRaSimPhysicalLayer>>* simNode, int nodeNum) {
	int driftPPM = (nodeNum == 0) ? -10 : 10;
	setClockDriftPPM(driftPPM);

    while (nodeRunning[nodeNum].load()) {  // Check if the thread should continue running
        simNode->update();  // Perform node updates
        std::this_thread::sleep_for(std::chrono::milliseconds(2));  // 500Hz loop speed
    }
}

void spawnNode(int nodeNum, float freq, float bw, uint8_t sf) {
    auto simNode = std::make_unique<SimNode<TDMARadio<LoRaSimPhysicalLayer>>>(nodeNum, freq, bw, sf, true);
    simNode->setup();

	{
		std::lock_guard<std::mutex> lock(nodeMutex);
		simNodes[nodeNum] = std::move(simNode);
		nodeRunning[nodeNum].store(true);	// set bool for the update loop in runNode to start running
	}

	nodeThreads[nodeNum] = std::thread(runNode, simNodes[nodeNum].get(), nodeNum);
}

void despawnNode(int nodeNum){
	nodeRunning[nodeNum].store(false);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	if (nodeThreads[nodeNum].joinable()) {
		nodeThreads[nodeNum].join();
	}	

	{
		std::lock_guard<std::mutex> lock(nodeMutex);
		simNodes[nodeNum] = nullptr;
	}
}

void spawnManager(){

	// LoRa params
	float freq = 868e6;
	float bw = 250e3;
	uint8_t sf = 7;

	spawnNode(0, freq, bw, sf);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	spawnNode(1, freq, bw, sf);

	std::this_thread::sleep_for(std::chrono::seconds(15));
	despawnNode(0);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	spawnNode(0, freq, bw, sf);

	std::this_thread::sleep_for(std::chrono::seconds(15));
	despawnNode(0);
	despawnNode(1);

}

int main()
{
    // Start node manager thread
    std::thread spawnManagerThread(spawnManager);

    if (spawnManagerThread.joinable()) {
        spawnManagerThread.join();
    }
	
    return 0;
}