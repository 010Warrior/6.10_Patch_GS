#pragma once
#include <spdlog/spdlog.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>

static inline std::vector<spdlog::sink_ptr> sinks;
inline void SetupLogger()
{
	sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>())->set_pattern("[%D-%T] %n: %^%v%$");
	sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("6_1GameServer.log", true))->set_pattern("[%D-%T] %n: %l: %v");

	auto GameServer = std::make_shared<spdlog::logger>("GameServer", sinks.begin(), sinks.end());
	spdlog::register_logger(GameServer);
	GameServer->set_level(spdlog::level::level_enum::info);
	GameServer->flush_on(spdlog::level::level_enum::info);
}

#define LOG_INFO(...)					\
		spdlog::get("GameServer")->warn(std::format(__VA_ARGS__));