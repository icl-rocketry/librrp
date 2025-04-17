#pragma once

// std
#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

// Ric
#include <libriccore/riccorelogging.h>
#include <librnp/rnp_interface.h>
#include <librnp/rnp_packet.h>
#include <librnp/rnp_networkmanager.h>
#include <libriccore/platform/millis.h>
#include <librrp/rrp_nvs_save.h>

struct TDMARadioInterfaceInfo : public RnpInterfaceInfo 
{
    size_t maxSendBufferSize;
	size_t maxPayloadSize;
	size_t currentSendBufferSize;
    bool sendBufferOverflow;
};

enum TDMA_MODE : uint8_t
{
    DISCOVERY,
    TRANSMIT,
    RECEIVE
};

enum DISCOVERY_PHASE : uint8_t
{   
    ENTRY,
    SNIFFING,
    INIT_NETWORK,
    SYNCING,
    JOIN_REQUEST,
    JOIN_REQUEST_RESPONSE,
    EXIT
};

enum PACKET_TYPE : uint8_t
{
    NORMAL,
    ACK,
    NACK,
    JOINREQUEST,
    HEARTBEAT
};

template <typename PhysicalLayer>
class TDMARadio : public RnpInterface 
{
	public:
		TDMARadio(PhysicalLayer& physicalLayer,
				RnpNetworkManager& networkManager,
				uint8_t id = 2,
				std::string name = "TDMA radio")
			: 	m_networkManager(networkManager),
				m_physicalLayer(physicalLayer),
				m_info{},
				RnpInterface(id, name)
				{
					m_info.MTU = 256;
					m_info.maxPayloadSize = 80;
					m_info.maxSendBufferSize = 2048;
				}

		void setup() override 
		{
			if (m_physicalLayer.setup()) {
				m_physicalLayer.setChannel(0);
				calcTimeWindowLength();
				m_timeWindows = 1;			// single timewindow where node just listens
			}
			m_timeMovedTimeWindow = millis();
			srand(millis());
		}

		void sendPacket(RnpPacket& data) override
		{
			const size_t dataSize = data.header.size() + data.header.packet_len;

			if (dataSize > m_info.MTU){ 
				RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Packet exceeds interface MTU");
				++m_info.txerror;
				return;
			}
			if (dataSize + m_info.currentSendBufferSize > m_info.maxSendBufferSize){
				RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Send buffer overflow");
				++m_info.txerror;
				m_info.sendBufferOverflow = true;
				return;
			}

			std::unique_ptr<std::vector<uint8_t>> serializedPacket = std::make_unique<std::vector<uint8_t>>();
			data.serialize(*serializedPacket);
			m_sendBuffer.push(*serializedPacket);
			m_info.sendBufferOverflow = false;
			m_info.currentSendBufferSize += dataSize;
		}

		void update() override
		{
			getPacket();	// gotta scan for packets on every loop otherwise packet time-based info is inaccurate

			if (millis() - (m_timeMovedTimeWindow) >= m_timeWindowLength){
				m_currTimeWindow = (m_currTimeWindow + 1) % m_timeWindows;	// shift timewindow
				RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Shifted timewindow to " + std::to_string(m_currTimeWindow));
				m_timeMovedTimeWindow = millis();
		
				// reset bools
				m_packetSent = false;
				m_received = false;
				m_txWindowDone = false;
				m_rxWindowDone = false;
				m_skippingTurn = false;
			}
		
			if (m_currMode == TDMA_MODE::DISCOVERY){
				discovery();
			}
			else{
				if(m_currTimeWindow == m_txTimeWindow){
					m_currMode = TDMA_MODE::TRANSMIT;
					if(!m_txWindowDone){
						tx();
					}
				}
				else{
					m_currMode = TDMA_MODE::RECEIVE;
					if(!m_rxWindowDone){
						rx();
					}
				}
			}
		}

		const RnpInterfaceInfo* getInfo() override {
			return &m_info;
		}

	private:

		void calcTimeWindowLength()
		{
			float maxFrameLength = 2;	// assuming 2 seconds
			float clockDrift = 2e-5;	// s/s worst drift based on the current xtal
			float Tg = maxFrameLength * clockDrift;		// this calc doesnt give big enough value, i think it should be calculated based on the loop speed, clock drift is negligible in comparison
			m_timeWindowLength = (m_physicalLayer.calculateAirtime(m_info.maxPayloadSize + m_tdmaHeaderSize) + m_physicalLayer.calculateAirtime(m_tdmaHeaderSize) + Tg) * 1e3f;	// (payload + TDMA header) + ack
			RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Calculated timewindow length = " + std::to_string(m_timeWindowLength));
		}
	
