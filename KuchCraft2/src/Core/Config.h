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

#include "World/InGameTime.h"

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

        /// The X position of the window in windowed mode
        int PositionX = 200;

        /// The Y position of the window in windowed mode
        int PositionY = 200;

        /// The X position of the window before switching to fullscreen mode.
        int PositionBeforeFullscreenX = 0;

        /// The Y position of the window before switching to fullscreen mode.
        int PositionBeforeFullscreenY = 0;

        /// The width of the window before switching to fullscreen mode.
        int WidthBeforeFullscreen = 0;

        /// The height of the window before switching to fullscreen mode.
        int HeightBeforeFullscreen = 0;

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

    struct RendererConfigData
    {
        /// Flag to enable or disable rendering-related log messages.
        bool Logs = true;

        /// Total maximum number of texture slots available in the GPU.
        uint32_t MaxCombinedTextureSlots = 64;

        /// Maximum number of texture slots available in the GPU per shader
        uint32_t MaxTextureSlots = 32;

        /// Maximum number of 2D quads that can be rendered in a single batch.
        uint32_t Renderer2DMaxQuads = 20'000;

        /// Maximum number of 3D quads that can be rendered in a single batch.
        uint32_t Renderer3DMaxQuads = 20'000;

        /// GLSL shader version string to use in shader source files.
        std::string ShaderVersion = "460 core";

        /// Size of texture block
        uint32_t BlockTextureSize = 16;

        /// Skybox colors for different times of day, represented as a map.
        /// The keys are time periods (Dawn, Morning, Noon, etc.), and values are RGBA colors.
        std::map<TimeOfDay, glm::vec4> SkyboxColor = {
            { TimeOfDay::Dawn,      glm::vec4(0.8f, 0.4f, 0.1f, 1.0f) },
            { TimeOfDay::Morning,   glm::vec4(0.9f, 0.6f, 0.3f, 1.0f) },
            { TimeOfDay::Noon,      glm::vec4(0.6f, 0.8f, 1.0f, 1.0f) },
            { TimeOfDay::Afternoon, glm::vec4(0.7f, 0.7f, 1.0f, 1.0f) },
            { TimeOfDay::Evening,   glm::vec4(0.8f, 0.5f, 0.3f, 1.0f) },
            { TimeOfDay::Dusk,      glm::vec4(0.3f, 0.1f, 0.3f, 1.0f) },
            { TimeOfDay::Night,     glm::vec4(0.1f, 0.1f, 0.3f, 1.0f) }
		};  
    };

    /// Struct to hold world configuration data.
    struct WorldConfigData
    {
        /// In this directory worlds will be serialized and deserialized
        std::string WorldsDirectory = "worlds";

        /// Main world entities file
        std::string WorldDataFile = "world_data.kc";

        /// Item description info file
        std::string TexturePackFile = "itemInfo.kc";

        /// Biome description info file
        std::string BiomePackFile = "biomeInfo.kc";

        /// World generator description info file
        std::string WorldGeneratorPackFile = "worldGenerator.kc";

        /// Texture directory
        std::string TexturesDirectory = "assets/textures";

        /// Radius od maximum number of chunks to be visible
        uint32_t RenderDistance = 5;

        /// Radius of maximum number of chunks to be kept in memory
        uint32_t KeptInMemoryDistance = 10;

        /// Number of chunks to be build in single frame
        uint32_t ChunksToBuildInFrame = 1;

        /// Number of chunks to be recreated in single frame
        uint32_t ChuksToRecreateInFrame = 1;

		/// The duration of the day in minutes
        uint32_t DurationOfDayInMinutes = 20;
    };

    class ApplicationConfig
    {
    public:
        /// Initializes the application configuration
        static void Init();

        /// Saves application configuration
        static void Save();

        /// Retrieves log configuration data.
        /// @return Reference to the log configuration data.
        static inline [[nodiscard]] LogConfigData& GetLogData() noexcept { return s_LogConfig; }

        /// Retrieves window configuration data.
        /// @return Reference to the window configuration data.
        static inline [[nodiscard]] WindowConfigData& GetWindowData() noexcept { return s_WindowConfig; }

        /// Retrieves renderer configuration data.
        /// @return Reference to the renderer configuration data.
        static inline [[nodiscard]] RendererConfigData& GetRendererData() noexcept { return s_RendererConfig; }

        /// Retrieves world configuration data.
        /// @return Reference to the world configuration data.
        static inline [[nodiscard]] WorldConfigData& GetWorldData() noexcept { return s_WorldConfig; }

    private:
        /// Log configuration data.
        static inline LogConfigData s_LogConfig;

        /// Window configuration data.
        static inline WindowConfigData s_WindowConfig;

        /// Window configuration data.
        static inline RendererConfigData s_RendererConfig;

        /// World configuration data.
        static inline WorldConfigData s_WorldConfig;

    };
}
