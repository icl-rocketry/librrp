#pragma once

#include "radio_channel.h"
#include <map>
#include <memory>

class RadioChannelManager {
public:
    std::shared_ptr<RadioChannel> getChannel(int channelId);

    void registerNode(int channelId, void* nodeId, RadioChannel::ReceiveCallback callback);
    void unregisterNode(int channelId, void* nodeId);

private:
    std::map<int, std::shared_ptr<RadioChannel>> channels;
};
