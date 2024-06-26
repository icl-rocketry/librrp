#include "tdma.h"
#include <cmath>
#include <libriccore/riccorelogging.h>

template <typename Transport>
TDMA<Transport>::TDMA(Transport &transport, RnpNetworkManager& networkmanager, uint8_t id, std::string name, bool sim):
RnpInterface(id,name),
_config(defaultConfig),
_transport(transport),
_currentSendBufferSize(0),
_txDone(true),
_received(false),
_networkmanager(networkmanager),
_sim(sim)
{
    _info.MTU = 256;
    _info.sendBufferSize = 2048;
    _info.mode=currentMode;
};

template <typename Transport>
void TDMA<Transport>::setup(){
    
    if (!_transport.setup()){
        // _systemstatus.newFlag(SYSTEM_FLAG::ERROR_TDMA);
    }
    else{
        calcTimewindowDuration();
        registeredNodes.push_back(_networkmanager.getAddress());    // add own address to list
        timewindows = registeredNodes.size() + 1;                   // n+1 timewindows
        timeMovedTimewindow = millis();
    }
};

template <typename Transport>
void TDMA<Transport>::update(){

    if (millis() - (timeMovedTimewindow + networkTimeShift) >= timewindowDuration){
        currTimewindow = (currTimewindow + 1) % timewindows;
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Number of registered nodes: " + std::to_string(registeredNodes.size()));
        timeMovedTimewindow = millis();

        packetSent = false;
        txWindowDone = false;
        rxWindowDone = false;
    }

    if (millis() - pTime > 500)
    {
        std::string nodes;
        for (auto n :registeredNodes)
        {
            nodes += (std::to_string(n) + ","); 
        }
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Nodes: " + nodes);
        pTime = millis();
    }

    if (currentMode == TDMA_MODE::DISCOVERY){
        discovery();
    }
    else{   

        if(currTimewindow == txTimewindow){
            currentMode = TDMA_MODE::TRANSMIT;
            if(!txWindowDone){
                tx();
            }
        }
        else{
            currentMode = TDMA_MODE::RECEIVE;
            if(!rxWindowDone){
                rx();
            }
        }
    }
};

template <typename Transport>
void TDMA<Transport>::discovery(){

    std::vector<uint8_t> joinRequest(tdmaHeaderSize);   // variable declaration apparently needs to be outside switch case :(

    switch(currentDiscoveryPhase) {


        case DISCOVERY_PHASE::ENTRY: {
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Entered Discovery");
            timeEnteredDiscovery = millis();                    // timestamp entry
            currentDiscoveryPhase = DISCOVERY_PHASE::SNIFFING;  // transition to next phase
            break;
        }

        case DISCOVERY_PHASE::SNIFFING: {
            getPacket();        // scan for packets

            if (millis() - timeEnteredDiscovery > discoveryTimeout){
                currentDiscoveryPhase = DISCOVERY_PHASE::INIT_NETWORK;  // transition to network initialisation
            }

            if (_received){
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Network detected");
                currentDiscoveryPhase = DISCOVERY_PHASE::SYNCING;       // transition to network syncing
            }
            break;
        }
        

        case DISCOVERY_PHASE::INIT_NETWORK: {
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Initialising network");
            initNetwork();
            currentDiscoveryPhase = DISCOVERY_PHASE::EXIT;              // transition to exit phase
            break;
        }


        case DISCOVERY_PHASE::SYNCING: {
            sync();
            currentDiscoveryPhase = DISCOVERY_PHASE::JOIN_REQUEST;      // transition to requesting to join
            break;
        }


        case DISCOVERY_PHASE::JOIN_REQUEST: {
            if(currTimewindow == txTimewindow){

                generateTDMAHeader(joinRequest, PacketType::JOINREQUEST, packetSource);
                if(_transport.sendPacket(joinRequest) > 0){                                      // bytes written successfully
                    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Join request sent");
                    timeJoinRequestSent = millis();
                    currentDiscoveryPhase = DISCOVERY_PHASE::JOIN_REQUEST_RESPONSE; // transition to waiting for response
                }
            } 
            break;
        }


        case DISCOVERY_PHASE::JOIN_REQUEST_RESPONSE: {

            getPacket();        // scan for response

            if (_received){
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Received something: " + std::to_string(receivedPacketType) + ", " + std::to_string(packetDest) + ", " + std::to_string(_networkmanager.getAddress()));
            }

            if(_received && receivedPacketType == PacketType::ACK && packetDest == _networkmanager.getAddress()){
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Joined network");

                try{
                    updateRegisteredNodes(_networkmanager.getAddress());
                }catch (std::exception& e){
                    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR updating reg nodes list: " + std::string(e.what()));
                }
                try {
                    updateRegisteredNodes(packetSource);
                }catch (std::exception& e){
                    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR updating reg nodes list: " + std::string(e.what()));
                }
                //registeredNodes.push_back(_networkmanager.getAddress()); // add self to list
                //registeredNodes.push_back(packetDest);       // maybe should not add to the registered nodes list
                timewindows = packetRegNodes + 1;   // set local number of timewindows to match network
                currentDiscoveryPhase = DISCOVERY_PHASE::EXIT;
            }

            else if(_received && receivedPacketType == PacketType::NACK && packetDest == _networkmanager.getAddress()){  
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Node joined before");
                txTimewindow = packetInfo;
                try{
                    updateRegisteredNodes(_networkmanager.getAddress());
                }catch (std::exception& e){
                    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR updating reg nodes list: " + std::string(e.what()));
                }
                try {
                    updateRegisteredNodes(packetSource);
                }catch (std::exception& e){
                    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR updating reg nodes list: " + std::string(e.what()));
                }
                //registeredNodes.push_back(_networkmanager.getAddress()); // add self to list
                //registeredNodes.push_back(packetDest);       // maybe should not add to the registered nodes list
                timewindows = packetRegNodes + 1;
                currentDiscoveryPhase = DISCOVERY_PHASE::EXIT;
            }

            if (millis() - timeJoinRequestSent > joinRequestExpiryTime || (_received && receivedPacketType == PacketType::HEARTBEAT)){       // request expired
                currentDiscoveryPhase = DISCOVERY_PHASE::JOIN_REQUEST;  // try again
            }
            break;
        }

        
        case DISCOVERY_PHASE::EXIT: {
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Exiting discovery");
            currentMode = TDMA_MODE::TRANSMIT;      // to exit out of discovery, assign any other mode other than discovery
            break;
        }


        default: {
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("ERROR: Unknown discovery phase");
            break;
        }

    }

}

