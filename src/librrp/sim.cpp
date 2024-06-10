#include "sim.h"

bool Sim::setup(){
    if (_dequePtr != nullptr && _counterPtr != nullptr){
        return true;
    }
    else{
        return false;
    }
}

size_t Sim::sendPacket(std::vector<uint8_t> buf){
    std::lock_guard<std::mutex> lock(_mtx);
    _dequePtr->push_front(buf);
    return _dequePtr->front().size();
}

size_t Sim::readPacket(std::vector<uint8_t>& buf){
    std::lock_guard<std::mutex> lock(_mtx);

    buf = _dequePtr->back();
    size_t size = buf.size();
    if (size > 0){
        *(_counterPtr)++;

        if (*(_counterPtr) == _numNodes){
            _dequePtr->pop_back();
            *(_counterPtr) = 0;
        }

        return size;
    }
    else{
        return 0;
    }
}

