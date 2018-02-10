#include "DataManager.h"
#include "../../bailinUtils/RapidJsonMacro.h"
#include "../Util/archiveMacro.h"
#include "xxtea/xxtea.h"
#include "../../bailinUtils/EncryptUtil.h"
USING_NS_CC;
using namespace rapidjson;


static DataManager* s_global = nullptr;

DataManager* DataManager::getInstance()
{
	if (s_global == nullptr)
	{
		s_global = new DataManager;
		s_global->init();
	}
	return s_global;
}

void DataManager::init()
{

}

const rapidjson::Document& DataManager::getJsonDoc()
{
	return m_readDoc;
}


bool DataManager::isGameRecord()
{
	auto strBuf = UserDefault::getInstance()->getStringForKey(GAME_RECORD_NAME);
	bool  bRet = false;
	if (strBuf.size() != 0)
	{
		bRet = true;
	}
	return bRet;
}

void DataManager::saveRecord(const rapidjson::Value& savejson)
{
	// 储存文件
	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	savejson.Accept(writer);

	auto strBuf = encryptData(buffer.GetString(), buffer.GetSize());
	UserDefault::getInstance()->setStringForKey(GAME_RECORD_NAME, strBuf);
}

bool DataManager::readRecord()
{
	bool bRet = false;
	do
	{
		auto path = FileUtils::getInstance()->getWritablePath();
		path.append(JSON_FILE_NAME);
		// 读取文件
		std::string strReadBuf = UserDefault::getInstance()->getStringForKey(GAME_RECORD_NAME);
		std::string strBuf = decryptData((unsigned char*)strReadBuf.c_str(), strReadBuf.length());
		// 删除记录
		removeGameRecord();
		if (strBuf.length() == 0)
		{
			break;
		}

		m_readDoc.Parse<0>(strBuf.c_str());
		if (m_readDoc.HasParseError())
		{
			CCLOG("getParseError: %d", m_readDoc.GetParseError());
		}
		if (!m_readDoc.IsObject())
		{
			return bRet;
		}
		bRet = true;
	} while (0);
	return bRet;
}


std::string DataManager::encryptData(const char* pBuffer, int nBufSize)
{

	xxtea_long lOutputLen = 0;
	// 加密
	unsigned char* pNewBuf = xxtea_encrypt((unsigned char*)pBuffer, (xxtea_long)nBufSize, (unsigned char*)JSON_KEY, (xxtea_long)strlen(JSON_KEY), &lOutputLen);
	// 转成base64
	std::string strRet = bailin::util::EncryptUtil::encodeBase64((char*)pNewBuf, lOutputLen);
	free(pNewBuf);

	return strRet;
}


std::string DataManager::decryptData(unsigned char* pChBuf, int nBufLen)
{
	int nOutputLen = 0;
	// 装换二进制
	char* pBuffer = bailin::util::EncryptUtil::decodeBase64(pChBuf, nBufLen, nOutputLen);
	// 解码
	xxtea_long lBuflen = 0;
	unsigned char* uPChBuf = xxtea_decrypt((unsigned char*)pBuffer, nOutputLen, (unsigned char*)JSON_KEY, (xxtea_long)strlen(JSON_KEY), &lBuflen);
	std::string strRet;
	strRet.append((char*)uPChBuf, lBuflen);
	free(uPChBuf);
	delete pBuffer;
	return strRet;
}

void DataManager::removeGameRecord()
{
#if COCOS2D_VERSION > 0x0030330
	UserDefault::getInstance()->deleteValueForKey(GAME_RECORD_NAME);
#else
    UserDefault::getInstance()->setStringForKey(GAME_RECORD_NAME, "");
#endif
}


void DataManager::setGameOverCallBack(const std::function<void()>& fun)
{
	m_pGameVoerCallBack = fun;
}

void DataManager::safeModifyData(int& nNum, const std::string& strKey, int nChangeNum, bool bCheck)
{
	if (bCheck)
	{
		std::string strNumToMD5 = bailin::util::EncryptUtil::MD5(StringUtils::format("%d", nNum));
		auto saveMD5 = m_mGameBufferMD5.find(strKey);
		// 交验值是否被改变
		if (strNumToMD5.compare(saveMD5->second) == 0)
		{
			nNum += nChangeNum;
			strNumToMD5 = bailin::util::EncryptUtil::MD5(StringUtils::format("%d", nNum));
			m_mGameBufferMD5.erase(strKey);
			m_mGameBufferMD5.insert(make_pair(strKey, strNumToMD5));
		}
		else
		{
			// 游戏结束
			GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_Result);
			if (m_pGameVoerCallBack)
			{
				m_pGameVoerCallBack();
			}
		}
	}
	else
	{
		nNum = nChangeNum;
		std::string strNumToMD5 = bailin::util::EncryptUtil::MD5(StringUtils::format("%d", nNum));
		if (m_mGameBufferMD5.find(strKey) != m_mGameBufferMD5.end())
		{
			m_mGameBufferMD5.erase(strKey);
		}
		m_mGameBufferMD5.insert(make_pair(strKey, strNumToMD5));
	}
}

void DataManager::safeModifyHighScore(int& nNum, const std::string& strKey, int nChangeNum, bool bCheck)
{
	if (bCheck)
	{
		std::string strNumToMD5 = bailin::util::EncryptUtil::MD5(StringUtils::format("%d", nNum));
		auto saveMD5 = m_mGameBufferMD5.find(strKey);
		// 交验值是否被改变
		if (strNumToMD5.compare(saveMD5->second) == 0)
		{
			nNum = nChangeNum;
		}
		else
		{
			nNum = 0;
		}
		strNumToMD5 = bailin::util::EncryptUtil::MD5(StringUtils::format("%d", nNum));
		m_mGameBufferMD5.erase(strKey);
		m_mGameBufferMD5.insert(make_pair(strKey, strNumToMD5));
	}
	else
	{
		nNum = nChangeNum;
		std::string strNumToMD5 = bailin::util::EncryptUtil::MD5(StringUtils::format("%d", nNum));
		if (m_mGameBufferMD5.find(strKey) != m_mGameBufferMD5.end())
		{
			m_mGameBufferMD5.erase(strKey);
		}
		m_mGameBufferMD5.insert(make_pair(strKey, strNumToMD5));
	}
}
