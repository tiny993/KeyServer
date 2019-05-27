#ifndef ERROR_HANDLE_H_
#define ERROR_HANDLE_H_

#include <string>

enum Code
{
	NO_PROBLEM = 0,

	//command code
	CMD_GET_ONLINE_ACCOUNT = 100,
	CMD_DO_REGISTE,
	CMD_DO_LOGIN,

	//data type
	DAT_ACCOUNT_DATA = 200,
	DAT_LOGIN_DATA,
	DAT_REGISTER_PACKET,
	DAT_LOGIN_PACKET,
	DAT_HEARTBEAT_PACKET,
	DAT_COMMAND_PACKET,

	//server code
	SER_ACCOUNT_ALREADY_HAVE = 300,
	SER_ACCOUNT_NAME_HAVE,
	SER_STR_LENGTH_ZERO,
	SER_ACCOUNT_NUMBER_SHORT,
	SER_ACCOUNT_NUMBER_ONLY_NUMBER,
	SER_ACCOUNT_PWD_ILLEAGAL,
	SER_ACCOUNT_PWD_NOT_MATCH,
	SER_SECRET_KEY_ERROR,
	SER_ACCOUNT_NOT_EXIST,
	SER_FILE_OPEN_FAILED,
	SER_XML_NO_DATA,
	SER_WAS_STARTUP_FAILED,
	SER_SOCKET_INIT_FAILED,
	SER_BIND_FAILED,
	SER_LISTEN_FAILED,
	SER_SERVER_ERROR,
	SER_USE_MEMORY_MAP,
	SER_NOT_USE_MEMORY_MAP,


	//client code
	CLI_CONNECT_FAILED = 400,

	//config code
	CFG_FILE_OPEN_FAILED = 500,
	CFG_FILE_READ_FAILED,
	CFG_FILE_WRITE_FAILED,
	CFG_NOT_FOUND_VALUE,
	CFG_KEY_EXIST,
	CFG_KEY_VALUE_NULL,

	//universal code
	UNI_REGISTE_SUCCESS = 1000,
	UNI_LOGIN_SUCCESS,
	UNI_REGISTE_FAILED,
	UNI_LOGIN_FAILED,
	UNI_INVALID_HANDLE_VALUE,
	UNI_INVALID_FP_VALUE,
	UNI_ALLOC_MEM_FAILED
};


typedef struct Tiny_code
{
	unsigned code;
	std::string err_str;

	Tiny_code() : code(NO_PROBLEM), err_str(ErrDecl(NO_PROBLEM))
	{
		//construct
	}

	Tiny_code(unsigned u_code) : code(u_code), err_str(ErrDecl(u_code))
	{
		//construct
	}

	std::string ErrDecl(unsigned int code)
	{
		switch (code)
		{
		case NO_PROBLEM:
			return "No problem";

		case SER_ACCOUNT_ALREADY_HAVE:
			return "Server: This account is registered";

		case SER_ACCOUNT_NAME_HAVE:
			return "Server: User name has been used";

		case SER_STR_LENGTH_ZERO:
			return "Server: String variable length is 0";

		case SER_ACCOUNT_NUMBER_SHORT:
			return "Server: Account length is too short";

		case SER_ACCOUNT_NUMBER_ONLY_NUMBER:
			return "Server: Account can only use numbers";

		case SER_ACCOUNT_PWD_ILLEAGAL:
			return "Server: The composition of the password is illegal";

		case SER_ACCOUNT_PWD_NOT_MATCH:
			return "Server: Account password does not match";

		case SER_SECRET_KEY_ERROR:
			return "Server: Identification number is wrong";

		case SER_ACCOUNT_NOT_EXIST:
			return "Server: This account is not exists";

		case SER_FILE_OPEN_FAILED:
			return "Server: File open failed";

		case SER_XML_NO_DATA:
			return "Server: No data in XML file";

		case SER_WAS_STARTUP_FAILED:
			return "Server: Windows socket API startup failed";

		case SER_SOCKET_INIT_FAILED:
			return "Server: Socket initialization failed";

		case SER_BIND_FAILED:
			return "Server: Socket and address binding failed";

		case SER_LISTEN_FAILED:
			return "Server: Failed to listen on socket";

		case SER_SERVER_ERROR:
			return "Server: Server internal error";

		case SER_USE_MEMORY_MAP:
			return "Server: Use memory mapping";

		case SER_NOT_USE_MEMORY_MAP:
			return "Server: Do not use memory mapping";

		case CLI_CONNECT_FAILED:
			return "Client: Connection failure";

		case CFG_FILE_OPEN_FAILED:
			return "Config: File open failed";

		case CFG_FILE_READ_FAILED:
			return "Config: File read failed";

		case CFG_FILE_WRITE_FAILED:
			return "Config: File write failed";

		case CFG_NOT_FOUND_VALUE:
			return "Config: Cannot found value in this config file";

		case CFG_KEY_EXIST:
			return "Config: Config key is exist in this config file";

		case CFG_KEY_VALUE_NULL:
			return "Config: Config key value is needed";

		case UNI_INVALID_HANDLE_VALUE:
			return "Handle value is invalid";

		case UNI_INVALID_FP_VALUE:
			return "File create failed in linux";

		case UNI_REGISTE_SUCCESS:
			return "Registration success";

		case UNI_REGISTE_FAILED:
			return "Registration failure";

		case UNI_LOGIN_SUCCESS:
			return "Login server success";

		case UNI_LOGIN_FAILED:
			return "Login server failure";

		case UNI_ALLOC_MEM_FAILED:
			return "Failed to allocate memory";

		default:
			break;
		}

		return "Error code is not defined";
	}

}TG_CODE;

#endif // !ERROR_HANDLE_H_
