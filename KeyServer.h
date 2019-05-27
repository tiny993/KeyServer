#ifndef KEY_SERVER_H_
#define KEY_SERVER_H_

#include "Config.h"
#include "CommDef.h"

#include <vector>
#include <memory>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
//for souockaddr_in
#include <netinet/in.h>		
#include <sys/socket.h>

//for select
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

//for bzero
#include <strings.h>		
#include <string.h>

#define BUFF_SIZE 1024

class KeyServer
{
public:
	KeyServer();
	TG_CODE CreateServer(int cli_num);

private:
	std::string m_ip = std::string("");
	unsigned m_port = 0;
	std::vector<int> cli_vec;
};

KeyServer::KeyServer()
{
	std::shared_ptr<Config> config = std::make_shared<Config>("key_server.cfg");
	m_ip = config->Get("server.ip", "0.0.0.0");
	m_port = std::stoi(config->Get("server.port"));
	cli_vec.clear();
}

inline TG_CODE KeyServer::CreateServer(int cli_num)
{
	int ser_sock;

	struct sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(m_port);
	ser_addr.sin_addr.s_addr = INADDR_ANY;

	if ( (ser_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		return SER_SOCKET_INIT_FAILED;
	}

	if (bind(ser_sock, (const struct sockaddr *)&ser_addr, sizeof(sockaddr_in)) < 0)
	{
		return SER_BIND_FAILED;
	}

	if (listen(ser_sock, 128) < 0)
	{
		return SER_LISTEN_FAILED;
	}

	int max_fd = 1;
	fd_set ser_fdset;
	struct timeval tv;

	while (true)
	{
		tv.tv_sec = 15;
		tv.tv_usec = 0;

		FD_ZERO(&ser_fdset);

		max_fd = max_fd < 0 ? 0 : max_fd;

		FD_SET(ser_sock, &ser_fdset);

		for (int i = 0; i < cli_num; ++i)
		{
			if (cli_vec[i] != 0)
			{
				FD_SET(cli_vec[i], &ser_fdset);
				max_fd = max_fd < cli_vec[i] ? cli_vec[i] : max_fd;
			}
		}

		switch (select(max_fd + 1, &ser_fdset, NULL, NULL, &tv))
		{
		case 0:
			// need log(time out)
			continue;
			
		case -1:
			// need log(select failure)
			continue;

		default:
			//if (FD_ISSET(0, &ser_fdset)) //标准输入是否存在于ser_fdset集合中（也就是说，检测到输入时，做如下事情）
			//{
			//	printf("send message to");
			//	bzero(input_message, BUFF_SIZE);
			//	fgets(input_message, BUFF_SIZE, stdin);

			//	for (i = 0; i < CLI_NUM; i++)
			//	{
			//		if (client_fds[i] != 0)
			//		{
			//			printf("client_fds[%d]=%d\n", i, client_fds[i]);
			//			send(client_fds[i], input_message, BUFF_SIZE, 0);
			//		}
			//	}

			//}

			if (FD_ISSET(ser_sock, &ser_fdset))
			{
				struct sockaddr_in cli_addr;
				socklen_t addr_len;
				auto cli_sock = accept(ser_sock, (struct sockaddr*)&cli_addr, &addr_len);
				if (cli_sock > 0)
				{
					bool flags = false;

					for (int i = 0; i < cli_num; ++i)
					{
						if (cli_vec[i] == 0)
						{
							flags = true;
							cli_vec[i] = cli_sock;
							break;
						}
					}

					if (flags)
					{
						// add client socket ok!
					}
					else
					{
						// add client socket failed! vector is full.
						
					}
				}
			}
		}
	}

}

#endif // !KEY_SERVER_H_
