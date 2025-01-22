//librnp
#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librnp/default_packets/simplecommandpacket.h>

//librrp
#include <librrp/physical/lora_sim_physical_layer.h>
#include <librrp/datalink/turn_timeout.h>

// libriccore
#include <libriccore/platform/millis.h>

#include <mutex>
#include <memory>

template <typename DataLinkProtocol>
class SimNode {
    public:
        SimNode(std::shared_ptr<std::mutex> mtx, float frequency, float bandwidth, uint8_t spreadingFactor)
            : simphysicallayer(mtx, frequency, bandwidth, spreadingFactor),
            radio(simphysicallayer) {}
        
        void setup(){
            
            radio.setup();

            networkmanager.setNodeType(NODETYPE::HUB);
            networkmanager.addInterface(&radio);
            networkmanager.enableAutoRouteGen(true);
            networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST, {2});

            m_timeLastPacketPushed = millis();
        };

        void update(){
            radio.update();

            if (millis() - m_timeLastPacketPushed > 1000){
                pushDummyPackets();
                m_timeLastPacketPushed = millis();
            }
        }

        LoRaSimPhysicalLayer* getPhysicalLayer(){
            return &simphysicallayer;
        }

    private: 
        RnpNetworkManager networkmanager;
        LoRaSimPhysicalLayer simphysicallayer;
        DataLinkProtocol radio;

        uint64_t m_timeLastPacketPushed = 0;

        void pushDummyPackets(){
            SimpleCommandPacket simplecommandpacket(1,1);
            simplecommandpacket.header.type = 101;
            simplecommandpacket.header.source = networkmanager.getAddress();
            simplecommandpacket.header.source_service = static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND);
            simplecommandpacket.header.destination = 100;
            simplecommandpacket.header.destination_service = simplecommandpacket.header.source_service;
            simplecommandpacket.header.uid = simplecommandpacket.header.uid;

            networkmanager.sendPacket(simplecommandpacket);
        }

};