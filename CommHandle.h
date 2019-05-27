#ifndef COMM_HANDLE_H_
#define COMM_HANDLE_H_

#include <sys/socket.h>
#include "ErrorHandle.h"

template<typename _DataT>
class CommHandle
{
public:
	CommHandle(_DataT data_in) : data(data_in)
	{
		//Construct
	}

	TG_CODE DataSndRev(int fd, _DataT snd_data, _DataT rev_data)
	{
		send(fd, (char*)snd_data, sizeof(snd_data), 0);
	}

private:
	_DataT data;
};

#endif // !COMM_HANDLE_H_
