
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>

#include <iomanip>
#include <bitset>
#include <array>

#include <fstream>

#include <librnp/rnp_networkmanager.h>
#include "src/librrp/sim_transport.h"
#include "src/librrp/tdma.h"



int main()
{
    uint8_t numNodes = 1;

    SimTransport transport1;
    // RnpNetworkManager networkmanager1(0,NODETYPE::LEAF,true,200);
    // TDMA<SimTransport> TDMAnode1(transport1, networkmanager1, 2, "TDMA", true);
    // TDMAnode1.setup();

    return 0;
}