		void discovery(){

			switch(m_currDiscoveryPhase) {

				case DISCOVERY_PHASE::ENTRY: {
					RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Entered Discovery");
					m_timeEnteredDiscovery = millis();					// timestamp entry into discovery
					m_currDiscoveryPhase = DISCOVERY_PHASE::SNIFFING; 	// transition to next phase
					break;
				}

				case DISCOVERY_PHASE::SNIFFING: {

					if (m_received){
						m_currDiscoveryPhase = DISCOVERY_PHASE::SYNCING;       // transition to network syncing
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Network detected");
					}
					else if (millis() - m_timeEnteredDiscovery > m_discoveryTimeout){
						m_currDiscoveryPhase = DISCOVERY_PHASE::INIT_NETWORK;  // transition to network initialisation
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: No network activity detected, initialising network");
					}
					break;
				}
				

				case DISCOVERY_PHASE::INIT_NETWORK: {
					initNetwork();
					m_currDiscoveryPhase = DISCOVERY_PHASE::EXIT;
					break;
				}


				case DISCOVERY_PHASE::SYNCING: {
					sync();
					m_currDiscoveryPhase = DISCOVERY_PHASE::JOIN_REQUEST;
					break;
				}


				case DISCOVERY_PHASE::JOIN_REQUEST: {
					if(m_currTimeWindow == m_txTimeWindow){
						if (static_cast<float>(rand())/RAND_MAX > 0.5){
							std::vector<uint8_t> emptyPacket;
							if(sendPacketWithTDMAHeader(emptyPacket, PACKET_TYPE::JOINREQUEST, m_lastPacketSource) > 0){
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Join request sent");
								m_packetSent = true;
								m_received = false;
								m_timeJoinRequestSent = millis();
								m_currDiscoveryPhase = DISCOVERY_PHASE::JOIN_REQUEST_RESPONSE; // transition to waiting for response
							}
						}
						else{
							m_currDiscoveryPhase = DISCOVERY_PHASE::ENTRY;
						}
					} 
					break;
				}

				case DISCOVERY_PHASE::JOIN_REQUEST_RESPONSE: {

					// join request ack
					if(m_received && m_lastPacketType == PACKET_TYPE::ACK && m_lastPacketDest == m_networkManager.getAddress()){
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Received join request ack");

						m_timeWindows = m_lastPacketRegNodes + 1;   // set local number of timewindows to match network
						m_txTimeWindow = m_timeWindows - 2;			// set local tx timewindow
						m_currTimeWindow = m_lastPacketTimeWindow;

						m_regNodes.resize(m_lastPacketRegNodes);
						m_regNodes[m_lastPacketInfo] = m_lastPacketSource;	// m_lastPacketInfo field contains the tx timewindow of the acking node in the case of an ack
						m_regNodes[m_regNodes.size() - 1] = m_networkManager.getAddress();	// add self to end of list

						m_received = false;
						m_currDiscoveryPhase = DISCOVERY_PHASE::EXIT;

						for (int i = 0; i < m_regNodes.size(); ++i){
							RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("reg nodes after discovery phase: " + std::to_string(m_regNodes[i]));
						}
					}

					// join request nack
					else if(m_received && m_lastPacketType == PACKET_TYPE::NACK && m_lastPacketDest == m_networkManager.getAddress()){  

						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: This node has joined before");
						m_currTimeWindow = m_lastPacketTimeWindow;    // sync local current timewindow to network
						m_timeWindows = m_lastPacketRegNodes + 1;
						m_txTimeWindow = m_lastPacketInfo;	// m_lastPacketInfo field contains the tx timewindow of the requesting node in the case of a nack

						// TODO: this needs to change cuz we need to place the address of the nacking node in the right position in the reg nodes list
						m_regNodes.push_back(m_lastPacketSource);
						m_regNodes.insert(m_regNodes.begin() + m_txTimeWindow, m_networkManager.getAddress());
						//
						m_received = false;
						m_currDiscoveryPhase = DISCOVERY_PHASE::EXIT;

						for (int i = 0; i < m_regNodes.size(); ++i){
							RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("reg nodes after discovery phase: " + std::to_string(m_regNodes[i]));
						}
					}

					// join request expired
					if (millis() - m_timeJoinRequestSent > m_joinRequestTimeout || (m_received && m_lastPacketType == PACKET_TYPE::HEARTBEAT)){
						m_currDiscoveryPhase = DISCOVERY_PHASE::JOIN_REQUEST;  // try again
					}
					break;
				}

				
				case DISCOVERY_PHASE::EXIT: {
					m_currMode = TDMA_MODE::TRANSMIT;      // to exit out of discovery, assign any other mode other than discovery
					RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Exiting discovery");
					break;
				}


				default: {
					break;
				}

			}

		}