template <typename Transport>
void TDMA<Transport>::sync(){
    networkTimeShift = timeMovedTimewindow - (timePacketReceived - static_cast<uint64_t>(calcPacketToF(packetSize)*1e6f));   //resyncing
    
    currTimewindow = packetTimewindow;    // sync local current timewindow to network
    txTimewindow = packetRegNodes;        // send in the n+1th timewindow
    timewindows = packetRegNodes+1;       // update local number of timewindows
    synced = true;                        // syncing complete
}

template <typename Transport>
void TDMA<Transport>::initNetwork(){
    timewindows = registeredNodes.size() + 1;    //n+1 timewindows
    txTimewindow = 0;  //of this node
    currTimewindow = txTimewindow;
    timeMovedTimewindow = millis();
}

template <typename Transport>
void TDMA<Transport>::generateTDMAHeader(std::vector<uint8_t> &TDMAHeader, PacketType packettype, uint8_t destinationNode){
    TDMAHeader = {static_cast<uint8_t>(packettype), static_cast<uint8_t>(registeredNodes.size()), 
                    currTimewindow, static_cast<uint8_t>(_networkmanager.getAddress()), 
                    static_cast<uint8_t>(destinationNode), static_cast<uint8_t>(255)};
}

template <typename Transport>
void TDMA<Transport>::generateTDMAHeader(std::vector<uint8_t> &TDMAHeader, PacketType packettype, uint8_t destinationNode, uint8_t info){
    TDMAHeader = {static_cast<uint8_t>(packettype), static_cast<uint8_t>(registeredNodes.size()), 
                    currTimewindow, static_cast<uint8_t>(_networkmanager.getAddress()), 
                    static_cast<uint8_t>(destinationNode), info};
}

template <typename Transport>
void TDMA<Transport>::unpackTDMAHeader(std::vector<uint8_t> &packet){
    uint8_t initial_size = packet.size();

    //! This is kinda slow maybe to an inital copy first?

    receivedPacketType = static_cast<PacketType>(packet.front());
    packet.erase(packet.begin());

    packetRegNodes = packet.front();
    packet.erase(packet.begin());

    packetTimewindow = packet.front();
    packet.erase(packet.begin());

    packetSource = packet.front();
    packet.erase(packet.begin());

    packetDest = packet.front();
    packet.erase(packet.begin());

    if(packet.front() != 255){
        packetInfo = packet.front(); // default value?
    }
    packet.erase(packet.begin());

    if (initial_size - static_cast<uint8_t>(packet.size()) != tdmaHeaderSize){
        throw std::runtime_error("Error unpacking TDMA header");
    }

}

