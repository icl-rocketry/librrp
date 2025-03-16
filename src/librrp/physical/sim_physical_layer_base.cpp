#include "sim_physical_layer_base.h"
#include <thread>
#include <sstream>
#include <libriccore/platform/millis.h>

bool SimPhysicalLayerBase::setup(){
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Sim Physical Layer: setup complete");
    return true;
}

bool SimPhysicalLayerBase::isBusy(){
    // implement some way of checking if the simulated medium is busy
    return false;
}

size_t SimPhysicalLayerBase::sendPacket(std::vector<uint8_t> data){
    std::lock_guard<std::mutex> lock(*m_mtx);
    for (SimPhysicalLayerBase* node : m_nodes){  // push packet to receive buffer of all physical nodes in the simulation environment
        if (node != this) {
            node->pushToRxBuffer(data);
        }
    }
    return data.size();
}


void SimPhysicalLayerBase::setNodesList(std::vector<SimPhysicalLayerBase*> nodes){
    m_nodes = nodes;
}

void SimPhysicalLayerBase::pushToRxBuffer(std::vector<uint8_t> data){
    // std::lock_guard<std::mutex> lock(m_mtx);
    rxBuffer.push(data);
}

size_t SimPhysicalLayerBase::readPacket(std::vector<uint8_t>& data){
    // std::lock_guard<std::mutex> lock(*m_mtx);
    if (rxBuffer.size()){
		m_info.timeLastPacketReceived = millis();
        data = rxBuffer.front();
        rxBuffer.pop();
    }
    return data.size();
}

void SimPhysicalLayerBase::restart(){
    //do something maybe
}

