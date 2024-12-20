#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

// librrp
#include <librrp/physical/sim_physical_layer.h>
#include <librrp/datalink/turn_timeout.h>

// librnp
#include <librnp/rnp_networkmanager.h>
#include <librnp/default_packets/simplecommandpacket.h>

// libriccore
#include <libriccore/platform/millis.h>
#include <libriccore/platform/riccorethread_types.h>

#include "SimNode.h"

int main(int argc, char* argv[]) {

    // default number of sim nodes
    int numNodes = 2;

    // command line argument handling
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

    std::vector<std::unique_ptr<SimNode<TimeoutRadio<SimPhysicalLayer>>>> simNodes;
    std::vector<SimPhysicalLayer*> physicalNodesList;   // this is for the nodes to be able to push to each others physical layer rx buffers

    for (int i = 0; i < numNodes; ++i) {
        simNodes.push_back(std::make_unique<SimNode<TimeoutRadio<SimPhysicalLayer>>>());
        simNodes.back()->setup();
        physicalNodesList.push_back(simNodes.back()->getPhysicalLayer()); 
        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%1000));
    }

    for (int i = 0; i < numNodes; ++i){
        simNodes[i]->getPhysicalLayer()->setNodesList(physicalNodesList);
    }

    // main sim loop
    for (;;) {
        for (auto& node : simNodes) {
            node->update();
        }
    }

    return 0;
}
