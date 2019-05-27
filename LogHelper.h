#ifndef LOG_HELPER_H_
#define LOG_HELPER_H_

#include <string>
#include <time.h>

typedef unsigned	Log_level;
typedef time_t		Log_time;
typedef unsigned	Log_user;
typedef std::string	Log_key;
typedef std::string	Log_value;

enum Log_Enum
{
	/*
	* Level from low to high.
	*/

	LOG_ALL,
	LOG_TRACE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,
	LOG_OFF
};

class Log
{
	Log_level	m_lvl;
	Log_time	m_time;
	Log_user	m_usr;
	Log_key		m_key;
	Log_value	m_val;

public:
	explicit Log() :
		m_lvl(Log_Enum::LOG_ALL), 
		m_time(time(NULL)),
		m_usr(0),
		m_key(std::string("")),
		m_val(std::string(""))
	{
		// Default construct.
	}

	explicit Log(
		Log_user	usr,
		Log_key		key,
		Log_value	val,
		Log_level	lvl = Log_Enum::LOG_ALL,
		Log_time	tm = time(NULL)
	) : m_lvl(lvl),
		m_time(tm),
		m_usr(usr),
		m_key(key),
		m_val(val)
	{
		// Parameter construct.
	}
};

class LogHelper : public Log
{
public:
	LogHelper() : Log()
	{}
};

#endif
