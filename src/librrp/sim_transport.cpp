#include "sim_transport.h"

size_t SimTransport::sendPacket(std::vector<uint8_t> buf){
    std::lock_guard<std::mutex> lock(_mtx);
    for (SimTransport* node : _nodes){
        node->pushToRxBuffer(buf);
    }
    return buf.size();
}

void SimTransport::setNodesList(std::vector<SimTransport *> nodes)
{
    _nodes = nodes;
}

void SimTransport::pushToRxBuffer(std::vector<uint8_t> buf)
{
    rxBuffer.push(buf);
}

size_t SimTransport::readPacket(std::vector<uint8_t>& buf){
    std::lock_guard<std::mutex> lock(_mtx);
    buf = rxBuffer.front();
    rxBuffer.pop();
    return buf.size();
}

