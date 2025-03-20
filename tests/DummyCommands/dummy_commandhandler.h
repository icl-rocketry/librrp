#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <librnp/rnp_networkservice.h>
#include <librnp/rnp_packet.h>

template <typename COMMAND_ID_ENUM>
class DummyCommandHandler : public RnpNetworkService
{
    static_assert(std::is_enum_v<COMMAND_ID_ENUM>, "COMMAND_ID_ENUM template parameter is not an enum!");

public:
    using commandFunction_t = std::function<void(const RnpPacketSerialized&)>;
    using commandMap_t = std::unordered_map<COMMAND_ID_ENUM, commandFunction_t>;

    DummyCommandHandler(uint8_t ServiceID, commandMap_t commandMap) 
        : RnpNetworkService(ServiceID), m_commandMap(std::move(commandMap)) {}

private:
    const commandMap_t m_commandMap;

    void handleCommand(packetptr_t packetptr) {
        auto cmd = CommandPacket::getCommand(*packetptr);
        if (m_commandMap.count(static_cast<COMMAND_ID_ENUM>(cmd))) {
            m_commandMap.at(static_cast<COMMAND_ID_ENUM>(cmd))(*packetptr);
        }
    }

    void networkCallback(packetptr_t packetptr) override {
        handleCommand(std::move(packetptr));
    }
};
