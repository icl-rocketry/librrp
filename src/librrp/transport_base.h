#pragma once
#include <cstdint>
#include <vector>
#include <cstddef>


class TransportBase
{
    public:
        virtual bool setup();
        virtual size_t sendPacket(std::vector<uint8_t> buf);
        virtual size_t readPacket(std::vector<uint8_t>& buf);
        virtual bool isBusy();
};
