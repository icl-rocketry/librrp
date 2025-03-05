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


class SimPhysicalLayerBase : public PhysicalLayerBase {

    public:
        explicit SimPhysicalLayerBase(std::shared_ptr<std::mutex> mtx) : m_mtx(mtx) {}        
        ~SimPhysicalLayerBase() override = default;
        bool setup() override;
        size_t sendPacket(std::vector<uint8_t> data) override;
        size_t readPacket(std::vector<uint8_t>& data) override;
        bool isBusy() override;
        void restart() override;
		const PhysicalLayerInfo* getInfo() override {return &m_info;}
        void setNodesList(std::vector<SimPhysicalLayerBase*> nodes);
        void pushToRxBuffer(std::vector<uint8_t> data);

    protected:

        std::queue<std::vector<uint8_t>> rxBuffer;
        std::vector<SimPhysicalLayerBase*> m_nodes;
        std::shared_ptr<std::mutex> m_mtx;
		PhysicalLayerInfo m_info;
};