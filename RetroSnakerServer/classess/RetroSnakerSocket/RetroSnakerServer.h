#ifndef __RetroSnakerServer_H__
#define __RetroSnakerServer_H__


#pragma comment(lib, "WS2_32.lib")
#include <Windows.h>


class RetroSnakerServer
{
public:
	bool init();
	SOCKET CreateSock();
	SOCKET bindListen(SOCKET sock, u_int port);
	SOCKET AcceptClinet(SOCKET sock);
	void closeSocket(SOCKET sock);

};



#endif  //__RetroSnakerServer_H__