		void getPacket(){
			//TODO: add counter for time havent heard back from node
			//check if radio is still transmitting

			std::vector<uint8_t> data;
		
			if (m_physicalLayer.readPacket(data)){
				m_received = true;
				m_timeLastPacketReceived = millis();
		
				try{					// unpack TDMA header
					unpackTDMAHeader(data); // modifies data vector
				} catch (std::exception& e){
					RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Error: " + std::string(e.what()));
					return;
				}
		
				// TODO: fix this shit
				if (m_currMode != TDMA_MODE::DISCOVERY && m_lastPacketRegNodes - static_cast<uint8_t>(m_regNodes.size()) > 0){  //local node list is shorter
					RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("RESIZING REG NODES: old = " + std::to_string(m_regNodes.size()) + ", new = " + std::to_string(m_lastPacketRegNodes));
					m_regNodes.resize(m_lastPacketRegNodes);
					m_timeWindows = m_lastPacketRegNodes + 1;
					m_currTimeWindow = m_lastPacketTimeWindow;
				}        
		
				if (data.size()){     // packet contains something after unpacking the tdma header
		
					if (_packetBuffer == nullptr){
						return;
					}
					std::unique_ptr<RnpPacketSerialized> packet_ptr;
					try
					{
						packet_ptr = std::make_unique<RnpPacketSerialized>(data);
					}
					catch (std::exception& e)
					{
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Deserialization error: " + std::string(e.what()));
						return;
					}
		
					// update member variables with rnp header info
					m_lastPacketSource = packet_ptr->header.source;
					m_lastPacketDest = packet_ptr->header.destination;
		
					// update source interface
					packet_ptr->header.src_iface = getID();
					_packetBuffer->push(std::move(packet_ptr));	// add packet ptr to rnp packet buffer
				}
			}
		}

		void tx(){

			if(m_sendBuffer.size()){    //buffer not empty

				// if(m_countsNoAck > m_maxCountsNoAck){  	// check if exceed limit on resends
				// 	uint8_t poppedPacketSize = sizeof(m_sendBuffer.back());
				// 	m_sendBuffer.pop();      			// pop old packet
				// 	m_info.currentSendBufferSize -= poppedPacketSize;
				// 	m_countsNoAck = 0;      				// reset counter
				// 	m_txWindowDone = true;
				// }

				if(!m_packetSent){
					uint8_t bytesWritten = sendPacketWithTDMAHeader(m_sendBuffer.front(), PACKET_TYPE::NORMAL, 0) - m_tdmaHeaderSize;
					if (bytesWritten){
						m_packetSent = true;
						m_received = false;
						m_sendBuffer.pop();
						m_info.currentSendBufferSize -= bytesWritten;
						// m_countsNoAck++;  // just trust me bro, it makes sense
						m_countsNoTx = 0; 
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: RNP packet sent");
					}
				}
				else{                   // packet has been sent
					// RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Waiting for Ack");
					// if (m_received)
					// {
					// 	RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("received something: " +  std::to_string(m_lastPacketType));
					// }
					// if (m_received && m_lastPacketType == PACKET_TYPE::ACK){ // packet got acked
					// 	RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("got Ack");
					// 	uint8_t poppedPacketSize = sizeof(m_sendBuffer.back());
					// 	m_sendBuffer.pop();      			// pop old packet
					// 	m_info.currentSendBufferSize -= poppedPacketSize;
					// 	m_countsNoAck = 0;      				// reset counter
					// 	m_txWindowDone = true;
					// }
				} 
		
			}
			else{                           // buffer empty
				if (m_countsNoTx >= m_maxCountsNoTx){        // node didn't transmit in a long time
					std::vector<uint8_t> emptyPacket;
					sendPacketWithTDMAHeader(emptyPacket, PACKET_TYPE::HEARTBEAT, 0);
					m_countsNoTx = 0;
					m_txWindowDone = true;
					RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Heartbeat packet sent");
				}
				else{ 
					m_countsNoTx ++;             // update counter
					m_txWindowDone = true;       // exit tx mode
				}
		
			}
		}

