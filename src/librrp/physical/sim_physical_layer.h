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
        explicit SimPhysicalLayer(std::shared_ptr<std::mutex> mtx) : m_mtx(mtx) {}        
        ~SimPhysicalLayer() override = default;
        bool setup() override;
        size_t sendPacket(std::vector<uint8_t> data) override;
        size_t readPacket(std::vector<uint8_t>& data) override;
        bool isBusy() override;
        void restart() override;

        void setNodesList(std::vector<SimPhysicalLayer*> nodes);
        void pushToRxBuffer(std::vector<uint8_t> data);          

    protected:

        std::queue<std::vector<uint8_t>> rxBuffer;
        std::vector<SimPhysicalLayer*> m_nodes;
        std::shared_ptr<std::mutex> m_mtx;
};