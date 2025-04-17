#pragma once

// librnp
#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librnp/default_packets/simplecommandpacket.h>

// librrp
#include <librrp/physical/lora_sim_physical_layer.h>
#include <librrp/datalink/turn_timeout.h>
#include <librrp/datalink/tdma.h>

// libriccore
#include <libriccore/platform/millis.h>

#include "DummyCommands/dummy_commandhandler.h"

#include <mutex>
#include <memory>

enum class SimNode_COMMAND_IDS : uint8_t {
    getTime = 10
};

template <typename DataLinkProtocol>
class SimNode {
public:
    SimNode(int nodeNum, float frequency, float bandwidth, uint8_t spreadingFactor, bool pushDummyPackets = false)
        : m_nodeNum(nodeNum),
          m_pushDummyPackets(pushDummyPackets),
          m_simphysicallayer(frequency, bandwidth, spreadingFactor),
		  m_networkmanager(100, NODETYPE::LEAF, true),
		  m_radio(m_simphysicallayer, m_networkmanager),
          m_dummycommandhandler(static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND), createCommandMap()) 
    {RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("node" + std::to_string(m_nodeNum) + " constructed!");}

    ~SimNode() 
	{RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("node" + std::to_string(m_nodeNum) + " destroyed!");}

    void setup() {
        m_radio.setup();
		m_networkmanager.registerService(static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND),m_dummycommandhandler.getCallback()); 
        m_networkmanager.setNodeType(NODETYPE::HUB);
        m_networkmanager.addInterface(&m_radio);
		m_networkmanager.generateDefaultRoutes();            
        m_networkmanager.enableAutoRouteGen(true);
        m_networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST, {2});
		m_networkmanager.setLogCb([](const std::string& msg) {
			std::cout << "NetworkManager Log: " << msg << std::endl; 
			RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(msg);});
		int RNPAddress = 101 + m_nodeNum;
		if (find(simulatedRNPAddresses.begin(), simulatedRNPAddresses.end(), RNPAddress) == simulatedRNPAddresses.end()){
			simulatedRNPAddresses.push_back(RNPAddress);
		}
		m_networkmanager.setAddress(RNPAddress);
		RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Created sim node with RNP address = " + std::to_string(m_networkmanager.getAddress()));

        m_timeLastPacketPushed = millis();
    }

    void update() {
		m_networkmanager.update();
        m_radio.update();

        if (m_pushDummyPackets) {
            if (millis() - m_timeLastPacketPushed > m_sendDelta) {
                pushDummyPackets();
                m_timeLastPacketPushed = millis();
            }
        }
    }

    LoRaSimPhysicalLayer* getPhysicalLayer() {
        return &m_simphysicallayer;
    }

	int getNodeNum() {
		return m_nodeNum;
	}

private:
	int m_nodeNum;
	bool m_pushDummyPackets;
	LoRaSimPhysicalLayer m_simphysicallayer;
	RnpNetworkManager m_networkmanager;
	DataLinkProtocol m_radio;
    DummyCommandHandler<SimNode_COMMAND_IDS> m_dummycommandhandler;

	static std::vector<int> simulatedRNPAddresses; 

    uint32_t m_timeLastPacketPushed = 0;
    uint32_t m_sendDelta = 1000;

    void getTimeCommand(const RnpPacketSerialized& packet) {
        SimpleCommandPacket commandpacket(packet);

        uint32_t time = millis();

        BasicDataPacket<uint32_t, 0, 105> responsePacket(time);
        responsePacket.header.source_service = m_dummycommandhandler.getServiceID(); 
        responsePacket.header.destination_service = packet.header.source_service;
        responsePacket.header.source = packet.header.destination;
        responsePacket.header.destination = packet.header.source;
        responsePacket.header.uid = packet.header.uid;

        m_networkmanager.sendPacket(responsePacket);
    }

    void pushDummyPackets() {
        SimpleCommandPacket simplecommandpacket(static_cast<uint8_t>(SimNode_COMMAND_IDS::getTime), 0);
        simplecommandpacket.header.type = 101;
        simplecommandpacket.header.source = m_networkmanager.getAddress();
        simplecommandpacket.header.source_service = m_dummycommandhandler.getServiceID();

		int destAddress = -1;
		for (size_t i = 0; i < simulatedRNPAddresses.size(); ++i) {
			if (simulatedRNPAddresses[i] != m_networkmanager.getAddress()) {
				destAddress = simulatedRNPAddresses[i];
				break;  // Select the first address that is not this node's address
			}
		}

        simplecommandpacket.header.destination = destAddress;
        simplecommandpacket.header.destination_service = simplecommandpacket.header.source_service;
        simplecommandpacket.header.uid = simplecommandpacket.header.uid;

        m_networkmanager.sendPacket(simplecommandpacket);
    }

    typename DummyCommandHandler<SimNode_COMMAND_IDS>::commandMap_t createCommandMap() {
        return {
            {SimNode_COMMAND_IDS::getTime, 
                [this](const RnpPacketSerialized& packet) { this->getTimeCommand(packet); }}
        };
    }
};

template <typename DataLinkProtocol>
std::vector<int> SimNode<DataLinkProtocol>::simulatedRNPAddresses;