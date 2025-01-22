#include <iostream>
#include <sstream>
#include <cstring>
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

#include "SimNode.h"

// Function to run a node in a separate thread
void runNode(SimNode<TimeoutRadio<LoRaSimPhysicalLayer>>* simNode) {
    for (;;) {
        simNode->update();
    }
}

int main(int argc, char* argv[]) {
    // Default number of sim nodes
    int numNodes = 2;

    // Command line argument handling
    if (argc > 1) {
        try {
            numNodes = std::stoi(argv[1]);
            if (numNodes <= 0) {
                std::cerr << "Number of nodes must be greater than 0. Using default value (2)." << std::endl;
                numNodes = 2;
            }
        } catch (std::exception& e) {
            std::cerr << "Invalid argument for number of nodes. Using default value (2)." << std::endl;
            numNodes = 2;
        }
    }

    // Create nodes and physical layers
    std::vector<std::unique_ptr<SimNode<TimeoutRadio<LoRaSimPhysicalLayer>>>> simNodes;
    std::vector<SimPhysicalLayer*> physicalNodesList;   // Nodes for physical layer RX buffers

    // Mutex shared between all nodes
    std::shared_ptr<std::mutex> mtx = std::make_shared<std::mutex>();

    // LoRa params
    float freq = 868e6;
    float bw = 250e3;
    float sf = 7;

    for (int i = 0; i < numNodes; ++i) {
        // Create and initialize a new node
        auto simNode = std::make_unique<SimNode<TimeoutRadio<LoRaSimPhysicalLayer>>>(mtx, freq, bw, sf);
        simNode->setup();

        // Add the LoRaSimPhysicalLayer instance to the physical nodes list
        physicalNodesList.push_back(static_cast<SimPhysicalLayer*>(simNode->getPhysicalLayer()));

        // Add the node to the list of simulation nodes
        simNodes.push_back(std::move(simNode));

        // Delay for initialization randomness
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
    }

    // Set the nodes list for each physical layer
    for (int i = 0; i < numNodes; ++i) {
        simNodes[i]->getPhysicalLayer()->setNodesList(physicalNodesList);
    }

    // Create threads for each node
    std::vector<std::thread> threads;
    for (auto& node : simNodes) {
        threads.emplace_back(runNode, node.get());
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
