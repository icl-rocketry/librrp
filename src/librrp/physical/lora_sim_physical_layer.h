#pragma once

#include "sim_physical_layer_base.h"

//std
#include <cmath>

struct LoraSimPhysicalLayerInfo : public PhysicalLayerInfo {
    float frequency;       // Frequency in Hz
    float bandwidth;       // Bandwidth in Hz
    uint8_t spreadingFactor; // Spreading Factor (SF)
    uint8_t codingRate;     // Coding rate (denominator, e.g., 5 for 4/5)
    uint8_t preambleLength; // Preamble length (default: 8)
    bool crcEnabled;        // CRC enabled or not
    bool implicitHeader;    // Implicit header mode or not
    bool lowDataRateOptimization; // Low data rate optimization flag
};

class LoRaSimPhysicalLayer : public SimPhysicalLayerBase {
public:
    LoRaSimPhysicalLayer(std::shared_ptr<std::mutex> mtx, float frequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate = 1, uint8_t preambleLength = 8, 
				bool crcEnabled = true, bool implicitHeader = false, bool lowDataRateOptimization = false);
    size_t sendPacket(std::vector<uint8_t> data) override; // override to include airtime simulation
    float calculateAirtime(size_t payloadSize) const; // airtime calculator
    float calculateSuccessRatio() const;
	const PhysicalLayerInfo* getInfo() override {return &m_info;}

private:
	LoraSimPhysicalLayerInfo m_info;
	std::deque<bool> m_transmissionHistory;
    const size_t m_movingAverageWindowSize = 100;
};