template <typename Transport>
void TDMA<Transport>::tx(){

    if(_sendBuffer.size() > 0){    //buffer not empty

        if(countsNoAck > maxCountsNoAck){  // check if exceed limit on resends
            uint8_t popped_packet_size = sizeof(_sendBuffer.back());
            _sendBuffer.pop();      // pop old packet
            _currentSendBufferSize -= popped_packet_size;
            countsNoAck = 0;      // reset counter#
            txWindowDone = true;
        }
        if(!packetSent){
            uint8_t bytes_written = _transport.sendPacket(_sendBuffer.front());  // send from front of buffer -> sets packetSent to true?
            if (bytes_written){
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("RNP packet sent");
                countsNoAck ++;  // just trust me bro, it makes sense
                countsNoTx = 0; 
            }
        }
        else{                   // packet has been sent
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Waiting for Ack");
            getPacket();        // scan for acks
            if (_received)
            {
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("received something: " +  std::to_string(receivedPacketType));
            }
            if (_received && receivedPacketType == PacketType::ACK){ // packet got acked
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("got Ack");
                uint8_t popped_packet_size = sizeof(_sendBuffer.back());
                _sendBuffer.pop();      // remove packet from send queue
                _currentSendBufferSize -= popped_packet_size;
                countsNoAck = 0;      // reset counter
                txWindowDone = true;   // exit tx mode
            }
        } 

    }
    else{                           // buffer empty

        if (countsNoTx >= maxCountsNoTx){        // node didn't transmit in a long time
            std::vector<uint8_t> heartbeatPacket(tdmaHeaderSize);
            generateTDMAHeader(heartbeatPacket, PacketType::HEARTBEAT, 0);
            _transport.sendPacket(heartbeatPacket);  // send skipping buffer
            countsNoTx = 0;
            txWindowDone = true;
        }
        else{ 
            countsNoTx ++;             // update counter
            txWindowDone = true;       // exit tx mode
        }

    }

}

template <typename Transport>
void TDMA<Transport>::rx(){

    _transport.receiveBytes();    // scan for packets

    if(_received){
        networkTimeShift = timeMovedTimewindow - (timePacketReceived - static_cast<uint64_t>(calcPacketToF(packetSize)*1e6f));   //resyncing
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(std::to_string(static_cast<uint64_t>(calcPacketToF(packetSize)*1e6f)) + " vs " + std::to_string(timewindowDuration));

        std::vector<uint8_t> ackPacket(tdmaHeaderSize);     // initialising ack packet

        switch (receivedPacketType) {

            case PacketType::JOINREQUEST: {                             // handling join request
                
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Received join request");

                auto it = find(registeredNodes.begin(), registeredNodes.end(), packetSource);

                if(it == registeredNodes.end()){                        // node has not been registered yet
                    registeredNodes.push_back(packetSource);            // add to node list
                    timewindows = registeredNodes.size()+1;             // update number of timewindows
     
                    generateTDMAHeader(ackPacket, PacketType::ACK, packetSource);     //ack join request
                    _transport.sendPacket(ackPacket);            
                    rxWindowDone = true;
                }
                else{                                                   // node has already registered
                    uint8_t requesterTxTimewindow = static_cast<uint8_t>(it-registeredNodes.begin());
                    generateTDMAHeader(ackPacket, PacketType::NACK, packetSource, requesterTxTimewindow); // nack join request
                    _transport.sendPacket(ackPacket);            
                    rxWindowDone = true;               
                }
                break;
            }
                
            
            case PacketType::NORMAL: {                                  // handling RNP packet

                generateTDMAHeader(ackPacket, PacketType::ACK, packetSource);
                _transport.sendPacket(ackPacket);
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Radio receive");
                rxWindowDone = true; 
                break; 
            }

            case PacketType::HEARTBEAT: {                               // handling heartbeat packet
                rxWindowDone = true;
                break;
            }

            default: {                                                  // handling other packet
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Received unexpected packet type: " + std::to_string(receivedPacketType));
                rxWindowDone = true;
                break;
            }

        }
    }
}

