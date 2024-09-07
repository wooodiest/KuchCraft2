/// 
/// @file Log.cpp
/// 
/// @author Michal Kuchnicki 
/// 

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Core/Log.h"

namespace KuchCraft {

	void Log::Init()
	{
		// Guard against multiple initializations
		if (!s_Logger)
		{
			// Create file sink
			s_FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("KuchCraft.log", true);
			s_FileSink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] : %v");

			// Create logger with file sink
			s_Logger = std::make_shared<spdlog::logger>("KuchCraft2", s_FileSink);
			s_Logger->set_level(spdlog::level::info);

			// Register logger with spdlog
			spdlog::register_logger(s_Logger);
		}		
	}

}