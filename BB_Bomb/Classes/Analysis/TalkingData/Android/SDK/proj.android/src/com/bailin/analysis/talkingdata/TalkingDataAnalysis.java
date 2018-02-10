package com.bailin.analysis.talkingdata;

import java.util.HashMap;
import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;

import com.tendcloud.tenddata.TDGAAccount;
import com.tendcloud.tenddata.TDGAAccount.AccountType;
import com.tendcloud.tenddata.TDGAAccount.Gender;
import com.tendcloud.tenddata.TalkingDataGA;

public class TalkingDataAnalysis {

	private Activity activity = null;

	static private TalkingDataAnalysis s_instance = null;

	private TDGAAccount tdga_account = null;

	/**
	 * 给JNI调用的
	 * 
	 * @return
	 */
	static public Object getInstance() {
		return s_instance;
	}

	/**
	 * 构造函数
	 * 
	 * @param activity
	 */
	public TalkingDataAnalysis(Activity activity) {
		this.activity = activity;
		s_instance = this;
	}

	/**
	 * 初始化
	 * 
	 * @param strAppID
	 * @param strChannel
	 */
	public void startWithAppKeyAndChannel(String strAppID, String strChannel) {
		TalkingDataGA.init(activity, strAppID, strChannel);
	}

	/**
	 * 设置账号
	 * @param strAccount
	 */
	public void setAccount(String strAccount) {
		tdga_account = TDGAAccount.setAccount(strAccount);
	}

	/**
	 * 设置账号
	 */
	public void setAccountWithDeviceID() {
		tdga_account = TDGAAccount.setAccount(TalkingDataGA
				.getDeviceId(activity));
	}

	/**
	 * 设置昵称
	 * 
	 * @param strName
	 */
	public void setAccountName(String strName) {
		tdga_account.setAccountName(strName);
	}

	/**
	 * 设置昵称
	 */
	public void setAccountNameWithDeviceName() {
		tdga_account.setAccountName(android.os.Build.MODEL);
	}

	/**
	 * 设置账号类型
	 * 
	 * @param nAccountType
	 */
	public void setAccountType(int nAccountType) {
		AccountType t = AccountType.ANONYMOUS;
		switch (nAccountType) {
		case 0:
			t = AccountType.ANONYMOUS;
			break;
		case 1:
			t = AccountType.REGISTERED;
			break;
		case 2:
			t = AccountType.SINA_WEIBO;
			break;
		case 3:
			t = AccountType.QQ;
			break;
		case 4:
			t = AccountType.QQ_WEIBO;
			break;
		case 5:
			t = AccountType.ND91;
			break;
		case 6:
			t = AccountType.TYPE1;
			break;
		case 7:
			t = AccountType.TYPE2;
			break;
		case 8:
			t = AccountType.TYPE3;
			break;
		case 9:
			t = AccountType.TYPE4;
			break;
		case 10:
			t = AccountType.TYPE5;
			break;
		case 11:
			t = AccountType.TYPE6;
			break;
		case 12:
			t = AccountType.TYPE7;
			break;
		case 13:
			t = AccountType.TYPE8;
			break;
		case 14:
			t = AccountType.TYPE9;
			break;
		case 15:
			t = AccountType.TYPE10;
			break;
		}

		tdga_account.setAccountType(t);
	}

	/**
	 * 设置等级
	 * 
	 * @param nLevel
	 */
	void setLevel(int nLevel) {
		tdga_account.setLevel(nLevel);
	}

	/**
	 * 设置年龄
	 * 
	 * @param nAge
	 */
	void setAge(int nAge) {
		tdga_account.setAge(nAge);
	}

	/**
	 * 设置性别
	 * 
	 * @param nGender
	 */
	void setGender(int nGender) {
		Gender gender = Gender.UNKNOW;
		switch (nGender) {
		case 1:
			gender = Gender.MALE;
			break;
		case 2:
			gender = Gender.FEMALE;
			break;
		default:
			gender = Gender.UNKNOW;
			break;
		}
		tdga_account.setGender(gender);
	}

	void setGameServer(String strGameServer) {
		tdga_account.setGameServer(strGameServer);
	}

	/**
	 * 自定义数据
	 * 
	 * @param strEventID
	 * @param strJSONData
	 */
	void event(String strEventID, String strJSONData) {
		HashMap<String, Object> eventData = new HashMap<String, Object>();

		try {
			JSONObject jsonData = new JSONObject(strJSONData);
			@SuppressWarnings("unchecked")
			Iterator<String> iter = jsonData.keys();
			while (iter.hasNext()) {
				String key = (String) iter.next();
				Object value = jsonData.get(key);
				eventData.put(key, value);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}

		TalkingDataGA.onEvent(strEventID, eventData);
	}
}
