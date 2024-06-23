#pragma once
#include "transport_base.h"

#include <deque>
#include <memory>
#include <queue>
#include <mutex>
#include <atomic>

class SimTransport : public TransportBase {

    public:
        SimTransport() = default;
        ~SimTransport() override = default;

        bool setup() override;
        size_t sendPacket(std::vector<uint8_t> buf) override;
        size_t readPacket(std::vector<uint8_t>& buf) override;
        bool isBusy() override;

        void setNodesList(std::vector<SimTransport*> nodes);
        void pushToRxBuffer(std::vector<uint8_t> buf);          

    private:

        std::queue<std::vector<uint8_t>> rxBuffer;
        std::vector<SimTransport*> _nodes;
        std::mutex _mtx;
};