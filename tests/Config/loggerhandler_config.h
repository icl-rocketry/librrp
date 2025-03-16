#pragma once
#include <tuple>
#include "../loggers/simlogger.h"


namespace RicCoreLoggingConfig
{
    enum class LOGGERS
    {
        SYS, // default system logging
    };

    inline std::tuple logger_list =
        {
            SimLogger("SIM_LOG")
        };
};