#ifndef __USER_DATA_MANAGER_H__
#define __USER_DATA_MANAGER_H__

class UserDataManager
{
public:
	static UserDataManager* getInstance();
	void init();
public:
	/*
	* @brief 获取用户历史最高分
	*/
	int getHighScore();
	/*
	* @brief 设置历史最高分
	*/
	void setHighScore(int nScore);
private:
	/*
	* @brief  从磁盘中载入用户数据
	*/
	void loadData();
	/*
	* @brief 保存用户数据
	*/
	void saveData();
private:
	int m_nHighScore;

};
#endif // !__USER_DATA_MANAGER_H__
