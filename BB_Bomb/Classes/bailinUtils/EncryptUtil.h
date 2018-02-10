/*
 * =====================================================================================
 *
 *       Filename:  EncryptUtil.h
 *
 *    Description:  提供加密相关的通用函数
 *
 *        Version:  1.0
 *        Created:  03/19/2015 13:27:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (), sosoayaen@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#pragma once

#include "bailinMacro.h"
#include <string>
#include <functional>
#include <sys/types.h>

NS_BL_UTIL_BEGIN

class EncryptUtil
{
public:
	static const int MD5_BUFFER_LENGTH = 16;
	static const int SHA1_BUFFER_LENGTH = 20;

	// MD5文件校验进度通知回调
	typedef std::function<void(uint32_t, uint32_t)> MD5FileProgressCallback;

	/**
	 * @brief 返回数据的MD5值
	 * @param pData    原始数据
	 * @param nLength  数据长度
	 * @return std::string
	 */
	static std::string MD5(const std::string& strData);

	/**
	 * @brief 对文件计算MD5值
	 * @param strFilePath 待计算MD5的文件路径
	 * @param callback 进度通知回调函数
	 * @param p
	 */
	static std::string MD5File(const std::string& strFilePath, const MD5FileProgressCallback& callback = nullptr);

	static std::string blowfish(const uint8_t* pData, unsigned int nLength, const char* pszKey);

	/** @brief Encoding data with Base64 algorithm, return encoded string length */
	static int encodeBase64(const void* input, int inputLength, char* output, int outputBufferLength);

	static std::string encodeBase64(const std::string& strSource);

	static std::string encodeBase64(const char* pData, int nDataLen);
	static char* decodeBase64(const std::string& strData, int& nOutputBufferLen);
	static char* decodeBase64(const unsigned char* pChBuffer, int nBufLength, int& nOutputBufferLe);


	/** @brief Decoding Base64 string to data, return decoded data length */
	static int decodeBase64(const char* input, void* output, int outputBufferLength);

	static std::string decodeBase64(const std::string& strSource);

	/** @brief Calculate MD5, get MD5 code (not string) */
	static void MD5(void* input, int inputLength,
		unsigned char* output);

	/** @brief Calculate SHA1 with a secret key. */
	static void sha1(unsigned char* input, int inputLength,
		unsigned char* key, int keyLength,
		unsigned char* buffer, int bufferLength);

	/**
	 * @brief URLEncode
	 */
	static std::string urlEncode(const std::string& strPlainText);

	/**
	 * @brief URLDecode
	 */
	static std::string urlDecode(const std::string& strEncryptText);

};

NS_BL_UTIL_END
