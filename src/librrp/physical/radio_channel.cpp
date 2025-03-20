#include "radio_channel.h"
#include <libriccore/riccorelogging.h>

RadioChannel::RadioChannel() {}

void RadioChannel::transmitPacket(const std::vector<uint8_t>& data, uint32_t airtimeMs, void* senderId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto now = std::chrono::steady_clock::now();

    if (m_busy && now < m_busyUntil) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("RadioChannel: Packet collision — channel is busy");
		m_collisionDetected = true;
        return;
    }

	// Start transmission
    m_busy = true;
    m_busyUntil = now + std::chrono::milliseconds(airtimeMs);

    std::thread([this, data, airtimeMs, senderId]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(airtimeMs));

		std::lock_guard<std::mutex> lock(mtx);

		if (m_collisionDetected){
			RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("RadioChannel: Packet collison - packet already on air getting dropped");
		} else{
        	// Deliver the packet to all registered receivers except the sender itself
			for (const auto& receiver : m_receivers) {
				if (receiver.receiverId != senderId) {
					receiver.callback(data);
				}
			}
		}

        m_busy = false;
		m_collisionDetected = false;

    }).detach();
}

void RadioChannel::registerReceiver(void* receiverId, ReceiveCallback callback) {
    std::lock_guard<std::mutex> lock(mtx);
    m_receivers.push_back({receiverId, callback});
}

void RadioChannel::unregisterReceiver(void* receiverId) {
    std::lock_guard<std::mutex> lock(mtx);

    // Remove the receiver from the list
    m_receivers.erase(
        std::remove_if(m_receivers.begin(), m_receivers.end(),
            [receiverId](const Receiver& receiver) {
                return receiver.receiverId == receiverId;
            }
        ),
        m_receivers.end()
    );

}

bool RadioChannel::isBusy() const {
    std::lock_guard<std::mutex> lock(mtx);
    return m_busy && (std::chrono::steady_clock::now() < m_busyUntil);
}
