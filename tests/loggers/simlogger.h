#pragma once

#include <libriccore/logging/loggers/loggerbase.h>

#include <iostream>
#include <string>
#include <libriccore/platform/millis.h>
#include <thread>
#include <chrono>


class SimLogger : public LoggerBase
{
public:
    SimLogger(const std::string_view name) : LoggerBase(),
                                              logger_name(name){
                                              };

    void log(std::string_view msg)
    {   
        if (!enabled){return;};
        
        #ifdef ARDUINO
        Serial.println((std::string(logger_name) + ":[" + std::to_string(millis()).c_str() + "] -> "  + std::string(msg)).c_str());
        #else
        std::cout << logger_name << ":{" << std::this_thread::get_id() << "}" << ":[" << std::to_string(trueMillis()) << "]" << ":[" << std::to_string(millis()) << "] -> " << msg << "\n";
        #endif
    };

    void log(uint32_t status,uint32_t flag,std::string_view message)
    {
        std::string log_str = std::to_string(flag) + "," + std::string(message) + "," + std::to_string(status);
        log(log_str);
    };

    
    ~SimLogger(){};

private:
    const std::string logger_name;
	uint32_t trueMillis(){
		auto elapsed = std::chrono::steady_clock::now() - programStartTime;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		return static_cast<uint32_t>(static_cast<double>(ms));
	}
};