
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>

#include <iomanip>
#include <bitset>
#include <array>

#include <fstream>

// librrp
#include <librrp/sim_transport.h>
#include <librrp/tdma.h>

//librnp
#include <librnp/rnp_networkmanager.h>


int main()
{
    SimTransport transport1;
    RnpNetworkManager networkmanager;
    TDMA<SimTransport> node1(transport1, networkmanager);

    node1.setup();


    for (;;) {
        node1.update();
    }

    return 0;
}