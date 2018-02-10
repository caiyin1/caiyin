#include "TalkingDataAnalysis.h"
#import "SDK/TalkingDataGA.h"

static TDGAAccount* tdga_account = nil;

TalkingDataAnalysis* TalkingDataAnalysis::_instance = nullptr;

void TalkingDataAnalysis::startWithAppKey(const std::string &strAppKey)
{
    startWithAppKeyAndChannel(strAppKey, "appstore");
}

void TalkingDataAnalysis::startWithAppKeyAndChannel(const std::string &strAppKey, const std::string &strChannel)
{
    NSString* appID = [NSString stringWithUTF8String:strAppKey.c_str()];
    NSString* appChannel = [NSString stringWithUTF8String:strChannel.c_str()];
    [TalkingDataGA onStart:appID withChannelId:appChannel];
}

void TalkingDataAnalysis::setAccount(const std::string &strAccount)
{
    tdga_account = [TDGAAccount setAccount:[NSString stringWithUTF8String:strAccount.c_str()]];
}

void TalkingDataAnalysis::setAccountWithDeviceID()
{
    tdga_account = [TDGAAccount setAccount:[TalkingDataGA getDeviceId]];
}

void TalkingDataAnalysis::setAccountType(TalkingDataAnalysis::AccountType eType)
{
    switch (eType)
    {
        case AccountType::kType_Anonymous:
            [tdga_account setAccountType:kAccountAnonymous];
            break;
        case AccountType::kType_Registered:
            [tdga_account setAccountType:kAccountRegistered];
            break;
        case AccountType::kType_SinaWeibo:
            [tdga_account setAccountType:kAccountSinaWeibo];
            break;
        case AccountType::kType_TencentWeibo:
            [tdga_account setAccountType:kAccountTencentWeibo];
            break;
        case AccountType::kType_QQ:
            [tdga_account setAccountType:kAccountQQ];
            break;
        case AccountType::kType_ND91:
            [tdga_account setAccountType:kAccountND91];
            break;
        case AccountType::kType_1:
            [tdga_account setAccountType:kAccountType1];
            break;
        case AccountType::kType_2:
            [tdga_account setAccountType:kAccountType2];
            break;
        case AccountType::kType_3:
            [tdga_account setAccountType:kAccountType3];
            break;
        case AccountType::kType_4:
            [tdga_account setAccountType:kAccountType4];
            break;
        case AccountType::kType_5:
            [tdga_account setAccountType:kAccountType5];
            break;
        case AccountType::kType_6:
            [tdga_account setAccountType:kAccountType6];
            break;
        case AccountType::kType_7:
            [tdga_account setAccountType:kAccountType7];
            break;
        case AccountType::kType_8:
            [tdga_account setAccountType:kAccountType8];
            break;
        case AccountType::kType_9:
            [tdga_account setAccountType:kAccountType9];
            break;
        case AccountType::kType_10:
            [tdga_account setAccountType:kAccountType10];
            break;
    }
}

void TalkingDataAnalysis::setAccountName(const std::string &strAccountName)
{
    [tdga_account setAccountName:[NSString stringWithUTF8String:strAccountName.c_str()]];
}
void TalkingDataAnalysis::setAccountNameWithDeviceName()
{
    NSString* name = [UIDevice currentDevice].name;
    // NSString* model = [UIDevice currentDevice].model;
    // NSString* lmodel = [UIDevice currentDevice].localizedModel;
    // NSString* sysname = [UIDevice currentDevice].systemName;
    // NSString* sysv = [UIDevice currentDevice].systemVersion;
    // NSLog(@"%@, %@, %@, %@, %@", name, model, lmodel, sysname, sysv);
    [tdga_account setAccountName:name];
}

void TalkingDataAnalysis::setAge(int nAge)
{
    [tdga_account setAge:nAge];
}

void TalkingDataAnalysis::setLevel(int nLevel)
{
    [tdga_account setLevel:nLevel];
}

void TalkingDataAnalysis::setGender(int nGender)
{
    [tdga_account setGender:(TDGAGender)nGender];
}

void TalkingDataAnalysis::setGameServer(const std::string &strServerName)
{
    [tdga_account setGameServer:[NSString stringWithUTF8String:strServerName.c_str()]];
}

void TalkingDataAnalysis::event(const std::string &strEventId, const std::unordered_map<std::string, std::string> &eventData)
{
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
    
    for (auto iter : eventData)
    {
        [dict setValue:[NSString stringWithUTF8String:iter.second.c_str()] forKey:[NSString stringWithUTF8String:iter.first.c_str()]];
    }
    
    [TalkingDataGA onEvent:[NSString stringWithUTF8String:strEventId.c_str()] eventData:dict];
    
    [dict release];
}

void TalkingDataAnalysis::event(const std::string &strEventId, const std::unordered_map<std::string, long> &eventData)
{
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
    
    for (auto iter : eventData)
    {
        [dict setValue:[NSNumber numberWithLong:iter.second] forKey:[NSString stringWithUTF8String:iter.first.c_str()]];
    }
    
    [TalkingDataGA onEvent:[NSString stringWithUTF8String:strEventId.c_str()] eventData:dict];
    
    [dict release];
}
