#pragma once
#include <cstdint>
#include <vector>
#include <cstddef>


class TransportBase
{
    public:
        TransportBase() = default;
        virtual ~TransportBase() = default;

        virtual bool setup() = 0;
        virtual size_t sendPacket(std::vector<uint8_t> buf) = 0;
        virtual size_t readPacket(std::vector<uint8_t>& buf) = 0;
        virtual bool isBusy() = 0;
};
