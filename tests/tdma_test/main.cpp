
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>

#include <iomanip>
#include <bitset>
#include <array>

#include <fstream>

#include <librnp/rnp_networkmanager.h>
#include "sim.h"
#include "tdma.h"


int main()
{
    std::deque<std::vector<uint8_t>> exchangeDeque;
    std::atomic<uint8_t> counter = 0;

    uint8_t numNodes = 1;
    std::vector<Sim> simTransports;
    std::vector<TDMA> tdmaNodes;

    for (int i = 0; i < numNodes; ++i) {
        simTransports.emplace_back(&exchangeDeque, &counter, numNodes);
        tdmaNodes.emplace_back(&simTransports[i], systemstatus, networkmanager, true);
    }

    for (auto& node : tdmaNodes) {
        node.setup()
    }

    for (;;) {
        node.update();
    }

    return 0;
}