#pragma once
#include "pch.h"
// 로깅 라이브러리
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Singleton.h"
#include "Utility.h"

// 로그 레벨을 나타내는 열거형입니다.
enum LOG_LEVEL : uint8_t
{
	LOG_INFO = SPDLOG_LEVEL_INFO,			// 2
	LOG_TRACE = SPDLOG_LEVEL_TRACE,			// 0
	LOG_DEBUG = SPDLOG_LEVEL_DEBUG,			// 1
	LOG_WARN = SPDLOG_LEVEL_WARN,			// 3
	LOG_ERROR = SPDLOG_LEVEL_ERROR,			// 4
	LOG_CRITICAL = SPDLOG_LEVEL_CRITICAL,	// 5
	LOG_OFF = SPDLOG_LEVEL_OFF				// 6
};

/// <summary>
/// 로거 인터페이스를 정의하는 추상 클래스입니다.
/// 2024.05.07 _Doyo
/// </summary>
class ILogger
{
public:
	virtual ~ILogger() = default;

	/// <summary>
	/// 로그를 기록하는 함수입니다.
	/// </summary>
	/// <param name="_logLevel">해당 로그의 레벨을 설정합니다.</param>
	/// <param name="_message">로그의 메세지입니다.</param>
	/// <param name="_location">해당 로그의 위치를 나타냅니다. 
	/// C++20의 std::source_location 키워드를 사용해 자동으로 위치 정보를 제공합니다.</param>
	virtual void Log(LOG_LEVEL _logLevel, const std::string_view _message, std::source_location _location = std::source_location::current()) = 0;

	// 프로세스의 로그를 설정하는 함수입니다.
	void SetLogLevel(LOG_LEVEL _level = LOG_LEVEL::LOG_INFO)
	{
		m_logLevel = _level;
		if (m_logger)
		{
			m_logger->set_level(GetSpdLogLevel(_level));
		}
	}

	LOG_LEVEL GetLogLevel() const
	{
		return m_logLevel;
	}

protected:
	spdlog::level::level_enum GetSpdLogLevel(LOG_LEVEL _logLevel)
	{
		return static_cast<spdlog::level::level_enum>(_logLevel);
	}

protected:
	LOG_LEVEL m_logLevel = LOG_LEVEL::LOG_INFO;	// 현재 설정된 로그 레벨
	std::shared_ptr<spdlog::logger> m_logger;
};

/// <summary>
/// 콘솔창에 로그를 출력하는 클래스입니다. 디버그 모드에서만 사용할 클래스입니다.
/// 2024.05.07 _Doyo
/// </summary>
class ConsoleLogger : public ILogger
{
public:
	ConsoleLogger()
	{
		m_logger = spdlog::stdout_color_mt("console");
		SetLogLevel(LOG_TRACE);
	}

	~ConsoleLogger() = default;

	/// <summary>
	/// 콘솔창에 로그를 출력하는 함수입니다.
	/// </summary>
	/// <param name="_logLevel">해당 로그의 레벨을 설정합니다.</param>
	/// <param name="_message">로그의 메세지입니다.</param>
	/// <param name="_location">해당 로그의 위치를 나타냅니다. 
	/// C++20의 std::source_location 키워드를 사용해 자동으로 위치 정보를 제공합니다.</param>
	virtual void Log(LOG_LEVEL _logLevel, const std::string_view _message
		, std::source_location _location = std::source_location::current()) override
	{
		//SetLogLevel();
		if (_logLevel >= m_logLevel)
		{
			std::string fileName = std::filesystem::path(_location.file_name()).filename().string();
			//td::clog << "File: " << fileName << '(' << _location.line() << ") : " << _message << std::endl;
			m_logger->log(GetSpdLogLevel(_logLevel), "File: {}({}) : {}", fileName, _location.line(), _message);
		}
	}

	/// <summary>
	/// 프로세스의 로그 레벨을 설정하는 함수입니다.
	/// 콘솔 로거 클래스는 디버그 모드의 디폴트 로거로,
	/// 모든 로그들을 출력합니다.
	/// </summary>
	/// <param name="_level">디폴트값인 LOG_INFO로 설정합니다.</param>
	void SetLogLevel(LOG_LEVEL _level = LOG_LEVEL::LOG_INFO)
	{
		m_logLevel = _level;
		m_logger->set_level(GetSpdLogLevel(m_logLevel));
	}

private:
	spdlog::level::level_enum GetSpdLogLevel(LOG_LEVEL _logLevel)
	{
		switch (_logLevel)
		{
			case LOG_INFO:
				return spdlog::level::info;
			case LOG_WARN:
				return spdlog::level::warn;
			case LOG_ERROR:
				return spdlog::level::err;
			default:
				return spdlog::level::debug;
		}
	}
};

