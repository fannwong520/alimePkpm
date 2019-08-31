#pragma once

#include "LogStream.h"
#include <chrono>
#include <string_view>
//#include "Timestamp.h"

class Logger
{
public:
	enum LogLevel
	{
		TRACE_,
		DEBUG_,
		INFO_,
		WARN_,
		ERROR_,
		FATAL_,
		NUM_LOG_LEVELS,
	};

	// compile time calculation of basename of source file
	class SourceFile
	{
	public:
		template<int N>
		SourceFile(const char(&arr)[N])
			: data_(arr),
			size_(N - 1)
		{
#ifdef _WIN32
			char spliter = '\\';
#else
			char spliter = '/';
#endif // _WIN32 || _WIN64
			const char* slash = strrchr(data_, spliter);
			if (slash)
			{
				data_ = slash + 1;
				size_ -= static_cast<int>(data_ - arr);
			}
		}

		explicit SourceFile(const char* filename)
			: data_(filename)
		{
			const char* slash = strrchr(filename, '/');
			if (slash)
			{
				data_ = slash + 1;
			}
			size_ = static_cast<int>(strlen(data_));
		}

		const char* data_;
		int size_;
	};

	Logger(SourceFile file, int line);
	Logger(SourceFile file, int line, LogLevel level);
	Logger(SourceFile file, int line, LogLevel level, const char* func);
	Logger(SourceFile file, int line, bool toAbort);
	~Logger();

	LogStream& stream() { return impl_.stream_; }

	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);

	typedef void(*OutputFunc)(const char* msg, int len);
	typedef void(*FlushFunc)();
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);

private:

	class Impl
	{
	public:
		typedef Logger::LogLevel LogLevel;
		Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
		void formatTime();
		void finish();

		std::chrono::system_clock::time_point time_;
		LogStream stream_;
		LogLevel level_;
		int line_;
		SourceFile basename_;
	};

	Impl impl_;

};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
	return g_logLevel;
}

//
// CAUTION: do not write:
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//
#define LOG_TRACE if (Logger::logLevel() <= Logger::TRACE_) \
  Logger(__FILE__, __LINE__, Logger::TRACE_, __func__).stream()
#define LOG_DEBUG if (Logger::logLevel() <= Logger::DEBUG_) \
  Logger(__FILE__, __LINE__, Logger::DEBUG_, __func__).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::INFO_) \
  Logger(__FILE__, __LINE__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN_).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR_).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL_).stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr)
{
	if (ptr == NULL)
	{
		Logger(file, line, Logger::FATAL).stream() << names;
	}
	return ptr;
}
