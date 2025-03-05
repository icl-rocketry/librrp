//librnp
#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librnp/default_packets/simplecommandpacket.h>

//librrp
#include <librrp/physical/lora_sim_physical_layer.h>
#include <librrp/datalink/turn_timeout.h>
#include <librrp/datalink/tdma.h>

// libriccore
#include <libriccore/platform/millis.h>

#include <mutex>
#include <memory>

template <typename DataLinkProtocol>
class SimNode {
    public:
		SimNode(RnpNetworkManager& networkmanager, std::shared_ptr<std::mutex> mtx, float frequency, float bandwidth, uint8_t spreadingFactor)
			: m_networkmanager(networkmanager),
			m_simphysicallayer(mtx, frequency, bandwidth, spreadingFactor),
			m_radio(m_simphysicallayer, networkmanager)
			{}

        
        void setup(){
            
            m_radio.setup();

            m_networkmanager.setNodeType(NODETYPE::HUB);
            m_networkmanager.addInterface(&m_radio);
            m_networkmanager.enableAutoRouteGen(true);
            m_networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST, {2});

            m_timeLastPacketPushed = millis();
        };

        void update(){
            m_radio.update();

            if (millis() - m_timeLastPacketPushed > m_sendDelta){
                // pushDummyPackets();
                m_timeLastPacketPushed = millis();
            }
        }

        LoRaSimPhysicalLayer* getPhysicalLayer(){
            return &m_simphysicallayer;
        }

    private: 
        RnpNetworkManager& m_networkmanager;
        LoRaSimPhysicalLayer m_simphysicallayer;
        DataLinkProtocol m_radio;

        uint32_t m_timeLastPacketPushed = 0;
		uint32_t m_sendDelta = 1000;

        void pushDummyPackets(){
            SimpleCommandPacket simplecommandpacket(1,1);
            simplecommandpacket.header.type = 101;
            simplecommandpacket.header.source = m_networkmanager.getAddress();
            simplecommandpacket.header.source_service = static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND);
            simplecommandpacket.header.destination = 100;
            simplecommandpacket.header.destination_service = simplecommandpacket.header.source_service;
            simplecommandpacket.header.uid = simplecommandpacket.header.uid;

            m_networkmanager.sendPacket(simplecommandpacket);
        }

};