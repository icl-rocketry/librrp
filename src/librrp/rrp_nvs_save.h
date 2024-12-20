#pragma once

#include <string>
#include<type_traits>

#if (defined ESP32 && defined ARDUINO)
#include <Preferences.h>

namespace RrpNvsSave {

    /**
     * @brief Save value to NVS
     *
     * @param[in] namespaceName
     * @param[in] key
     * @param[in] value
     */
    template <typename DataType>
    static bool SaveValueToNVS(const std::string& namespaceName, const std::string& key, DataType value) {
        Preferences pref;
        
        if (!pref.begin(namespaceName.c_str()){
            return false;
        }

        save(pref, key, value);

        pref.end();

        return true;
    };

    /**
     * @brief Read value from NVS
     *
     * @param[in] namespaceName
     * @param[in] key
     * @param[in] value
     */
    static bool ReadValueFromNVS(const std::string& namespaceName, const std::string& key, DataType value) {
        Preferences pref;

        // Return error if preferences does not start with Rnp_Config
        if (!pref.begin(namespaceName.c_str())) {
            return false;
        }

        read(pref, key, value);

        pref.end();

        return true;
    };

    private:
        template <typename DataType>
        void save(Preferences& pref, const std::string& key, DataType value){
            if constexpr (std::is_same_v<T, int>) {
                pref.putInt(key.c_str(), value);
            } else if constexpr (std::is_same_v<T, long>) {
                pref.putLong(key.c_str(), value);
            } else if constexpr (std::is_same_v<T, bool>) {
                pref.putBool(key.c_str(), value);
            } else if constexpr (std::is_same_v<T, std::string>) {
                pref.putString(key.c_str(), value);
            }
        }

        template <typename DataType>
        void read(Preferences& pref, const std::string& key, DataType& value){
            if constexpr (std::is_same_v<T, int>) {
                value = pref.getInt(key.c_str());
            } else if constexpr (std::is_same_v<T, long>) {
                value = pref.getLong(key.c_str());
            } else if constexpr (std::is_same_v<T, bool>) {
                value = pref.getBool(key.c_str());
            } else if constexpr (std::is_same_v<T, std::string>) {
                value = pref.getString(key.c_str());
            }
        }
}; // namespace RnpNvsSave

#else

#include <iostream>

namespace RrpNvsSave {

    /**
     * @brief Save value to NVS
     *
     * @param[in] namespaceName
     * @param[in] key
     * @param[in] value
     */
    template <typename DataType>
    static bool SaveValueToNVS(const std::string& namespaceName, const std::string& key, DataType value) {
        // i dont know if there is actually any point implementing this for unix,
        // it just needs to compile
        return true;
    };

    /**
     * @brief Read value from NVS
     *
     * @param[in] namespaceName
     * @param[in] key
     * @param[in] value
     */
    template <typename DataType>
    static bool ReadValueFromNVS(const std::string& namespaceName, const std::string& key, DataType value) {
        return false;   // go to default config value
    };

}; // namespace RrpNvsSave

#endif