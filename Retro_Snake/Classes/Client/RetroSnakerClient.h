#include "cocos2d.h"
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
USING_NS_CC;

class RetroSNakerClient
{
public:
	bool initSock();
	/**
	* @breif ����IP��ַ
	*/
	u_long ResolverAdress(char *serverIp);
	/**
	* @breif ���ӷ�����
	*/
	SOCKET ConnectServer(u_long serverIp, int port);


public:
	SOCKET m_sock;
private:





};