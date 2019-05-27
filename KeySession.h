#ifndef KEY_SESSION_H_
#define KEY_SESSION_H_

#include <string>
#include <time.h>

class Session 
{
public:
	Session() :
		status(0),
		m_account(std::string("")),
		m_password(std::string("")),
		m_check(std::string("")),
		m_login(NULL),
		m_logout(NULL),
		m_payload(std::string(""))
	{
		// Construct
	}

	// public member
	// record the client and server communicate status
	unsigned status;			
	std::string m_account;
	std::string m_password;
	std::string m_check;
	time_t m_login;
	time_t m_logout;
	std::string m_payload;

	// public method
	virtual std::string getSessionAccount() = 0;
	virtual std::string getSessionPasswd() = 0;
	virtual std::string getSessionCheck() = 0;
};

class KeySession : public Session
{
public:
	virtual std::string getSessionAccount();

private:

};

inline std::string KeySession::getSessionAccount()
{
	
}
#endif // !KEY_SESSION_H_
