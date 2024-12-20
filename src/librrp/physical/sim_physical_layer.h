#pragma once
#include "physical_layer_base.h"

// std
#include <deque>
#include <memory>
#include <queue>
#include <mutex>
#include <atomic>

// ric
#include <libriccore/riccorelogging.h>


class SimPhysicalLayer : public PhysicalLayerBase {

    public:
        SimPhysicalLayer() = default;
        ~SimPhysicalLayer() override = default;

        bool setup() override;
        size_t sendPacket(std::vector<uint8_t> data) override;
        size_t readPacket(std::vector<uint8_t>& data) override;
        bool isBusy() override;
        void restart() override;

        void setNodesList(std::vector<SimPhysicalLayer*> nodes);
        void pushToRxBuffer(std::vector<uint8_t> data);          

    private:

        std::queue<std::vector<uint8_t>> rxBuffer;
        std::vector<SimPhysicalLayer*> m_nodes;
        std::mutex m_mtx;
};