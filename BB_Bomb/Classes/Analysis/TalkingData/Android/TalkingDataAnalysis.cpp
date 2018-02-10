#include "TalkingDataAnalysis.h"
#include "RapidJsonMacro.h"
#include "cocos2d.h"

USING_NS_CC;

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

#define JNI_CLASS "com/bailin/analysis/talkingdata/TalkingDataAnalysis"

TalkingDataAnalysis* TalkingDataAnalysis::_instance = nullptr;

void TalkingDataAnalysis::startWithAppKey(const std::string &strAppKey)
{
    startWithAppKeyAndChannel(strAppKey, "android");
}

void TalkingDataAnalysis::startWithAppKeyAndChannel(const std::string &strAppKey, const std::string &strChannel)
{
	do
	{
		CCLOG("jni startWithAppKeyAndChannel");
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "startWithAppKeyAndChannel",   //方法名
											   "(Ljava/lang/String;Ljava/lang/String;)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		jstring jstrAppKey = minfo.env->NewStringUTF(strAppKey.c_str());
		jstring jstrChannel = minfo.env->NewStringUTF(strChannel.c_str());

		minfo.env->CallVoidMethod(jobj, minfo.methodID, jstrAppKey, jstrChannel);

		minfo.env->DeleteLocalRef(jstrAppKey);
		minfo.env->DeleteLocalRef(jstrChannel);
		
	} while (0);
}

void TalkingDataAnalysis::setAccountWithDeviceID()
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setAccountWithDeviceID",   //方法名
											   "()V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	} while (0);
}

void TalkingDataAnalysis::setAccountType(TalkingDataAnalysis::AccountType eType)
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setAccountType",   //方法名
											   "(I)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		minfo.env->CallVoidMethod(jobj, minfo.methodID, (int)eType);
	} while (0);
}

void TalkingDataAnalysis::setAccountName(const std::string &strAccountName)
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setAccountName",   //方法名
											   "(Ljava/lang/String;)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		jstring jstrName = minfo.env->NewStringUTF(strAccountName.c_str());

		minfo.env->CallVoidMethod(jobj, minfo.methodID, jstrName);

		minfo.env->DeleteLocalRef(jstrName);
	} while (0);
}

void TalkingDataAnalysis::setAccountNameWithDeviceName()
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setAccountNameWithDeviceName",   //方法名
											   "()V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	} while (0);
}

void TalkingDataAnalysis::setAge(int nAge)
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setAge",   //方法名
											   "(I)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		minfo.env->CallVoidMethod(jobj, minfo.methodID, nAge);
	} while (0);

}

void TalkingDataAnalysis::setLevel(int nLevel)
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setLevel",   //方法名
											   "(I)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		minfo.env->CallVoidMethod(jobj, minfo.methodID, nLevel);
	} while (0);
	
}

void TalkingDataAnalysis::setGender(int nGender)
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setGender",   //方法名
											   "(I)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		minfo.env->CallVoidMethod(jobj, minfo.methodID, nGender);
	} while (0);
	
}

void TalkingDataAnalysis::setGameServer(const std::string &strServerName)
{
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "setGameServer",   //方法名
											   "(Ljava/lang/String;)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		jstring jstrServerName = minfo.env->NewStringUTF(strServerName.c_str());

		minfo.env->CallVoidMethod(jobj, minfo.methodID, jstrServerName);

		minfo.env->DeleteLocalRef(jstrServerName);
		
	} while (0);
	
}

void TalkingDataAnalysis::event(const std::string &strEventId, const std::unordered_map<std::string, std::string> &eventData)
{
	// 把数据先转换成 JSON 串
	rapidjson::Value jsonData(rapidjson::kObjectType);
	rapidjson::Value::AllocatorType at;
	for (auto iter : eventData)
	{
		jsonData.AddMember(iter.first.c_str(), iter.second.c_str(), at);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsonData.Accept(writer);

	std::string strJSONData = buffer.GetString();
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "event",   //方法名
											   "(Ljava/lang/String;Ljava/lang/String;)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		jstring jstrEventId = minfo.env->NewStringUTF(strEventId.c_str());
		jstring jstrEventData = minfo.env->NewStringUTF(strJSONData.c_str());

		minfo.env->CallVoidMethod(jobj, minfo.methodID, jstrEventId, jstrEventData);

		minfo.env->DeleteLocalRef(jstrEventId);
		minfo.env->DeleteLocalRef(jstrEventData);
		
	} while (0);
}

void TalkingDataAnalysis::event(const std::string &strEventId, const std::unordered_map<std::string, long> &eventData)
{
	// 把数据先转换成 JSON 串
	rapidjson::Value jsonData(rapidjson::kObjectType);
	rapidjson::Value::AllocatorType at;
	for (auto iter : eventData)
	{
		jsonData.AddMember(iter.first.c_str(), (int)iter.second, at);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsonData.Accept(writer);

	std::string strJSONData = buffer.GetString();
	do
	{
		// 安卓平台
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		
		// 1.得到调用JNI函数对应的对象
		bool bHaveMethod = JniHelper::getStaticMethodInfo(minfo,
														  JNI_CLASS,  //类的路径
														  "getInstance",   //方法名
														  "()Ljava/lang/Object;");   //括号里的是参数，后面的是返回值。
		CC_BREAK_IF(!bHaveMethod);
		jobject jobj;
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		
		// 2. 设置 APPID 和 渠道
		bHaveMethod = JniHelper::getMethodInfo(minfo,
											   JNI_CLASS,  //类的路径
											   "event",   //方法名
											   "(Ljava/lang/String;Ljava/lang/String;)V");   //括号里的是参数，后面的是返回值
		
		CC_BREAK_IF(!bHaveMethod);
		jstring jstrEventId = minfo.env->NewStringUTF(strEventId.c_str());
		jstring jstrEventData = minfo.env->NewStringUTF(strJSONData.c_str());

		minfo.env->CallVoidMethod(jobj, minfo.methodID, jstrEventData);

		minfo.env->DeleteLocalRef(jstrEventId);
		minfo.env->DeleteLocalRef(jstrEventData);
		
	} while (0);
}
