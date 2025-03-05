#include "lora_sim_physical_layer.h"
#include <chrono>
#include <thread>
#include <algorithm>

LoRaSimPhysicalLayer::LoRaSimPhysicalLayer(std::shared_ptr<std::mutex> mtx, float frequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate, uint8_t preambleLength, bool crcEnabled, bool implicitHeader, bool lowDataRateOptimization)
    : SimPhysicalLayerBase(mtx)
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


size_t LoRaSimPhysicalLayer::sendPacket(std::vector<uint8_t> data) {
    if (!m_mtx->try_lock()) {		// Mutex is already locked, indicating a collision
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa Sim Physical Layer: Packet collision detected");
        m_transmissionHistory.push_back(false);
    }
	else {

		double airtime = calculateAirtime(data.size());    // Calculate airtime

		// Simulate packet transmission (mutex held for airtime duration)
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(airtime * 1000)));

		// Send the packet to all nodes except itself
		for (SimPhysicalLayerBase* node : m_nodes) {
			if (node && node != this) {
				node->pushToRxBuffer(data);
			}
		}

		m_mtx->unlock();
		m_transmissionHistory.push_back(true);
	}

	if (m_transmissionHistory.size() > m_movingAverageWindowSize) {
		m_transmissionHistory.pop_front();
	}

    return data.size();
}


float LoRaSimPhysicalLayer::calculateSuccessRatio() const {
    if (m_transmissionHistory.empty()) {
        return 1.0f;
    }

    size_t successCount = std::count(m_transmissionHistory.begin(), m_transmissionHistory.end(), true);
    return static_cast<float>(successCount) / m_transmissionHistory.size();
}
