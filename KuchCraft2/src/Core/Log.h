///
/// @file Log.h
/// 
/// @author Michal Kuchnicki 
/// 
/// @brief Header file containing the declaration of the Log class, which provides static methods 
///        for logging messages to a file.
/// 
/// @details The Log class uses the spdlog library for efficient and thread-safe logging. 
///          It provides various logging levels such as info, warning, and error.
///          This logger is intended for use in large-scale applications and should be initialized 
///          at the start of the program via the Init() function. After calling Init(), the logger
///          will be set up for logging messages to file.
/// 
/// @note The Init() method must be called before any logging methods. Failure to do so will result in
///       undefined behavior.
/// 
/// @thread_safety Logger is thread-safe as spdlog handles internal synchronization.
/// 
/// @example
///         // Initializing the logger at the start of the application.
///         KuchCraft::Log::Init();
///         
///         // Logging an info message.
///         KuchCraft::Log::Info("Application started successfully");
/// 
///         // Logging a warning message.
///         KuchCraft::Log::Warn("Low disk space: {0}% remaining", diskSpacePercentage);
/// 
///         // Logging an error message.
///         KuchCraft::Log::Error("Failed to open file: {0}", fileName);
/// 
/// @future Could be extended to log also to game console.
/// 
/// @see spdlog::logger for more information about the underlying logger library used.
/// 

#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace KuchCraft {

	class Log
	{
	public:
		/// Initializes the logger. Should be called once at the start of the application.
		static void Init();

		/// Logs an info message.
		/// @param format - format string
		/// @param args - format arguments
		template<typename... Args>
		static inline void Info(fmt::format_string<Args...> format, Args&&... args)
		{
			s_Logger->info(format, std::forward<Args>(args)...);
		}

		/// Logs an warning message.
		/// @param format - format string
		/// @param args - format arguments
		template<typename... Args>
		static inline void Warn(fmt::format_string<Args...> format, Args&&... args)
		{
			s_Logger->warn(format, std::forward<Args>(args)...);
		}

		/// Logs an error message.
		/// @param format - format string
		/// @param args - format arguments
		template<typename... Args>
		static inline void Error(fmt::format_string<Args...> format, Args&&... args)
		{
			s_Logger->error(format, std::forward<Args>(args)...);
		}

	private:
		/// The main logger instance.
		static inline std::shared_ptr<spdlog::logger> s_Logger = nullptr;

		/// A file sink for logging to a file.
		static inline spdlog::sink_ptr s_FileSink;

	private:
		/// The Log class is a static utility class and cannot be instantiated.
		/// Constructors, destructors, and assignment operators are deleted to
		/// prevent instantiation and copying.
		Log() = delete;
		~Log() = delete;
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;
		Log(Log&&) = delete;
		Log& operator=(Log&&) = delete;

	};

}