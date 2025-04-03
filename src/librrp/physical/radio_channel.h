#pragma once

#include <vector>
#include <functional>
#include <mutex>
#include <chrono>
#include <thread>
#include <algorithm>

class RadioChannel {
public:
    using ReceiveCallback = std::function<void(const std::vector<uint8_t>&)>;

    RadioChannel();

    void transmitPacket(const std::vector<uint8_t>& data, uint32_t airtimeMs, void* senderId);

    void registerReceiver(void* receiverId, ReceiveCallback callback);
    void unregisterReceiver(void* receiverId);

    bool isBusy() const;

private:

	struct Receiver {
		void* receiverId;
		ReceiveCallback callback;
	};

    mutable std::mutex mtx;

    bool m_busy = false;
	bool m_collisionDetected = false;

    std::chrono::steady_clock::time_point m_busyUntil;
    std::vector<Receiver> m_receivers;

	float m_packetDropProbability = 0.0;
};