		void rx(){

			if(m_received){
				if (!(m_lastPacketType == ACK || m_lastPacketType == NACK)){	// cuz acks and nacks can be sent at the end of the timewindow
					m_timeMovedTimeWindow = m_timeLastPacketReceived - static_cast<uint32_t>(m_physicalLayer.calculateAirtime(m_lastPacketSize)*1e3f);
				}
		
				switch (m_lastPacketType) {
		
					case PACKET_TYPE::JOINREQUEST: {
						
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Received join request");
		
						if (m_lastPacketDest == m_networkManager.getAddress()){
							auto it = find(m_regNodes.begin(), m_regNodes.end(), m_lastPacketSource);
			
							if(it == m_regNodes.end()){                        		// node has not been registered yet
								m_regNodes.push_back(m_lastPacketSource);           // add to node list
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: RNP Node (requesting node) " + std::to_string(m_lastPacketSource) + " added to list");
								m_timeWindows = m_regNodes.size() + 1;             	// update number of timewindows
								std::vector<uint8_t> emptyPacket;
								sendPacketWithTDMAHeader(emptyPacket, PACKET_TYPE::ACK, m_lastPacketSource, m_txTimeWindow);
								m_rxWindowDone = true;
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Join request acked");
							}
							else{                                                   // node has already been registered
								uint8_t requesterTxTimewindow = static_cast<uint8_t>(it - m_regNodes.begin());
								std::vector<uint8_t> emptyPacket;
								sendPacketWithTDMAHeader(emptyPacket, PACKET_TYPE::NACK, m_lastPacketSource, requesterTxTimewindow);
								m_rxWindowDone = true;      
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Join request nacked");   
							}
						}
						break;
					}
						
					
					case PACKET_TYPE::NORMAL: {                                  // handling RNP packet
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Received RNP packet");
						// std::vector<uint8_t> emptyPacket;
						// sendPacketWithTDMAHeader(emptyPacket, PACKET_TYPE::ACK, m_lastPacketSource);
						if (m_lastPacketSource != m_regNodes[m_currTimeWindow]){
							if (!m_regNodes[m_currTimeWindow]){
								m_regNodes[m_currTimeWindow] = m_lastPacketSource;
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Updating 0 value with missed address " + std::to_string(m_lastPacketSource));
							}
							else{
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Received packet from " + std::to_string(m_lastPacketSource) + " when expecting " + std::to_string(m_regNodes[m_currTimeWindow]));
							}
						}
						m_rxWindowDone = true; 
						break; 
					}

					case PACKET_TYPE::HEARTBEAT: { 
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Received heartbeat packet");
						if (m_lastPacketSource != m_regNodes[m_currTimeWindow]){
							if (!m_regNodes[m_currTimeWindow]){
								m_regNodes[m_currTimeWindow] = m_lastPacketSource;
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Updating 0 value with missed address " + std::to_string(m_lastPacketSource));
							}
							else{
								RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Received packet from " + std::to_string(m_lastPacketSource) + " when expecting " + std::to_string(m_regNodes[m_currTimeWindow]));
							}
						}
						m_rxWindowDone = true; 
						break; 
					}
		
					default: {                                                  // handling other packet
						RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Radio: Received packet type: " + std::to_string(m_lastPacketType));
						m_rxWindowDone = true;
						break;
					}
		
				}
				m_received = false;
			}
		}

