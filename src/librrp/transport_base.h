#include <cstdint>
#include <vector>

#pragma once

class Transport_Base
{
    public:
        bool setup();
        size_t sendPacket(std::vector<uint8_t> buf);
        std::vector<uint8_t> readPacket();
        size_t readPacket(std::vector<uint8_t>& buf);
        bool isBusy();
};
