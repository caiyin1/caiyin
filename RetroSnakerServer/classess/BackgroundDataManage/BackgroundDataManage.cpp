
BackgroundDataManage::BackgroundDataManage()
{
	init();
}

BackgroundDataManage::init()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m_nConten[i][j] = 0;
		}
	}
	// 初始化背景数据表 0. 空 、 1 块块、2 food、 3 bar

}

void BackgroundDataManage::initBlock()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_nControlBlock == 1)
		{
			m_nControlBlock--;
			continue;
		}
		int nType = rand() % 100;
		if (nType > 0 && nType < 35)
		{
			// 一行一个
			m_nControlBlock++;
			int nColumn = rand() % 5;
			m_nConten[i][nColumn] = 1;
		}
		else if ( nType < 70 && nType > 35)
		{
			// 一行2个
			// 控制2每两行 必须最多就一行有块块
			m_nControlBlock++;
			int nFirst = -1;
			for (int j = 0; j < 2; j ++)
			{
				int nCoulumn = rand() % 5;
				if ( nCoulumn == nFirst)
				{
					// 位置与第一个重叠时，重新从新创建位置
					j--;
					continue;
				}
				else
				{
					nFirst = nCoulumn;
					m_nConten[i][nFirst] = 1;
				}
			}

		}
		else if ( nType > 70 && nType < 90)
		{
			if (m_nAllBlockNum > 0)
			{
				i--;
				continue;
			}
			// 添加一整行 都是块块
			m_nControlBlock++;
			m_nAllBlockNum++;
			for (int j = 0; j < 5; j++)
			{
				m_nConten[i][j] = 1;
			}
		}
		else
		{
			m_nControlBlock;
		}
	}
}

void BackgroundDataManage::initFood()
{
	for (int i = 0; i < 8; i++)
	{
		int nLineFoodNum = 0;
		for (int j = 0; j < 5; j++)
		{
			if (m_nConten[i][j] == 0)
			{
				int nTpye = rand() % 100;
				// 空的情况下有 15% 的几率有食物
				if (nTpye < 15)
				{
					if ( nLineFoodNum == 2)
					{
						continue;
					}
					m_nConten[i][j] = 2;
					nLineFoodNum++;
				}
			}
		}
	}
}

 