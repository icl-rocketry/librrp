#pragma once
#include <cstdint>
#include <vector>
#include <cstddef>


struct PhysicalLayerInfo {
	uint32_t timeLastPacketReceived;

    virtual ~PhysicalLayerInfo(){};
};

class PhysicalLayerBase
{
    public:
        PhysicalLayerBase() = default;
        virtual ~PhysicalLayerBase() = default;

        virtual bool setup() = 0;
        virtual size_t sendPacket(std::vector<uint8_t> data) = 0;
        virtual size_t readPacket(std::vector<uint8_t>& data) = 0;
        virtual bool isBusy() = 0;
        virtual void restart() = 0;
		virtual const PhysicalLayerInfo* getInfo() = 0;
};
