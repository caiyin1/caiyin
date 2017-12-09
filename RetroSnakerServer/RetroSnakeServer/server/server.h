#ifndef __Server_H__
#define __Server_H__

#include <WinSock2.h>
#define PORT 6000

class Server
{
public:
	bool init();
	SOCKET CreateSock();
	SOCKET BindListen(SOCKET sock);
	SOCKET acceptClient(SOCKET sock);
	void closeSockt(SOCKET sock);
};



#endif //__Server_H__


