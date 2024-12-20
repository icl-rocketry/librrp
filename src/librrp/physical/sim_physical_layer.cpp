#include "sim_physical_layer.h"

bool SimPhysicalLayer::setup(){
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Sim Physical Layer: setup complete");
    return true;
}

bool SimPhysicalLayer::isBusy(){
    // implement some way of checking if the simulated medium is busy
    return false;
}

size_t SimPhysicalLayer::sendPacket(std::vector<uint8_t> data){
    std::lock_guard<std::mutex> lock(m_mtx);
    for (SimPhysicalLayer* node : m_nodes){  // push packet to receive buffer of all physical nodes in the simulation environment
        if (node != this) {
            node->pushToRxBuffer(data);
            // RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Sim Physical Layer: sending packet, size = " + std::to_string(data.size()));
        }
    }
    return data.size();
}


void SimPhysicalLayer::setNodesList(std::vector<SimPhysicalLayer *> nodes)
{
    m_nodes = nodes;
}

void SimPhysicalLayer::pushToRxBuffer(std::vector<uint8_t> data)
{
    rxBuffer.push(data);
}

size_t SimPhysicalLayer::readPacket(std::vector<uint8_t>& data){
    std::lock_guard<std::mutex> lock(m_mtx);
    if (rxBuffer.size()){
        data = rxBuffer.front();
        // RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Sim Physical Layer: received packet, size = " + std::to_string(data.size()));
        rxBuffer.pop();
    }
    return data.size();
}

void SimPhysicalLayer::restart(){
    //do something maybe
}

