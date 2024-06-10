#pragma once
#include "transport_base.h"

#include <deque>
#include <memory>
#
#include <mutex>
#include <atomic>

class Sim : Transport_Base {

    public:
        Sim(std::deque<std::vector<uint8_t>>* dequePtr, std::atomic<uint8_t>* counterPtr, uint8_t numNodes):
            _dequePtr(dequePtr),
            _counterPtr(counterPtr),
            _numNodes(numNodes)
        {};

        bool setup() override;
        size_t sendPacket(std::vector<uint8_t> buf) override;
        size_t readPacket(std::vector<uint8_t>& buf) override;
        bool isBusy() override;

    private:
        std::deque<std::vector<uint8_t>>* _dequePtr;
        std::mutex _mtx;
        std::atomic<uint8_t>* _counterPtr;
        uint8_t _numNodes;
};