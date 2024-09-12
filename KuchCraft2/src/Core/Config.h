/// 
/// @file Config.h
/// 
/// @author Michal Kuchnicki 
/// 
/// @brief Header file containing the declaration of the ApplicationConfig class which stores
///        configuration settings and all of the declarations of the ...ConfigData for 
///        configuration which are simply divided into categories via structs.
/// 
/// @details The ApplicationConfig class is responsible for holding and retrieving configuration data.
///          ApplicationConfig should be initialized via the Init() function to load all configration data at the start
///          of the program. Otherwise it will have the default values ​​that are provided for each structure with their declaration
/// 
/// @thread_safety ApplicationConfig is not thread-safe.
/// 
/// @example
///         // Initializing the application configuration at the start of the application.
///         KuchCraft::ApplicationConfig::Init();
///         
///         // Getting example data   
///         loggerFilePath = KuchCraft::ApplicationConfig::GetLogData().File;
/// 
/// @future Add more configuration files such as audio, renderer, application, etc.
/// 

#pragma once

namespace KuchCraft {

    /// Path to the application configuration file.
    static inline constexpr const char* s_ConfigPath = "config.json";

    /// Struct to hold log configuration data.
    struct LogConfigData
    {
        /// Logger name
        std::string Name = "KuchCraft2";

        /// Log output file path
        std::string File = "KuchCraft.log";

        /// Log output pattern format
        std::string Pattern = "[%Y-%m-%d %H:%M:%S] [%l] : %v";

    };

    class ApplicationConfig
    {
    public:
        /// Initializes the application configuration
        static void Init();

        /// Retrieves log configuration data.
        /// @return Reference to the log configuration data.
        static inline [[nodiscard]] const LogConfigData& GetLogData() { return s_LogConfig; }

    private:
        /// Log configuration data.
        static inline LogConfigData s_LogConfig;

    };
}
