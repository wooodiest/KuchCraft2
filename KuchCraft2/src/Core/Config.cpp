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
					rendererConfig.Logs               = json["Renderer"]["Logs"].get<bool>();
					rendererConfig.ShaderVersion      = json["Renderer"]["ShaderVersion"].get<std::string>();
					rendererConfig.Renderer2DMaxQuads = json["Renderer"]["Renderer2DMaxQuads"].get<uint32_t>();
					rendererConfig.Renderer3DMaxQuads = json["Renderer"]["Renderer3DMaxQuads"].get<uint32_t>();
					rendererConfig.BlockTextureSize   = json["Renderer"]["BlockTextureSize"].get<uint32_t>();
					s_RendererConfig = rendererConfig;

					WorldConfigData worldConfig;
					worldConfig.WorldsDirectory        = json["World"]["WorldsDirectory"].get<std::string>();
					worldConfig.WorldDataFile          = json["World"]["WorldDataFile"].get<std::string>();
					worldConfig.TexturePackFile        = json["World"]["TexturePackFile"].get<std::string>();
					worldConfig.TexturesDirectory      = json["World"]["TexturesDirectory"].get<std::string>();
					worldConfig.RenderDistance         = json["World"]["RenderDistance"].get<uint32_t>();
					worldConfig.KeptInMemoryDistance   = json["World"]["KeptInMemoryDistance"].get<uint32_t>();
					worldConfig.ChunksToBuildInFrame   = json["World"]["ChunksToBuildInFrame"].get<uint32_t>();
					worldConfig.ChuksToRecreateInFrame = json["World"]["ChuksToRecreateInFrame"].get<uint32_t>();
					worldConfig.DurationOfDayInMinutes = json["World"]["DurationOfDayInMinutes"].get<uint32_t>();
					s_WorldConfig = worldConfig;
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
			{ "Logs",               s_RendererConfig.Logs },
			{ "ShaderVersion",      s_RendererConfig.ShaderVersion },
			{ "Renderer2DMaxQuads", s_RendererConfig.Renderer2DMaxQuads },
			{ "Renderer3DMaxQuads", s_RendererConfig.Renderer3DMaxQuads },
			{ "BlockTextureSize",   s_RendererConfig.BlockTextureSize }
		};

		json["World"] = {
			{ "WorldsDirectory",        s_WorldConfig.WorldsDirectory },
			{ "WorldDataFile",          s_WorldConfig.WorldDataFile },
			{ "TexturePackFile",        s_WorldConfig.TexturePackFile },
			{ "TexturesDirectory",      s_WorldConfig.TexturesDirectory },
			{ "RenderDistance",         s_WorldConfig.RenderDistance },
			{ "KeptInMemoryDistance",   s_WorldConfig.KeptInMemoryDistance },
			{ "ChunksToBuildInFrame",   s_WorldConfig.ChunksToBuildInFrame },
			{ "ChuksToRecreateInFrame", s_WorldConfig.ChuksToRecreateInFrame },
			{ "DurationOfDayInMinutes", s_WorldConfig.DurationOfDayInMinutes }
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
