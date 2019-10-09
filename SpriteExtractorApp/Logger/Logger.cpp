#include "Logger.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/msvc_sink.h>

namespace Logger
{
    namespace detail
    {
        static std::vector<spdlog::sink_ptr>& GetSinks()
        {
            static std::vector<spdlog::sink_ptr> sinks =
            {
                    {std::make_shared<spdlog::sinks::stdout_color_sink_mt>()},
#ifdef _WIN32
                    {std::make_shared<spdlog::sinks::msvc_sink_mt>()}
#endif
            };

            return sinks;
        }


        LoggerPtr CreateLogger(const std::string& name)
        {
            LoggerPtr logger = std::make_shared<spdlog::logger>(name, spdlog::sinks_init_list());

            std::vector<spdlog::sink_ptr>& sinks = logger->sinks();
            const std::vector<spdlog::sink_ptr>& registeredSinks = GetSinks();
            sinks.reserve(registeredSinks.size());
            sinks.insert(sinks.begin(), registeredSinks.begin(), registeredSinks.end());

            spdlog::register_logger(logger);

            // TODO
            logger->set_level(spdlog::level::debug);

            return logger;
        }

        void AddSink(spdlog::sink_ptr sink)
        {
            GetSinks().push_back(sink);

            auto appendSink = [sink](std::shared_ptr<spdlog::logger> logger)
            {
                logger->sinks().emplace_back(sink);
            };
            spdlog::apply_all(appendSink);
        }
    }
}

Logger::LoggerPtr Logger::GetLogger(const std::string& name)
{
    auto logger = spdlog::get(name);

    if (logger == nullptr)
    {
        logger = detail::CreateLogger(name);
    }

    return logger;
}

void Logger::RegisterSink(spdlog::sink_ptr sink)
{
    detail::AddSink(sink);
}
