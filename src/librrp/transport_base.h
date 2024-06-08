#include <cstdint>
#include <vector>

#pragma once

class Transport_Base
{
    public:
        virtual bool setup();
        virtual size_t sendPacket(std::vector<uint8_t> buf);
        virtual std::vector<uint8_t> readPacket();
        virtual size_t readPacket(std::vector<uint8_t>& buf);
        virtual bool isBusy();
};