/// <summary>
/// 로그 파일에 로그를 기록하는 클래스입니다. 릴리즈 모드에서 사용할 클래스입니다.
/// 기획자분들이나 테스터 유저들에게 받아서 디버그에 사용할 수 있습니다.
/// 2024.05.07 _Doyo
/// </summary>
class FileLogger : public ILogger
{
public:
	FileLogger()
	{
		// 인스턴스 생성 시 새로운 로그 파일 생성 및 실행.
		OpenNewLogFile();
	}

	~FileLogger() = default;

	/// <summary>
	/// 로그 파일에 로그를 출력하는 함수입니다.
	/// 로그 파일의 크기가 일정 이상에 도달했을 경우 새로운 로그 파일을 생성하고 실행합니다.
	/// </summary>
	/// <param name="_logLevel">해당 로그의 레벨을 설정합니다.</param>
	/// <param name="_message">출력할 메세지입니다.</param>
	/// <param name="_location">해당 로그의 위치를 나타냅니다. 
	/// C++20의 std::source_location 키워드를 사용해 자동으로 위치 정보를 제공합니다.</param>
	void Log(LOG_LEVEL _logLevel, const std::string_view _message, std::source_location _location = std::source_location::current()) override
	{
		if (_logLevel < m_logLevel)
		{
			return;
		}

		if (m_logger)
		{
			std::string fileName = std::filesystem::path(_location.file_name()).filename().string();
			m_logger->log(GetSpdLogLevel(_logLevel), "File: {}({}) : {}", fileName, _location.line(), _message);
		}
	}

	/// <summary>
	/// 로그 레벨을 설정하는 함수입니다.
	/// 파일 로거 클래스는 릴리즈 모드의 로거로,
	/// 플레이에 지장이 있을 수준의 로그들을 출력합니다.
	/// </summary>
	/// <param name="_level"></param>
	void SetLogLevel(LOG_LEVEL _level = LOG_LEVEL::LOG_INFO)
	{
		m_logLevel = _level;
		if (m_logger)
		{
			m_logger->set_level(GetSpdLogLevel(m_logLevel));
		}
	}

private:
	/// <summary>
	/// 새로운 로그 파일을 생성하고 실행하는 함수입니다.
	/// 로그 파일의 파일명은 현지의 표준 시간대를 기준으로 합니다.
	/// </summary>
	void OpenNewLogFile()
	{
		auto now = std::chrono::system_clock::now();
		auto local_now = std::chrono::zoned_time{ std::chrono::current_zone(), now };
		std::string fileName = std::format("../log/log_{}.log", std::format("{:%Y-%m-%d_%H-%M-%S}", local_now));

		m_logger = spdlog::basic_logger_mt("file_logger", fileName);
		if (!m_logger)
		{
			throw std::runtime_error("Failed to open log file.");
		}
		m_logger->set_level(GetSpdLogLevel(m_logLevel));
	}

	spdlog::level::level_enum GetSpdLogLevel(LOG_LEVEL _logLevel)
	{
		switch (_logLevel)
		{
			case LOG_INFO:
				return spdlog::level::info;
			case LOG_WARN:
				return spdlog::level::warn;
			case LOG_ERROR:
				return spdlog::level::err;
			default:
				return spdlog::level::debug;
		}
	}

private:
	std::ofstream m_logFile;	// 현재 저장중인 파일
	std::size_t m_maxFileSize = 5 * 1024 * 1024;	// 로그 파일의 최대 크기

};

// 전역 로거 포인터 선언
extern ILogger* g_pLogger;

// 로거 초기화 및 정리 함수 선언
void InitializeLogger();
void PrintLoggerInfo();
void CleanupLogger();

#define DLOG(_logLevel, _message) \
if(g_pLogger) g_pLogger->Log(_logLevel, _message)

#define SET_LOG_LEVEL(level) \
if(g_pLogger) g_pLogger->SetLogLevel(level)