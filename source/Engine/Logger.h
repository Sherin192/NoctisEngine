#ifndef LOGGER_H
#define LOGGER_H

#include "Core_pch.h"
#include "spdlog/spdlog.h"

namespace noctis
{
	enum class LogLevel {Trace, Info, Warning, Error};
	class Logger
	{
	public:
		Logger();
		[[nodiscard]] const auto& Get() const noexcept { return m_pLogger; }
	private:
		std::unique_ptr<spdlog::logger> m_pLogger;
	};

	template <typename... Args>
	void Log(LogLevel level, Args&&... args)
	{
		static std::unique_ptr<Logger> pLogger = std::make_unique<Logger>();
		if (level == LogLevel::Trace)
			pLogger->Get()->trace(args...);
		else if (level == LogLevel::Info)
			pLogger->Get()->info(args...);
		else if (level == LogLevel::Warning)
			pLogger->Get()->warn(args...);
		else if (level == LogLevel::Error)
			pLogger->Get()->error(args...);
	}
}

#endif LOGGER_H