#pragma once
#include"spdlog/spdlog.h"

#define BANANA_DEFAULT_LOGGER_NAME "bananaLogger"

#if defined(BANANA_PLATFORM_WINDOWS)
#define BANANA_BREAK() __debugbreak();
#else
#define BANANA_BREAK() __builtin_trap();
#endif

#ifndef BANANA_CONFIG_RELEASE
#define BANANA_TRACE(...)	 if(spdlog::get(BANANA_DEFAULT_LOGGER_NAME)!=nullptr) {spdlog::get(BANANA_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);}
#define BANANA_DEBUG(...)	 if(spdlog::get(BANANA_DEFAULT_LOGGER_NAME)!=nullptr) {spdlog::get(BANANA_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);}
#define BANANA_INFO(...)	 if(spdlog::get(BANANA_DEFAULT_LOGGER_NAME)!=nullptr) {spdlog::get(BANANA_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);}
#define BANANA_WARN(...)	 if(spdlog::get(BANANA_DEFAULT_LOGGER_NAME)!=nullptr) {spdlog::get(BANANA_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);}
#define BANANA_ERROR(...)	 if(spdlog::get(BANANA_DEFAULT_LOGGER_NAME)!=nullptr) {spdlog::get(BANANA_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);}
#define BANANA_FATAL(...)	 if(spdlog::get(BANANA_DEFAULT_LOGGER_NAME)!=nullptr) {spdlog::get(BANANA_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
#define BANANA_ASSERT(x,msg) if((x)) {} else {BANANA_FATAL("ASSERT - {}\n\t{}\n\tfile: {}\n\ton line: {}",#x,msg,__FILE__,__LINE__); BANANA_BREAK(); }
#else
//disable logging in release builds
#define BANANA_ASSERT(x,msg) (void)0
#define BANANA_TRACE(...) (void)0
#define BANANA_DEBUG(...) (void)0
#define BANANA_INFO(...)  (void)0
#define BANANA_WARN(...)  (void)0
#define BANANA_ERROR(...) (void)0
#define BANANA_FATAL(...) (void)0
#endif
