#ifndef COMM_DEF_H_
#define COMM_DEF_H_

#include <string>

typedef struct _IP_PORT
{
	IP_PORT() :m_ip("0.0.0.0"), m_port(18080)
	{
		//construction
	}
	IP_PORT(const std::string& ip, unsigned port) : m_ip(ip), m_port(port)
	{
		//construction
	}

	std::string m_ip;
	unsigned m_port;
}IP_PORT;

#endif // !COMM_DEF_H_
