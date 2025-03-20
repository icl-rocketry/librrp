#include "radio_channel_manager.h"

std::shared_ptr<RadioChannel> RadioChannelManager::getChannel(int channelId) {
    if (channels.find(channelId) == channels.end()) {
        channels[channelId] = std::make_shared<RadioChannel>();
    }
    return channels[channelId];
}

void RadioChannelManager::registerNode(int channelId, void* nodeId, RadioChannel::ReceiveCallback callback) {
	getChannel(channelId)->registerReceiver(nodeId, callback);
}

void RadioChannelManager::unregisterNode(int channelId, void* nodeId) {
    if (channels.find(channelId) != channels.end()) {
        channels[channelId]->unregisterReceiver(nodeId);
    }
}