		void sync(){
			m_currTimeWindow = m_lastPacketTimeWindow;    // sync local current timewindow to network
			m_txTimeWindow = m_lastPacketRegNodes;        // set to send join request in the n+1th timewindow
			m_timeWindows = m_lastPacketRegNodes+1;       // update local number of timewindows
			RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Syncing: time last packet received = " + std::to_string(m_timeLastPacketReceived) + ", airtime of packet = " + std::to_string(static_cast<uint32_t>(m_physicalLayer.calculateAirtime(m_lastPacketSize)*1e3f)) + 
				"last packet timewindow = " + std::to_string(m_lastPacketTimeWindow));
			m_timeMovedTimeWindow = m_timeLastPacketReceived - static_cast<uint32_t>(m_physicalLayer.calculateAirtime(m_lastPacketSize)*1e3f);
			m_synced = true;                        // syncing complete
		}

		void initNetwork(){
			m_regNodes.push_back(m_networkManager.getAddress());	// add own address to the list (should be first in the list of nodes)
			m_timeWindows = m_regNodes.size() + 1;    				// there should n+1 timewindows
			m_txTimeWindow = 0;  									// tx timewindow of this node
			m_currTimeWindow = m_txTimeWindow;
			m_timeMovedTimeWindow = millis();
		}

		size_t sendPacketWithTDMAHeader(std::vector<uint8_t> &packet, PACKET_TYPE packettype, uint8_t destinationNode){
			std::vector<uint8_t> TDMAHeader = {static_cast<uint8_t>(packettype), static_cast<uint8_t>(m_regNodes.size()), 
				m_currTimeWindow, static_cast<uint8_t>(m_networkManager.getAddress()), 
				static_cast<uint8_t>(destinationNode), static_cast<uint8_t>(255)};
			packet.insert(packet.begin(), TDMAHeader.begin(), TDMAHeader.end());
			return (m_physicalLayer.sendPacket(packet));
		}

		size_t sendPacketWithTDMAHeader(std::vector<uint8_t> &packet, PACKET_TYPE packettype, uint8_t destinationNode, uint8_t info){
			std::vector<uint8_t> TDMAHeader = {static_cast<uint8_t>(packettype), static_cast<uint8_t>(m_regNodes.size()), 
				m_currTimeWindow, static_cast<uint8_t>(m_networkManager.getAddress()), 
				static_cast<uint8_t>(destinationNode), info};
			packet.insert(packet.begin(), TDMAHeader.begin(), TDMAHeader.end());
			return (m_physicalLayer.sendPacket(packet));
		}

		void unpackTDMAHeader(std::vector<uint8_t> &packet){
		    uint8_t initial_size = packet.size();
			auto it = packet.begin();

			m_lastPacketSize = initial_size;

			m_lastPacketType = static_cast<PACKET_TYPE>(*it++);
			m_lastPacketRegNodes     = *it++;
			m_lastPacketTimeWindow   = *it++;
			m_lastPacketSource       = *it++;
			m_lastPacketDest         = *it++;
		
			if (*it != 255) {
				m_lastPacketInfo = *it;
			}
			++it;
		
			packet.erase(packet.begin(), it);
		
			if (initial_size - static_cast<uint8_t>(packet.size()) != m_tdmaHeaderSize) {
				throw std::runtime_error("size mismatch");
			}
		
		};

		std::queue<std::vector<uint8_t>> m_sendBuffer; 

		uint8_t m_timeWindows;
		uint8_t m_currTimeWindow;
		uint8_t m_txTimeWindow;
		
		std::vector<uint8_t> m_regNodes;

		uint32_t m_timeMovedTimeWindow = 0;
		uint32_t m_timeWindowLength;
		uint32_t m_timeLastPacketReceived;
		static constexpr uint32_t m_discoveryTimeout = 10e3;
		static constexpr uint32_t m_joinRequestTimeout = 5e3;
		uint32_t m_timeEnteredDiscovery;
		uint32_t m_timeJoinRequestSent = 0;

		uint8_t m_countsNoAck = 0;
		static constexpr uint8_t m_maxCountsNoAck = 2;
		uint8_t m_countsNoTx = 0;
		static constexpr uint8_t m_maxCountsNoTx = 10;

		float m_joinDutyCycle = 0.5;

		bool m_packetSent = false;
		bool m_received = false;
		bool m_synced = false;
		bool m_txWindowDone;
		bool m_rxWindowDone;
		bool m_skippingTurn = false;

		TDMA_MODE m_currMode = TDMA_MODE::DISCOVERY;

		DISCOVERY_PHASE m_currDiscoveryPhase = DISCOVERY_PHASE::ENTRY;

		uint8_t m_tdmaHeaderSize = 6;

		uint8_t m_lastPacketSource;
		uint8_t m_lastPacketDest;
		uint8_t m_lastPacketRegNodes;
		uint8_t m_lastPacketTimeWindow;
		uint8_t m_lastPacketInfo;
		PACKET_TYPE m_lastPacketType;
		size_t m_lastPacketSize;

		TDMARadioInterfaceInfo m_info;
		PhysicalLayer& m_physicalLayer;
		RnpNetworkManager& m_networkManager;

};