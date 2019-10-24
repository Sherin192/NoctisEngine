#include "Logger.h"
#include <stdarg.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#if defined(_WIN32)
#include "spdlog/sinks/msvc_sink.h"
#endif

#define NOCTIS_LOG_TO_FILE 1
namespace noctis
{
	Logger::Logger()
	{
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(spdlog::level::trace);
		console_sink->set_pattern("%^[%T] %n: %v%$");

		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("debug.txt", true);
		file_sink->set_level(spdlog::level::trace);
		file_sink->set_pattern("%^[%T] %n: %v%$");

#if defined(_WIN32)
		auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		msvc_sink->set_level(spdlog::level::trace);
		msvc_sink->set_pattern("%^[%T] %n: %v%$");
#endif
		using namespace std::string_literals;
		m_pLogger = std::make_unique<spdlog::logger>("Noctis"s, spdlog::sinks_init_list{ console_sink, file_sink NOCTIS_MSVC_ONLY(, msvc_sink) });
		m_pLogger->set_level(spdlog::level::trace);
	}


}