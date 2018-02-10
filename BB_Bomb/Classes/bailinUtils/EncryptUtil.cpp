/*
 * =====================================================================================
 *
 *       Filename:  EncryptUtil.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/19/2015 13:30:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (), sosoayaen@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "EncryptUtil.h"
#include <assert.h>
#include <string.h>

extern "C"
{
#include "crypto/md5/md5.h"
#include "crypto/sha1/sha1.h"
#include "crypto/base64/libb64.h"
}

NS_BL_UTIL_BEGIN

std::string EncryptUtil::MD5(const std::string &strData)
{
	std::string strMD5 = "";
	do
	{
		BREAK_IF(strData.empty());

		MD5_CTX ctx;
		_MD5_Init(&ctx);
		_MD5_Update(&ctx, (void*)strData.c_str(), strData.length());
		unsigned char szBuff[16] = { 0 };
		_MD5_Final(szBuff, &ctx);

		char szMD5[33] = { 0 };
		char* pCh = szMD5;
		for (int i = 0; i < sizeof(szBuff); i++)
		{
			sprintf(pCh, "%02x", szBuff[i]);
			pCh = pCh + 2;
		}
		strMD5 = szMD5;

	} while (0);

	return strMD5;
}

#define MD5_FILE_READ_BUFFER 4096
std::string EncryptUtil::MD5File(const std::string& strFilePath, const std::function<void(uint32_t, uint32_t)>& callback)
{
	std::string strRet = "";
	// 设定校验的缓存
	unsigned char buffer[MD5_FILE_READ_BUFFER] = { 0 };
	// 先打开文件
	FILE* fp = fopen(strFilePath.c_str(), "rb");
	if (fp)
	{
		// 得到文件长度
		fseek(fp, 0, SEEK_END);
		size_t nFileLen = ftell(fp);
		rewind(fp);

		if (nFileLen <= 0) return strRet;

		MD5_CTX ctx;
		_MD5_Init(&ctx);
		unsigned char szResult[16];
		// 设定读取的长度
		size_t nReaded = 0;
		while (nReaded < nFileLen)
		{
			size_t nRead = fread(buffer, 1, MD5_FILE_READ_BUFFER, fp);
			if (nRead < MD5_FILE_READ_BUFFER)
			{
				// 读取出问题了
				if (feof(fp))
				{
					// 读到了文件末尾
					nReaded += nRead;
				}
				else if (ferror(fp))
				{
					// 清空错误位
					clearerr(fp);
					// 出问题了，直接返回空即可
					return strRet;
				}
			}
			else
			{
				// 加上已经读取的字节
				nReaded += nRead;
			}
			_MD5_Update(&ctx, buffer, nRead);

			// 进度回调通知
			if (callback)
			{
				callback((uint32_t)nReaded, (uint32_t)nFileLen);
			}
		}
		fclose(fp);
		_MD5_Final(szResult, &ctx);

		// 进度回调通知
		if (callback)
		{
			callback((uint32_t)nReaded, (uint32_t)nFileLen);
		}

		// 输出结果
		char szMD5Result[33] = { 0 };
		for (int i = 0; i < 16; i++)
		{
			sprintf(&szMD5Result[i * 2], "%02x", szResult[i]);
		}
		strRet = szMD5Result;
	}

	return strRet;
}

void EncryptUtil::MD5(void *input, int inputLength, unsigned char *output)
{

}

void EncryptUtil::sha1(unsigned char *input, int inputLength, unsigned char *key, int keyLength, unsigned char *buffer, int bufferLength)
{

}

int EncryptUtil::encodeBase64(const void *input, int inputLength, char *output, int outputBufferLength)
{
	return 0;
}

int EncryptUtil::decodeBase64(const char *input, void *output, int outputBufferLength)
{
	return 0;
}


std::string EncryptUtil::encodeBase64(const char* pData, int nDataLen)
{
	// 初始化
	base64_encodestate encodeState;
	base64_init_encodestate(&encodeState);

	// 临时处理用的 buffer
	auto len = nDataLen * 2;
	char* pBuffer = new char[len];
	memset(pBuffer, 0, len);

	char* pCh = pBuffer;
	// 处理加密
	int nCnt = base64_encode_block(pData, nDataLen, pCh, &encodeState);
	pCh += nCnt;
	nCnt += base64_encode_blockend(pCh, &encodeState);

	std::string strRet = pBuffer;
	delete pBuffer;

	return strRet;
}

char* EncryptUtil::decodeBase64(const std::string& strData, int& nOutputBufferLen)
{
	// 初始化
	base64_decodestate decodeState;
	base64_init_decodestate(&decodeState);

	// 临时处理的 buffer
	char* pBuffer = new char[strData.length()];

	char* pCh = pBuffer;

	// 解密处理
	nOutputBufferLen = base64_decode_block(strData.c_str(), strData.length(), pBuffer, &decodeState);
	pCh += nOutputBufferLen;
	*pCh = '\0';

	return pBuffer;
}

char* EncryptUtil::decodeBase64(const unsigned char* pChBuffer, int nBufLength, int& nOutputBufferLen)
{
	// 初始化
	base64_decodestate decodeState;
	base64_init_decodestate(&decodeState);

	// 临时处理的 buffer
	char* pBuffer = new char[nBufLength];

	char* pCh = pBuffer;

	// 解密处理
	nOutputBufferLen = base64_decode_block((const char*)pChBuffer, nBufLength, pBuffer, &decodeState);
	pCh += nOutputBufferLen;
	*pCh = '\0';

	return pBuffer;
}

std::string EncryptUtil::encodeBase64(const std::string &strSource)
{
	std::string strRet = "";
	// 初始化
	base64_encodestate encodeState;
	base64_init_encodestate(&encodeState);

	// 临时处理用的 buffer
	auto len = strSource.length() * 2;
	char* pBuffer = new char[len];
	memset(pBuffer, 0, len);

	char* pCh = pBuffer;
	// 处理加密
	int nCnt = base64_encode_block(strSource.c_str(), strSource.length(), pCh, &encodeState);
	pCh += nCnt;
	base64_encode_blockend(pCh, &encodeState);
	strRet = pBuffer;
	delete[]pBuffer;

	return strRet;
}

std::string EncryptUtil::decodeBase64(const std::string &strSource)
{
	std::string strRet = "";

	// 初始化
	base64_decodestate decodeState;
	base64_init_decodestate(&decodeState);

	// 临时处理的 buffer
	auto len = strSource.length();
	char* pBuffer = new char[len];

	char* pCh = pBuffer;

	// 解密处理
	int nCnt = base64_decode_block(strSource.c_str(), len, pBuffer, &decodeState);
	pCh += nCnt;
	*pCh = '\0';

	strRet = pBuffer;

	delete[]pBuffer;

	return strRet;
}

std::string EncryptUtil::blowfish(const uint8_t *pData, unsigned int nLength, const char *pszKey)
{
	std::string strBlowfish = "";
	return strBlowfish;
}

static unsigned char toHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

static unsigned char fromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string EncryptUtil::urlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += toHex((unsigned char)str[i] >> 4);
			strTemp += toHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string EncryptUtil::urlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = fromHex((unsigned char)str[++i]);
			unsigned char low = fromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

NS_BL_UTIL_END
