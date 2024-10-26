/// 
/// @file Config.cpp
/// 
/// @author Michal Kuchnicki 
/// 

#include "kcpch.h"

#include "Core/Config.h"
#include "Core/Application.h"

#include <json.hpp>

namespace KuchCraft {

	void ApplicationConfig::Init()
	{
		try
		{
			std::ifstream file(s_ConfigPath);

			if (file.is_open())
			{
				nlohmann::json json;
				file >> json;

				try
				{
					LogConfigData logsConfig;
					logsConfig.Name    = json["Logs"]["Name"].get<std::string>();
					logsConfig.File    = json["Logs"]["File"].get<std::string>();
					logsConfig.Pattern = json["Logs"]["Pattern"].get<std::string>();
					s_LogConfig = logsConfig;

					WindowConfigData windowConfig;
					windowConfig.Width                     = json["Window"]["Width"].get<int>();
					windowConfig.Height                    = json["Window"]["Height"].get<int>();
					windowConfig.PositionX                 = json["Window"]["PositionX"].get<int>();
					windowConfig.PositionY                 = json["Window"]["PositionY"].get<int>();
					windowConfig.PositionBeforeFullscreenX = json["Window"]["PositionBeforeFullscreenX"].get<int>();
					windowConfig.PositionBeforeFullscreenY = json["Window"]["PositionBeforeFullscreenY"].get<int>();
					windowConfig.WidthBeforeFullscreen     = json["Window"]["WidthBeforeFullscreen"].get<int>();
					windowConfig.HeightBeforeFullscreen    = json["Window"]["HeightBeforeFullscreen"].get<int>();
					windowConfig.Vsync                     = json["Window"]["Vsync"].get<bool>();
					windowConfig.Resizable                 = json["Window"]["Resizable"].get<bool>();
					windowConfig.FullScreen                = json["Window"]["FullScreen"].get<bool>();
					windowConfig.ShowCursor                = json["Window"]["ShowCursor"].get<bool>();
					s_WindowConfig = windowConfig;

					RendererConfigData rendererConfig;
					rendererConfig.Logs          = json["Renderer"]["Logs"].get<bool>();
					rendererConfig.ShaderVersion = json["Renderer"]["ShaderVersion"].get<std::string>();
					s_RendererConfig = rendererConfig;

				}
				catch (const std::exception& e)
				{
					/// Default values
					std::cout << e.what() << std::endl;
				}

				file.close();
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		
	}

	void ApplicationConfig::Save()
	{
		s_WindowConfig = Application::GetWindow().GetWindowData();
		nlohmann::json json;

		json["Logs"] = {
			{ "Name",    s_LogConfig.Name },
			{ "File",    s_LogConfig.File },
			{ "Pattern", s_LogConfig.Pattern }
		};

		json["Window"] = {
			{ "Width",                     s_WindowConfig.Width  > 0 ? s_WindowConfig.Width  : min_window_width  },
			{ "Height",                    s_WindowConfig.Height > 0 ? s_WindowConfig.Height : min_window_height },
			{ "PositionX",                 s_WindowConfig.PositionX >= 0 ? s_WindowConfig.PositionX : (int)(0.1f * min_window_width ) },
			{ "PositionY",                 s_WindowConfig.PositionY >= 0 ? s_WindowConfig.PositionY : (int)(0.1f * min_window_height) },
			{ "PositionBeforeFullscreenX", s_WindowConfig.PositionBeforeFullscreenX ? s_WindowConfig.PositionBeforeFullscreenX : (int)(0.1f * min_window_width ) },
			{ "PositionBeforeFullscreenY", s_WindowConfig.PositionBeforeFullscreenY ? s_WindowConfig.PositionBeforeFullscreenY : (int)(0.1f * min_window_height) },
			{ "WidthBeforeFullscreen",     s_WindowConfig.WidthBeforeFullscreen },
			{ "HeightBeforeFullscreen",    s_WindowConfig.HeightBeforeFullscreen },
			{ "Vsync",                     s_WindowConfig.Vsync },
			{ "Resizable",                 s_WindowConfig.Resizable },
			{ "FullScreen",                s_WindowConfig.FullScreen },
			{ "ShowCursor",                s_WindowConfig.ShowCursor }
		};

		json["Renderer"] = {
			{ "Logs",          s_RendererConfig.Logs },
			{ "ShaderVersion", s_RendererConfig.ShaderVersion }
		};

		std::ofstream file(s_ConfigPath);
		if (file.is_open())
		{
			file << json.dump(4);
			file.close();
		}
		else
			Log::Error("Could not open config file for saving");

	}
}
