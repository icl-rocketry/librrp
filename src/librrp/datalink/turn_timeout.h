#pragma once

// std
#include <memory>
#include <vector>
#include <string>
#include <queue>

// Ric
#include <librnp/rnp_interface.h>
#include <librnp/rnp_packet.h>
#include <librnp/rnp_networkmanager.h>
#include <libriccore/riccorelogging.h>


// #include <librrp/rrp_nvs_save.h>


struct RadioInterfaceInfo : public RnpInterfaceInfo {
    uint32_t prevTimeSent;
    uint32_t prevTimeReceived;
    size_t currentSendBufferSize;
    bool sendBufferOverflow;
    bool txDone;
    bool received;
    size_t sendBufferSize;
	uint32_t txCount;
	uint32_t rxCount;

	    int rssi;
    int packet_rssi;
    float snr;
    float packet_snr;
    long freqError;
};

struct TimeoutConfig {
    uint32_t turnTimeout;
};

template <typename PhysicalLayer>
class TimeoutRadio : public RnpInterface {
public:
    TimeoutRadio(PhysicalLayer& physicalLayer,
		RnpNetworkManager& networkManager,
        uint8_t id = 2, 
        std::string name = "Timeout radio")
        : RnpInterface(id, name),
		  _physicalLayer(physicalLayer), 
		  _networkManager(networkManager),
          _info{} 
		  {
            _info.MTU = 256;
            _info.sendBufferSize = 2048;
			_info.txCount = 0;
			_info.rxCount = 0;
          }

    void setup() override {
        if (!_physicalLayer.setup())
		{
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Timeout Radio: failed to setup physical layer");
			return ;
		}
		_physicalLayer.setChannel(0);
    }

    void sendPacket(RnpPacket& data) override {

        const size_t dataSize = data.header.size() + data.header.packet_len;


        if (dataSize > _info.MTU){ // will implement packet segmentation here at a later data
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Timeout Radio: packet exceeds MTU (size=" + std::to_string(dataSize) + ", MTU=" + std::to_string(_info.MTU) + ")");
            ++_info.txerror;
            return;
        }
        if (dataSize + _info.currentSendBufferSize > _info.sendBufferSize){
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Timeout Radio: send buffer overflow (size=" + std::to_string(_info.currentSendBufferSize) + ", limit=" + std::to_string(_info.sendBufferSize) + ")");
            ++_info.txerror;
            _info.sendBufferOverflow = true;
            return;
        }

        std::vector<uint8_t> serializedPacket;
        data.serialize(serializedPacket);
        _sendBuffer.push(serializedPacket); // add to send buffer
        _info.sendBufferOverflow = false;
        _info.currentSendBufferSize += dataSize;
        checkSendBuffer(); // see if we can send 
    
    }

    void update() override {
        
        std::vector<uint8_t> rxData;
        if (_physicalLayer.readPacket(rxData)){  // received data

			RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Timeout Radio: packet received");

            if (_packetBuffer == nullptr){
                return;
            }
            std::unique_ptr<RnpPacketSerialized> packet_ptr;

            try{
                packet_ptr = std::make_unique<RnpPacketSerialized>(rxData);
            }
            catch (std::exception& e)
            {
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Deserialization error: " + std::string(e.what()));
                return;
            }

            //update source interface
            packet_ptr->header.src_iface = getID();
            _packetBuffer->push(std::move(packet_ptr));//add packet ptr  to buffer
            _info.received=true; 
			_info.rxCount++;

			RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Tx = " + std::to_string(_info.txCount) + ", Rx = " + std::to_string(_info.rxCount));
        }

        checkSendBuffer();
    }

    void checkSendBuffer(){
        if (_sendBuffer.size() == 0){
            return; // exit if nothing in the buffer
        }

        if (_info.received || (millis()-_info.prevTimeSent > _config.turnTimeout)){
            sendFromBuffer();
        }
    }

    void sendFromBuffer()
    {
        size_t bytes_written = _physicalLayer.sendPacket(_sendBuffer.front());
        if (bytes_written){ // if we succesfully send packet
            _sendBuffer.pop(); //remove packet from buffer
            _info.currentSendBufferSize -= bytes_written;
            _info.txDone = false;
            _info.prevTimeSent = millis();
            _info.received = false;
			_info.txCount++;
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Timeout Radio: packet sent");
        }
    }
    

    const RnpInterfaceInfo* getInfo() override {
        // this needs to be data link layer info plus encapsulation of physical layer info
        return &_info;
    }

    // const TimeoutConfig& getConfig() const {
    //     return _config;
    // }

    // void setConfig(TimeoutConfig config)
    // {
    //     _config = config;
    //     _physicalLayer.restart();
    // }

    // void setConfig(TimeoutConfig config, bool overrideNVS) {
    //     _config = config;
    //     _physicalLayer.restart();
    //     if (overrideNVS) {
    //         saveConf();
    //     }
    // }


    // void saveConf() {
    //     RrpNvsSave::SaveValueToNVS("Timeout Radio Config", "turnTimeout", _config.turnTimeout);
    //     RrpNvsSave::SaveValueToNVS("Timeout Radio Config", "sendBufferSize", _info.sendBufferSize);
    // }

    // void loadConf() {
    //     _config = defaultConfig;

    //     if(!RrpNvsSave::ReadValueFromNVS("Timeout Radio Config", "turnTimeout", _config.turnTimeout)){
    //         RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Turn timeout not configured!");
    //     }

    //     if(!RrpNvsSave::ReadValueFromNVS("Timeout Radio Config", "sendBufferSize", _info.sendBufferSize)){
    //         RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Send buffer size of turn timeout radio not configured!");
    //     }
    // };

private:
    PhysicalLayer& _physicalLayer;
	RnpNetworkManager& _networkManager;
    RadioInterfaceInfo _info;
    TimeoutConfig _config;
    static constexpr TimeoutConfig defaultConfig{static_cast<uint32_t>(250)};

    std::queue<std::vector<uint8_t>> _sendBuffer;
};
