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

    struct WindowConfigData
    {
        /// The title of the window.
        std::string Title = "KuchCraft2";

        /// The width of the window in pixels
        int Width = 1280;

        /// The height of the window in pixels.
        int Height = 720;

        /// Flag indicating whether vertical synchronization is enabled.
        bool Vsync = true;

        /// Flag indicating whether the window is resizable.
        bool Resizable = true;

        /// Flag indicating whether the window is in fullscreen mode.
        bool FullScreen = false;

        /// Flag indicating whether the cursor should be shown.
        bool ShowCursor = true;

    };

    /// Limits for window sizes
    constexpr inline int min_window_width  = 640;
    constexpr inline int min_window_height = 480;
    constexpr inline int max_window_width  = 7680;
    constexpr inline int max_window_height = 4320;

    /// Max value for delta time
    constexpr inline float max_delta_time = 1.0f / 60.0f;

    class ApplicationConfig
    {
    public:
        /// Initializes the application configuration
        static void Init();

        /// Retrieves log configuration data.
        /// @return Reference to the log configuration data.
        static inline [[nodiscard]] const LogConfigData& GetLogData() noexcept { return s_LogConfig; }

        /// Retrieves window configuration data.
        /// @return Reference to the window configuration data.
        static inline [[nodiscard]] const WindowConfigData& GetWindowData() noexcept { return s_WindowConfig; }

    private:
        /// Log configuration data.
        static inline LogConfigData s_LogConfig;

        /// Window configuration data.
        static inline WindowConfigData s_WindowConfig;

    };
}
