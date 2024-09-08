/// 
/// @file Log.cpp
/// 
/// @author Michal Kuchnicki 
/// 

#include "kcpch.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Core/Log.h"
#include "Core/Config.h"

namespace KuchCraft {

	void Log::Init()
	{
		/// Guard against multiple initializations
		if (!s_Logger)
		{
			/// Create file sink
			s_FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(ApplicationConfig::GetLogData().File, true);
			s_FileSink->set_pattern(ApplicationConfig::GetLogData().Pattern);

			/// Create logger with file sink
			s_Logger = std::make_shared<spdlog::logger>(ApplicationConfig::GetLogData().Name, s_FileSink);
			s_Logger->set_level(spdlog::level::info);

			/// Register logger with spdlog
			spdlog::register_logger(s_Logger);
		}		
	}

}