template <typename Transport>
void TDMA<Transport>::calcTimewindowDuration(){
    float maxTframe = 2;                 //assuming 2 seconds
    float clock_drift = 2E-5;            //s/s
    float Tg = maxTframe*clock_drift;    //guard time   
    float ff = 1.1;                        //magic fudge factor
    timewindowDuration = ff*(calcPacketToF(_config.max_payload_length) + calcPacketToF(_config.max_ack_length) + Tg)*1e3f;  // in ms
}

template <typename Transport>
float TDMA<Transport>::calcPacketToF(int Lpayload){
    float Rs = _config.bandwidth/std::pow(2,_config.spreading_factor);
    float CR = _config.cr_denominator - 4;
    float Tsym = 1/Rs;
    float Tpreamble = (_config.preamble_length + 4.25)*Tsym;
    float Tpayload = Tsym*(8 + ceil((8*Lpayload - 4*_config.spreading_factor + 44)/(4*_config.spreading_factor))*(CR + 4));
    return Tpreamble + Tpayload;
}

template <typename Transport>
void TDMA<Transport>::sendPacket(RnpPacket& data)
{
    const size_t dataSize = data.header.size() + data.header.packet_len;
    //! DATA SIZE NOT updated with tdma header length

    if (dataSize > _info.MTU){ // will implement packet segmentation here at a later data
        ++_info.txerror;
        return;
    }
    if (dataSize + _currentSendBufferSize > _info.sendBufferSize){
        // _systemstatus.newFlag(SYSTEM_FLAG::ERROR_LORA," Lora Send Buffer Overflow!");
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Lora Send Buffer Overflow!");
        ++_info.txerror;
        _info.sendBufferOverflow = true;
        return;
    }

    std::unique_ptr<std::vector<uint8_t>> serializedPacket = std::make_unique<std::vector<uint8_t>>();
    data.serialize(*serializedPacket);
    std::vector<uint8_t> tdmaHeader(tdmaHeaderSize);
    generateTDMAHeader(tdmaHeader, PacketType::NORMAL, 0);  //TODO:: fix this
    serializedPacket->insert(serializedPacket->begin(), tdmaHeader.begin(), tdmaHeader.end());
    _sendBuffer.push(*serializedPacket);
    _info.sendBufferOverflow = false;
    _currentSendBufferSize += dataSize;

}

template <typename Transport>
void TDMA<Transport>::updateRegisteredNodes(uint8_t rnp_node){
    auto it = find(registeredNodes.begin(), registeredNodes.end(), rnp_node);
    if (it != registeredNodes.end()){
        throw std::runtime_error("RNP node " + std::to_string(rnp_node) + " already exists in list");
        return;
    }
    else{
        registeredNodes.push_back(rnp_node);
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("RNP node " + std::to_string(rnp_node) + " added to list");
    }
}

template <typename Transport>
void TDMA<Transport>::getPacket(){

    size_t packetSize = _transport.readPacket();

    if (_transport.readPacket() > 0){
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Radio receive");
        _received=true; 

        timePacketReceived = millis();
        std::vector<uint8_t> data(packetSize); 
        _transport.readPacket(data.data());

        std::string bytestring;
        for (auto e : data)
        {
            bytestring += e;
        }

        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("IPacket Size: " + std::to_string(data.size()) );
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("IPacket: " + bytestring );

        //unpacking TDMA header
        try{
            unpackTDMAHeader(data); // modifies data vector
        }
        catch (std::exception& e)
        {
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("TDMA Error: " + std::string(e.what()));
            return;
        }

        bytestring.clear();
        for (auto e : data)
        {
            bytestring += e;
        }
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Packet Size: " + std::to_string(data.size()) );
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Packet: " + bytestring );

        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(std::to_string(receivedPacketType));      

        if (data.size() > 0){     // packet still has smthing left in it

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

            // useful rnp header vals
            packetSource = packet_ptr->header.source;
            packetDest = packet_ptr->header.destination;

            if (packetDest == _networkmanager.getAddress()){
                packetForMe = true;
            }
            else{
                packetForMe = false;
            }

            //update source interface
            packet_ptr->header.src_iface = getID();
            _packetBuffer->push(std::move(packet_ptr));//add packet ptr  to buffer
        }
    }
}

template <typename Transport>
const RnpInterfaceInfo* TDMA<Transport>::getInfo()
{
    return _transport.getInfo();
};

template <typename Transport>
const RadioConfig& TDMA<Transport>::getConfig(){return _config;};

template <typename Transport>
void TDMA<Transport>::setConfig(RadioConfig config)
{
    _config = config;
    _transport.setConfig(_config);
}
