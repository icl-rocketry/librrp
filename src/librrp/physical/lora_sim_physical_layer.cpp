#include "lora_sim_physical_layer.h"
#include <chrono>
#include <thread>

LoRaSimPhysicalLayer::LoRaSimPhysicalLayer(std::shared_ptr<std::mutex> mtx, float frequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate, uint8_t preambleLength, bool crcEnabled, bool implicitHeader, bool lowDataRateOptimization)
    : SimPhysicalLayer(mtx),
      m_frequency(frequency),
      m_bandwidth(bandwidth),
      m_spreadingFactor(spreadingFactor),
      m_codingRate(codingRate),
      m_preambleLength(preambleLength),
      m_crcEnabled(crcEnabled),
      m_implicitHeader(implicitHeader),
      m_lowDataRateOptimization(lowDataRateOptimization) {}


float LoRaSimPhysicalLayer::calculateAirtime(size_t payloadSize) const {
    // Symbol duration (T_symbol)
    float tSymbol = std::pow(2, m_spreadingFactor) / m_bandwidth;

    // Compute payload symbols
    int payloadSymbols = 8 + std::max(
        static_cast<int>(std::ceil(
            (8.0 * payloadSize - 4.0 * m_spreadingFactor + 28.0 + 16.0 * m_crcEnabled - 20.0 * m_implicitHeader) /
            (4.0 * (m_spreadingFactor - 2.0 * m_lowDataRateOptimization))
        ) * (m_codingRate + 4)), 
        0);

    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Payload size = " + std::to_string(payloadSize) + ", airtime = " + std::to_string(tSymbol * (m_preambleLength + payloadSymbols)));

    // Total time = T_symbol * (Preamble + Payload)
    return tSymbol * (m_preambleLength + payloadSymbols);
}



size_t LoRaSimPhysicalLayer::sendPacket(std::vector<uint8_t> data) {
    if (!m_mtx->try_lock()) {
        // Mutex is already locked, indicating a collision
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(
            "LoRa Sim Physical Layer: Packet collision detected"
        );
        // Packet does not reach other nodes
        return data.size();
    }

    // Calculate airtime
    double airtime = calculateAirtime(data.size());

    // Send the packet to all nodes except itself
    for (SimPhysicalLayer* node : m_nodes) {
        if (node && node != this) {
            node->pushToRxBuffer(data);
        }
    }

    // Log successful transmission
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(
        "LoRa Sim Physical Layer: Packet sent successfully, size = " + std::to_string(data.size())
    );

    // Simulate packet transmission (mutex held for airtime duration)
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(airtime * 1000)));

    m_mtx->unlock();
    return data.size();
}