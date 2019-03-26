#pragma once

#include <string>
#include <memory>
#include <spdlog/logger.h>

namespace Logger
{
	using LoggerPtr = std::shared_ptr<spdlog::logger>;
	
	LoggerPtr GetLogger(const std::string& name);
	void RegisterSink(spdlog::sink_ptr sink);
}
