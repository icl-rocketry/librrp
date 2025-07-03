#include "lora_sim_physical_layer.h"
#include <thread>
#include <sstream>
#include <libriccore/platform/millis.h>
#include <libriccore/riccorelogging.h>

RadioChannelManager LoRaSimPhysicalLayer::radioChannelManager;

LoRaSimPhysicalLayer::LoRaSimPhysicalLayer(float frequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate, uint8_t preambleLength, bool crcEnabled, bool implicitHeader, bool lowDataRateOptimization)
	{
		m_info.frequency = frequency;
      	m_info.bandwidth = bandwidth;
      	m_info.spreadingFactor = spreadingFactor;
      	m_info.codingRate = codingRate;
      	m_info.preambleLength = preambleLength;
      	m_info.crcEnabled = crcEnabled;
      	m_info.implicitHeader = implicitHeader;
      	m_info.lowDataRateOptimization = lowDataRateOptimization;
	}

bool LoRaSimPhysicalLayer::setup(){
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa Sim Physical Layer: setup complete");
    return true;
}

bool LoRaSimPhysicalLayer::isBusy(){
	return false;
}

size_t LoRaSimPhysicalLayer::sendPacket(std::vector<uint8_t> data){
	if (m_currentChannel == -1) return 0;

	uint32_t airtimeMs = static_cast<uint32_t>(calculateAirtime(data.size()) * 1000);
	
    auto channel = radioChannelManager.getChannel(m_currentChannel);
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa Sim Physical Layer: sending packet on channel " + std::to_string(m_currentChannel));
    channel->transmitPacket(data, airtimeMs, this);
	
	return data.size();
}

size_t LoRaSimPhysicalLayer::readPacket(std::vector<uint8_t>& data){
    if (!m_rxBuffer.empty()) {
        data = m_rxBuffer.front();
        m_rxBuffer.pop();
        return data.size();
    }    
	return 0;
}

float LoRaSimPhysicalLayer::calculateAirtime(size_t payloadSize) const {
    float tSymbol = std::pow(2, m_info.spreadingFactor) / m_info.bandwidth;

    int payloadSymbols = 8 + std::max(
        static_cast<int>(std::ceil(
            (8.0 * payloadSize - 4.0 * m_info.spreadingFactor + 28.0 + 16.0 * m_info.crcEnabled - 20.0 * m_info.implicitHeader) /
            (4.0 * (m_info.spreadingFactor - 2.0 * m_info.lowDataRateOptimization))
        ) * (m_info.codingRate + 4)), 
        0);

    // Total time = T_symbol * (Preamble + Payload)
    return tSymbol * (m_info.preambleLength + payloadSymbols);
}

void LoRaSimPhysicalLayer::restart(){
    //do something maybe
}


void LoRaSimPhysicalLayer::setChannel(int newChannel){
	if (m_currentChannel != -1) {
		radioChannelManager.unregisterNode(m_currentChannel, this);
	}
	
	m_currentChannel = newChannel;
	
	radioChannelManager.registerNode(m_currentChannel, this, [this](const std::vector<uint8_t>& data) { pushToRxBuffer(data); });
}

void LoRaSimPhysicalLayer::pushToRxBuffer(std::vector<uint8_t> data) {
	m_rxBuffer.push(data);
}
