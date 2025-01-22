#pragma once

#include "sim_physical_layer.h"

#include <cmath>

class LoRaSimPhysicalLayer : public SimPhysicalLayer {
public:
    LoRaSimPhysicalLayer(std::shared_ptr<std::mutex> mtx, float frequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate = 1, uint8_t preambleLength = 8, bool crcEnabled = true, bool implicitHeader = false, bool lowDataRateOptimization = false);

    size_t sendPacket(std::vector<uint8_t> data) override; // override to include airtime simulation

private:
    float calculateAirtime(size_t payloadSize) const; // airtime calculator

    float m_frequency;       // Frequency in Hz
    float m_bandwidth;       // Bandwidth in Hz
    uint8_t m_spreadingFactor; // Spreading Factor (SF)
    uint8_t m_codingRate;     // Coding rate (denominator, e.g., 5 for 4/5)
    uint8_t m_preambleLength; // Preamble length (default: 8)
    bool m_crcEnabled;        // CRC enabled or not
    bool m_implicitHeader;    // Implicit header mode or not
    bool m_lowDataRateOptimization; // Low data rate optimization flag

};
