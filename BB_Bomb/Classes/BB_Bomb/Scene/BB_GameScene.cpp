#include "BB_GameScene.h"
#include "../Manager/GameDeploy.h"
#include "../Manager/GameStatusManager.h"
#include "../Manager/BombDataManager.h"
#include "../Manager/FortDataManager.h"
#include "../Manager/BackgroundDataManager.h"
#include "../Manager/UserDataManager.h"
#include "../UI/BlockNode.h"
#include "../UI/FoodNode.h"
#include "../Util/gbk2utf8.h"
#include "../Util/ColorUtil.h"
#include "extensions/cocos-ext.h"  
USING_NS_CC_EXT;
USING_NS_CC;

/*
* @brief 根据点数得到不同的颜色
* @param Block的点数
*/
static Color3B getBlockColorWithNumer(int nNum)
{
	double h = 0.45;
	double s = 1.0;
	double l = 0.48;
	float fRate = nNum * 1.0f / 60;
	if (fRate > 1.0)
		fRate = 1.0;
	h = fRate;
	return blutil::HSL2RGB(h, s, l);
}

Scene* BB_GameScene::create()
{
	BB_GameScene* pRet = new BB_GameScene;
	if (pRet && pRet->init() && pRet->initWithPhysics())
	{
		pRet->autorelease();
#if COCOS2D_DEBUG > 0
		pRet->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool BB_GameScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Scene::init());
		// 设置屏幕缩放比
		GameDeploy::getInstance()->setScalingRatio(Director::getInstance()->getContentScaleFactor());
		// 添加一个游戏开始自定义事件
		_eventDispatcher->addCustomEventListener(START_GAME_EVENT, CC_CALLBACK_1(BB_GameScene::onStartGameEvent, this));
		// 开启碰撞回调事件回调函数
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(BB_GameScene::onContactBegin, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
		// 设置添加背景回调函数
		BackgroundDataManager::getInstance()->setAddBackgroundCallBack(CC_CALLBACK_1(BB_GameScene::addBackgroundCallBack, this));
		initHomeLayer();
		bRet = true;
	} while (0);
	return bRet;
}

void BB_GameScene::onEnter()
{
	Scene::onEnter();

	createTouchListener();
}

void BB_GameScene::createTouchListener()
{
	m_pEventListenerTouch = EventListenerTouchOneByOne::create();
	// 
	m_pEventListenerTouch->onTouchBegan = CC_CALLBACK_2(BB_GameScene::onTouchBegin, this);
	m_pEventListenerTouch->onTouchMoved = CC_CALLBACK_2(BB_GameScene::onTouchMoved, this);
	m_pEventListenerTouch->onTouchEnded = CC_CALLBACK_2(BB_GameScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pEventListenerTouch, this);

}

bool BB_GameScene::onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	// 获取游戏状态
	auto gameStatus = GameStatusManager::getInstance()->getGameStatus();
	if (gameStatus == GameStatusManager::GameStatus::kStatus_ReadyShoot)
	{
		m_fPosX = touch->getLocation().x;
		auto pos = touch->getLocation();

		// 设置箭头向量的位置
		// 添加提示箭头
		m_pFortNode->addArrow(pos);
		CCLOG("onTouchBegan Pos.y(%f)", pos.y);
		return true;
	}
	else
	{
		return false;
	}
}

void BB_GameScene::initHomeLayer()
{
	m_pHomeLayer = Layer::create();
	addChild(m_pHomeLayer);
	// 获取玩家数据
	auto pUserData = UserDataManager::getInstance();

	// 添加一个点击任意一处 开始
	auto pStartMenu = Menu::create();
	auto pStartButton = MenuItemImage::create();
	pStartButton->setContentSize(_contentSize);
	pStartButton->setCallback(CC_CALLBACK_1(BB_GameScene::menuStartCallBack, this));
	pStartMenu->addChild(pStartButton);
	m_pHomeLayer->addChild(pStartMenu);
	// 添加游戏名称
	float fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	auto pGameNameLabel = Label::createWithSystemFont(bailinText("BB弹"), FNT_NAME, 400 / fScalingRatio);

	pGameNameLabel->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.8f);
	m_pHomeLayer->addChild(pGameNameLabel);
	// 黄冠
	// 添加历史最高分前面的皇冠
	auto pCrownSprite = Sprite::create("res/BB_Bomb/Image/bb_crown.png");
	pCrownSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	pCrownSprite->setPosition(10 / fScalingRatio, 0);
	// 设置颜色
	pCrownSprite->setColor(Color3B(255, 253, 16));
	// 添加蛇历史最高分
	auto pHistoryScoreLabel = Label::createWithSystemFont(StringUtils::format("%d", pUserData->getHighScore()), FNT_NAME, 100 / fScalingRatio);
	pHistoryScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	pHistoryScoreLabel->setColor(Color3B(255, 253, 16));
	// 创建一个Node 使score和皇冠永远居中
	auto pNode = Node::create();
	// 获取皇冠的boundingBox
	auto crownRect = pCrownSprite->getBoundingBox();
	// 获取scoreLabel的boundingBox
	auto scoreLabelRect = pHistoryScoreLabel->getBoundingBox();
	// 设置Node的大小时 皇冠和score的长度再加上固定的间距
	pNode->setContentSize(Size(crownRect.size.width + scoreLabelRect.size.width + 30 / fScalingRatio, crownRect.size.height));
	pNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pNode->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.55f);
	m_pHomeLayer->addChild(pNode);
	pHistoryScoreLabel->setPosition(pNode->getContentSize().width, crownRect.size.height * 0.5f);
	pNode->addChild(pCrownSprite);
	pNode->addChild(pHistoryScoreLabel);

	std::string strTipsImage = "res/BB_Bomb/Image/bb_home_start.png";
	auto pStartSprite = createPrompt(strTipsImage);
	pStartSprite->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.3f);
	m_pHomeLayer->addChild(pStartSprite);
}

void BB_GameScene::menuStartCallBack(cocos2d::Ref* spender)
{
	m_pHomeLayer->removeFromParent();
	// 发送游戏开始的事件
	this->_eventDispatcher->dispatchCustomEvent(START_GAME_EVENT);
}

void BB_GameScene::onStartGameEvent(cocos2d::EventCustom* event)
{
	// 游戏开始
	// 改变游戏状态 
	GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_AddBack);
	// 初始化游戏数据
	initGameData();
	// 初始化游戏Layer
	initGameLayer();
	// 开启Update
	scheduleUpdate();
	// 关闭子弹碰撞的自动检测
	this->getPhysicsWorld()->setAutoStep(false);
}

void BB_GameScene::initGameData()
{
	m_nScore = 1;
	m_nMoveBombNum = 0;
	m_nAlreadyShootBombNum = 0;
	m_nTimer = 0;
	m_bMoveFort = false;
	// 初始化子弹数据
	BombDataManager::getInstance()->init();
	// 初始化炮台数据
	FortDataManager::getInstance()->init();
	// 初始化游戏配置信息
	GameDeploy::getInstance()->init();
	// 初始化背景数据
	BackgroundDataManager::getInstance()->init();
}

void BB_GameScene::onExit()
{
	// 是定义通知事件
	// 促销事件
	_eventDispatcher->removeEventListener(m_pEventListenerTouch);
	Scene::onExit();
}

void BB_GameScene::initGameLayer()
{
	m_pGameLayer = Layer::create();
	addChild(m_pGameLayer);
	// 创建碰撞边界线刚体
	createBroundLine();
	// 初始化炮台
	m_pFortNode = FortNode::create();
	m_pFortNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_pFortNode->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.1f);
	m_pGameLayer->addChild(m_pFortNode, 2);
	// 设置炮台位置
	FortDataManager::getInstance()->setFortPosition(m_pFortNode->getPosition());
	// 初始化子弹Node
	m_pBombNode = Node::create();
	m_pBombNode->setContentSize(_contentSize);
	m_pBombNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pBombNode->setPosition(_contentSize * 0.5f);
	m_pGameLayer->addChild(m_pBombNode);
	//// 添加一个子弹
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	// 初始化食物Node
	m_pFoodNode = Node::create();
	m_pFoodNode->setContentSize(_contentSize);
	m_pFoodNode->setPosition(_contentSize * 0.5f);
	m_pFoodNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pGameLayer->addChild(m_pFoodNode);
	// 初始化Block的Node
	m_pBlockNode = Node::create();
	m_pBlockNode->setContentSize(_contentSize);
	m_pBlockNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pBlockNode->setPosition(_contentSize * 0.5f);
	m_pGameLayer->addChild(m_pBlockNode);
	// 初始化玩家得分Label
	m_pScoreLabel = Label::createWithBMFont(NUM_FILE, StringUtils::format("score : %d", m_nScore));
	m_pScoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	m_pScoreLabel->setPosition(_contentSize.width - 20 / fScalingRatio, _contentSize.height - 20 / fScalingRatio);
	m_pScoreLabel->setColor(Color3B::WHITE);
	m_pGameLayer->addChild(m_pScoreLabel);
	// 初始化边界线
	// 上边界线
	auto pTopDraw = DrawNode::create();
	pTopDraw->drawSolidRect(Vec2(0, _contentSize.height * 0.9),
		Vec2(_contentSize.width, _contentSize.height * 0.9 + 10 / fScalingRatio), Color4F::RED);
	m_pGameLayer->addChild(pTopDraw);
	// 下边界
	auto pButtonDraw = DrawNode::create();
	pButtonDraw->drawSolidRect(Vec2(0, _contentSize.height * 0.1f),
		Vec2(_contentSize.width, _contentSize.height * 0.1f - 10 / fScalingRatio), Color4F::RED);
	m_pGameLayer->addChild(pButtonDraw);

	createRecyclingBombButton();

}

void BB_GameScene::createBroundLine()
{
	// 上边
	auto pTopBroundLine = PhysicsBody::createEdgeSegment(Vec2(0, _contentSize.height * 0.9f + 50),
		Vec2(_contentSize.width, _contentSize.height * 0.9f + 50), PhysicsMaterial(0.0f, 1.0f, 0.0f), 50);
	pTopBroundLine->setDynamic(false);
	// 设置遮掩码
	pTopBroundLine->setCategoryBitmask(2);
	pTopBroundLine->setContactTestBitmask(8);
	pTopBroundLine->setCollisionBitmask(4);
	// 创建存放边界的Node
	auto pNodeTop = Node::create();
	pNodeTop->setContentSize(Size(0, 0));
	pNodeTop->setPosition(0, 0);
	pNodeTop->setPhysicsBody(pTopBroundLine);
	m_pGameLayer->addChild(pNodeTop);

	// 左边
	auto pLeftBroundLine = PhysicsBody::createEdgeSegment(Vec2(0, _contentSize.height * 0.1f),
		Vec2(0, _contentSize.height * 0.9f), PhysicsMaterial(0.0f, 1.0f, 0.0f));
	// 设置遮掩码
	pLeftBroundLine->setCategoryBitmask(2);
	pLeftBroundLine->setContactTestBitmask(8);
	pLeftBroundLine->setCollisionBitmask(4);
	// 创建存放边界的Node
	auto pNodeLeft = Node::create();
	pNodeLeft->setPosition(0, 0);
	pNodeLeft->setPhysicsBody(pLeftBroundLine);
	m_pGameLayer->addChild(pNodeLeft);

	// 右边
	auto pRightBroundLine = PhysicsBody::createEdgeSegment(Vec2(_contentSize.width, _contentSize.height * 0.1f),
		Vec2(_contentSize.width, _contentSize.height * 0.9f), PhysicsMaterial(0.0f, 1.0f, 0.0f));
	// 设置遮掩码
	pRightBroundLine->setCategoryBitmask(2);
	pRightBroundLine->setContactTestBitmask(8);
	pRightBroundLine->setCollisionBitmask(4);
	// 创建存放边界的Node
	auto pNodeRight = Node::create();
	pNodeRight->setPosition(0, 0);
	pNodeRight->setPhysicsBody(pRightBroundLine);
	m_pGameLayer->addChild(pNodeRight);

	//// 下边
	auto pBottomBroundLine = PhysicsBody::createEdgeSegment(Vec2(0, _contentSize.height * 0.1f),
		Vec2(_contentSize.width, _contentSize.height * 0.1f), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	// 设置遮掩码
	pBottomBroundLine->setCategoryBitmask(2);
	pBottomBroundLine->setContactTestBitmask(4);
	pBottomBroundLine->setCollisionBitmask(4);
	pBottomBroundLine->setTag(BOUNDING_TAG_NUM);
	// 创建存放边界的Node
	auto pNodeBottom = Node::create();
	pNodeBottom->setPosition(0, 0);
	pNodeBottom->setPhysicsBody(pBottomBroundLine);
	m_pGameLayer->addChild(pNodeBottom);
}

void BB_GameScene::update(float dt)
{
	// 开启检测欧碰撞
	for (int i = 0; i < 3; ++i)
	{
		this->getPhysicsWorld()->step(1 / 180.0f);
	}
	// 获取游戏状态
	auto gameStatus = GameStatusManager::getInstance()->getGameStatus();
	if (gameStatus == GameStatusManager::GameStatus::kStatus_Result)
	{
		return;
	}
	// 获取游戏配置
	auto pGameDeploy = GameDeploy::getInstance();
	int nBombLaunchTimer = pGameDeploy->getBombShootTimer();
	// 判断是否开启子弹回收按钮
	if (gameStatus == GameStatusManager::GameStatus::kStatus_Runnning)
	{
		setRecyclingBombEnable(true);
	}
	else
	{
		setRecyclingBombEnable(false);
	}
	// 创建子弹
	handleAddBomb();
	// 发射子弹
	handleShootBomb();
	// 处理游戏背景数据
	handleAddBackground();
	// 处理屏幕屏幕内容下移动
	handleRollScreen();
}

void BB_GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	auto pos = touch->getLocation();
	m_pFortNode->setArrowRotation(pos);
}

void BB_GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	// 删除箭头
	m_pFortNode->removeArrow();
	// 获取弧度
	auto fRadian = m_pFortNode->getArrowRadian();
	// 判断是否有效的弧度
	float fAngle = CC_RADIANS_TO_DEGREES(fRadian);
	if (fAngle < 20.0f || fAngle > 160.0f)
	{
		// 不符合射击角度不发射
		return;
	}
	// 改变游戏状态
	GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_Shoot);
	// 设置速度
	auto pFortMangager = FortDataManager::getInstance();
	pFortMangager->setBombSpeed(fRadian);
	// 开启炮台的移动
	m_bMoveFort = true;
	// 设置飞行的子弹
	m_nMoveBombNum = m_nScore;
	CCLOG("m_nMoveBombNum (%f)", m_nMoveBombNum);
}

bool BB_GameScene::onContactBegin(cocos2d::PhysicsContact& contac)
{
	// 获取游戏状态数据
	auto pGameStatus = GameStatusManager::getInstance();
	if (pGameStatus->getGameStatus() == GameStatusManager::GameStatus::kStatus_AddBack)
	{
		return false;
	}
	PhysicsBody* a = contac.getShapeA()->getBody();
	PhysicsBody* b = contac.getShapeB()->getBody();
	Node* pBombNode = nullptr;
	Node* pNode = nullptr;
	int nBombNum = a->getTag();
	int nBNum;
	if (nBombNum != BOMB_TAG_NUM)
	{
		nBombNum = b->getTag();
		nBNum = a->getTag();
		pNode = a->getNode();
		pBombNode = b->getNode();
	}
	else
	{
		nBNum = b->getTag();
		pNode = b->getNode();
		pBombNode = a->getNode();
	}
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	switch (nBNum)
	{
	case BLOCK_TAG_NUM:
	{
		if (pNode == nullptr)
		{
			break;
		}
		auto pBlockNode = dynamic_cast<BlockNode*>(pNode->getParent());
		pBlockNode->hitted();
		// 获取Block的Hp
		auto nHp = pBlockNode->getBLockHp();
		if (nHp <= 0)
		{
			auto pos = pBlockNode->getPosition();
			pBlockNode->removeFromParent();
			// 播放爆炸效果
			auto pParticleRatio = ParticleSystemQuad::create("res/BB_Bomb/particle/bb_block_explode.plist");
			auto nNum = random(0, 60);
			CCLOG("nNum(%d)", nNum);
			pParticleRatio->setStartColorVar(Color4F(getBlockColorWithNumer(nNum)));
			pParticleRatio->setPosition(pos);
			pParticleRatio->setCascadeColorEnabled(true);
			// pParticleRatio->setEndColorVar(Color4F(getBlockColorWithNumer(random(0, 60))));
			pParticleRatio->setScale(1 / fScalingRatio);
			pParticleRatio->setLife(4);
			pParticleRatio->setAutoRemoveOnFinish(true);
			addChild(pParticleRatio);
		}
	}
	break;
	case BOUNDING_TAG_NUM:
	{
		m_nMoveBombNum--;
		// 获取子弹管理类
		auto pBombDataManager = BombDataManager::getInstance();
		// 获取炮台位置 移动到炮台的位置
		// 判断本回合是否结束
		if (m_nMoveBombNum <= 0)
		{
			// 改变游戏状态
			pGameStatus->setGameStatus(GameStatusManager::GameStatus::kStatus_AddBack);
			// 清空数据
			pBombDataManager->clearFlyBomb();
		}
		a->setVelocity(Vec2(0, 0));
		// 移动炮台
		if (m_bMoveFort)
		{
			auto pANode = a->getNode();
			auto pBombSprite = dynamic_cast<BombNode*>(pANode);
			// 从飞行的子弹数组中删除这个子弹
			pBombDataManager->removeFlyBombToVector(pBombSprite);
			auto pos = a->getPosition();
			auto pMoveTo = MoveTo::create(0.5f, Vec2(pos.x, _contentSize.height * 0.1f + 10 / fScalingRatio));
			auto delayTime = DelayTime::create(0.1f);
			auto seq = Sequence::create(delayTime, pMoveTo, nullptr);
			pBombSprite->runAction(seq);
			moveFort(pos.x);
			m_bMoveFort = false;
		}
		else
		{
			// 获取炮台坐标
			auto pos = FortDataManager::getInstance()->getForPosition();
			// 移动子弹
			auto pBombNode = dynamic_cast<BombNode*>(a->getNode());
			// 从飞行的子弹数组中删除这个子弹
			pBombDataManager->removeFlyBombToVector(pBombNode);

			pBombNode->setBombStatus(BombNode::BombStatus::Status_MoveTo);
			ActionInterval* pMoveTo = MoveTo::create(0.5f, Vec2(pos.x, _contentSize.height * 0.1f + 10 / fScalingRatio));
			auto delayTime = DelayTime::create(0.1f);
			auto seq = Sequence::create(delayTime, pMoveTo, nullptr);
			pBombNode->runAction(seq);
		}
	}
	break;
	case FOOD_TAG_NUM:
	{
		if (pNode == NULL) break;;
		auto pFoodNode = dynamic_cast<FoodNode*>(pNode->getParent());
		pFoodNode->removeFromParent();
		// 得分加一
		m_nScore++;
		m_pScoreLabel->setString(StringUtils::format("score : %d", m_nScore));
	}
	break;
	}
	return true;
}

void BB_GameScene::handleShootBomb()
{
	// 获取游戏状态
	auto pGameStatusManager = GameStatusManager::getInstance();
	auto gameStatus = pGameStatusManager->getGameStatus();
	// 获取游戏配置
	auto pGameDeploy = GameDeploy::getInstance();
	// 获取子弹发送间隔
	int nShootTimer = pGameDeploy->getBombShootTimer();
	// 检测是否处于发射状态行	
	if (gameStatus == GameStatusManager::GameStatus::kStatus_Shoot)
	{

		if (m_nTimer > nShootTimer)
		{
			auto pBombManger = BombDataManager::getInstance();
			auto vBomb = pBombManger->getBombVector();

			// 发射子弹
			m_nTimer -= nShootTimer;
			// 获取炮台数据
			auto pFortDataManger = FortDataManager::getInstance();
			// 获取子弹速度
			auto bombSpeed = pFortDataManger->getBombSpeed();
			if (m_nAlreadyShootBombNum < pFortDataManger->getBombSize())
			{
				// 获取炮台的位置
				// auto fX = m_pFortNode->getPositionX();
				auto pBombNode = vBomb[m_nAlreadyShootBombNum];
				pBombNode->setBombSpeed(bombSpeed);
				pBombNode->setBombStatus(BombNode::BombStatus::Status_Move);
				// 已经发射的子弹加一
				m_nAlreadyShootBombNum++;
				// 记录到飞行的子弹数组中
				pBombManger->addFlyBombToVector(pBombNode);
			}
			else
			{
				// 子弹已经发射
				// 改变游戏状态
				pGameStatusManager->setGameStatus(GameStatusManager::GameStatus::kStatus_Runnning);
				// 发射子弹置零
				m_nAlreadyShootBombNum = 0;
			}
		}
		else
		{
			m_nTimer++;
		}
	}
}

void BB_GameScene::handleAddBackground()
{
	// 获取游戏状态	
	auto pGameStatus = GameStatusManager::getInstance();
	auto gameStatus = pGameStatus->getGameStatus();
	if (gameStatus == GameStatusManager::GameStatus::kStatus_AddBack)
	{
		auto pBackgroundData = BackgroundDataManager::getInstance();
		pBackgroundData->addBackData();
	}
}

void BB_GameScene::addBackgroundCallBack(cocos2d::Node* pNode)
{
	float fSize = BLOCK_SIZE / GameDeploy::getInstance()->getScalingRatio();
	pNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pNode->setPositionY(_contentSize.height * 0.9f - fSize * 0.5f);
	int nNum = pNode->getTag();
	switch (nNum)
	{
	case BLOCK_TAG_NUM:
		m_pBlockNode->addChild(pNode);
		break;
	case FOOD_TAG_NUM:
		m_pFoodNode->addChild(pNode);
		break;
	default:
		break;
	}
}

void BB_GameScene::handleRollScreen()
{
	auto pGameStatus = GameStatusManager::getInstance();
	if (pGameStatus->getGameStatus() != GameStatusManager::GameStatus::kStatus_AddBack)
	{
		return;
	}
	// 所有内容下移
	float fSize = BLOCK_SIZE / GameDeploy::getInstance()->getScalingRatio();
	auto vBlock = m_pBlockNode->getChildren();
	for (int i = 0; i < vBlock.size(); i++)
	{
		auto pBlockNode = vBlock.at(i);
		auto blockPos = pBlockNode->getPosition();
		float fY = blockPos.y - fSize;
		auto pMoveTo = MoveTo::create(1, Vec2(blockPos.x, fY));
		auto delayTime = DelayTime::create(0.5f);
		auto seq = Sequence::create(delayTime, pMoveTo, nullptr);
		pBlockNode->runAction(seq);
		if (fY <= _contentSize.height * SCREEN_BUTTOM_COFFICIENT)
		{
			// 游戏结束 
			handleGameOver();
			return;
		}
	}

	// 食物一下移
	auto vFood = m_pFoodNode->getChildren();
	for (int i = 0; i < vFood.size(); i++)
	{
		auto pFoodNode = vFood.at(i);
		auto foodPos = pFoodNode->getPosition();
		float fY = foodPos.y - fSize;
		auto pMoveTo = MoveTo::create(1, Vec2(foodPos.x, fY));
		auto delayTime = DelayTime::create(0.5f);
		auto seq = Sequence::create(delayTime, pMoveTo, nullptr);
		pFoodNode->runAction(seq);
		if (fY <= _contentSize.height * SCREEN_BUTTOM_COFFICIENT)
		{
			pFoodNode->removeFromParent();
		}
	}
	// 改变游戏状态
	GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_AddBomb);
}

void BB_GameScene::handleAddBomb()
{
	// 获取游戏状态
	auto pGameStatusManager = GameStatusManager::getInstance();
	auto gameStatus = pGameStatusManager->getGameStatus();
	if (gameStatus != GameStatusManager::GameStatus::kStatus_AddBomb)
	{
		return;
	}
	auto pBombDataManager = BombDataManager::getInstance();
	auto vBomb = pBombDataManager->getBombVector();
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	auto pos = FortDataManager::getInstance()->getForPosition();
	// 创建子弹
	for (int i = vBomb.size(); i < m_nScore; i++)
	{
		auto pBombSprite = BombDataManager::getInstance()->addBombNode();
		pBombSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		pBombSprite->setPosition(pos.x, _contentSize.height * 0.1f + 10 / fScalingRatio);
		pBombSprite->setBombSpeed(Vec2(0.0f, 0.0f));
		m_pBombNode->addChild(pBombSprite);
	}

	// 改为准备射击状态
	pGameStatusManager->setGameStatus(GameStatusManager::GameStatus::kStatus_ReadyShoot);
}

void BB_GameScene::moveFort(const float fX)
{
	float fY = m_pFortNode->getPositionY();
	// 移动炮台
	ActionInterval* pMoveTo = MoveTo::create(0.5f, Vec2(fX, fY));
	m_pFortNode->runAction(pMoveTo);
	// 记录炮台的位置
	FortDataManager::getInstance()->setFortPosition(Vec2(fX, fY));
}

void BB_GameScene::handleGameOver()
{
	clearPhysicsBody();
	// 删除游戏界面
	m_pGameLayer->removeFromParent();
	// 改变游戏状态
	GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_Result);
	// 获取游戏界面
	float fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	// 显示结束界面
	m_pGameOverLayer = LayerColor::create();
	m_pGameOverLayer->setColor(Color3B::BLACK);
	m_pGameOverLayer->setOpacity(0.4 * 255);
	m_pGameOverLayer->ignoreAnchorPointForPosition(false);
	m_pGameOverLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pGameOverLayer->setPosition(_contentSize * 0.5f);
	addChild(m_pGameOverLayer, 2);

	// 创建一个点击任意地方返回首页
	auto pHomeMenu = Menu::create();
	auto pHomeButton = MenuItemImage::create();
	pHomeButton->setContentSize(_contentSize);
	pHomeButton->setCallback(CC_CALLBACK_1(BB_GameScene::menuHomeCallBack, this));
	pHomeMenu->addChild(pHomeButton);
	m_pGameOverLayer->addChild(pHomeMenu);
	auto pUserData = UserDataManager::getInstance();
	// 检测是否打破记录
	int nHistoryHighScore = pUserData->getHighScore();
	if (nHistoryHighScore < m_nScore)
	{
		pUserData->setHighScore(m_nScore);
		nHistoryHighScore = m_nScore;
	}

	// 添加一个大的Block
	auto pS9ScoreBoard = ui::Scale9Sprite::create("res/BB_Bomb/Image/bb_end_block.png");
#if defined(SVB_VERTICAL_SCREEN)
	pS9ScoreBoard->setPreferredSize(Size(_contentSize.width * 0.8f, _contentSize.width * 0.6f));
	pS9ScoreBoard->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.6f);
#else
	pS9ScoreBoard->setPreferredSize(Size(_contentSize.height * 0.8f, _contentSize.height * 0.6f));
	pS9ScoreBoard->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.57f);
#endif
	pS9ScoreBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pGameOverLayer->addChild(pS9ScoreBoard);
	// 获取Board的尺寸
	auto boardSize = pS9ScoreBoard->getContentSize();
	// 显示玩家得分
	auto pScoreLabel = Label::createWithSystemFont(StringUtils::format("%d", m_nScore), FNT_NAME, 250 / fScalingRatio);
	pScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	pScoreLabel->setTextColor(Color4B::BLACK);
	pScoreLabel->setPosition(boardSize.width * 0.5f, boardSize.height * 0.48f);
	pS9ScoreBoard->addChild(pScoreLabel);
	// 添加历史最高分前面的皇冠
	auto pCrownSprite = Sprite::create("res/BB_Bomb/Image/bb_crown.png");
	pCrownSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	pCrownSprite->setColor(Color3B(255, 253, 16));
	// 显示历史最高分
	auto pHistorHighScoreLabel = Label::createWithSystemFont(StringUtils::format("%d", nHistoryHighScore), FNT_NAME, 100 / fScalingRatio);
	pHistorHighScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	pHistorHighScoreLabel->setTextColor(Color4B(255, 253, 16, 255));
	// 获取皇冠的boundingBox
	auto crownRect = pCrownSprite->getBoundingBox();
	// 获取scoreLabel的boundingBox
	auto scoreLabelRect = pHistorHighScoreLabel->getBoundingBox();

	// 添加输出分数弹框
	auto pS9ScoreBomb = ui::Scale9Sprite::create("res/BB_Bomb/Image/bb_end_block.png");
	auto fMarginX = 100 / fScalingRatio;
	auto fMarginY = 30 / fScalingRatio;
	auto fMarginCenter = 40 / fScalingRatio;
	pS9ScoreBomb->setPreferredSize(Size(crownRect.size.width + scoreLabelRect.size.width +
		fMarginCenter + 2 * fMarginX,
		crownRect.size.height + 2 * fMarginY));
	pS9ScoreBomb->setPosition(boardSize.width * 0.5f, boardSize.height * 0.09f);
	pS9ScoreBomb->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	pS9ScoreBomb->setColor(Color3B::BLACK);
	pS9ScoreBoard->addChild(pS9ScoreBomb);

	// 设置历史最高分Label的位置
	pHistorHighScoreLabel->setPosition(pS9ScoreBomb->getBoundingBox().size.width - fMarginX,
		crownRect.size.height * 0.5f + fMarginY);
	pS9ScoreBomb->addChild(pHistorHighScoreLabel);
	// 设置皇冠的位置
	pCrownSprite->setPosition(10 / fScalingRatio + fMarginX, fMarginY);
	pS9ScoreBomb->addChild(pCrownSprite);

}

void BB_GameScene::menuHomeCallBack(cocos2d::Ref* spender)
{
	// 删除结束界面的Layer 
	m_pGameOverLayer->removeFromParent();
	// 
	initHomeLayer();
}

void BB_GameScene::clearPhysicsBody()
{
	// 获取子弹刚体
	auto pBombDataManager = BombDataManager::getInstance();
	auto vBomb = pBombDataManager->getBombVector();
	for (int i = 0; i < vBomb.size(); i++)
	{
		vBomb[i]->removePhysicsBody();
		vBomb[i]->removeFromParent();
	}
	// 
	pBombDataManager->init();
	// 获取炮台信息
}

cocos2d::Sprite* BB_GameScene::createPrompt(const std::string& strFileImage)
{
	float fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	// 添加任意键点击开始图片
	auto pStartSprite = Sprite::create(strFileImage);
	auto pAnimation = Animation::create();
	// 创建帧动画
	auto pArrowRightSprite = Sprite::create("res/BB_Bomb/Image/bb_arrow1.png");
	pArrowRightSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	auto startRect = pStartSprite->getBoundingBox();
	pArrowRightSprite->setPosition(startRect.size.width + 40 / fScalingRatio,
		0.5f * startRect.size.height);
	pStartSprite->addChild(pArrowRightSprite);
	for (int i = 0; i < 4; i++)
	{
		pAnimation->addSpriteFrameWithFile(StringUtils::format("res/BB_Bomb/Image/bb_arrow%d.png", i + 1));
	}
	pAnimation->setDelayPerUnit(1 / 4.0f);
	auto action = Animate::create(pAnimation);
	pArrowRightSprite->runAction(RepeatForever::create(action));

	auto pArriwLeftSprite = Sprite::create("res/BB_Bomb/Image/bb_arrow1.png");
	pArrowRightSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pArriwLeftSprite->setRotation(180);
	pArriwLeftSprite->setPosition(-40 / fScalingRatio,
		0.5f * startRect.size.height);
	pArriwLeftSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pStartSprite->addChild(pArriwLeftSprite);
	pArriwLeftSprite->runAction(RepeatForever::create(action->clone()));
	return pStartSprite;
}

void BB_GameScene::menuRecyclingBombCallBack(cocos2d::Ref* spender)
{
	// 获取飞行中的子弹数组
	auto pBombDataManager = BombDataManager::getInstance();
	auto vFlyBomb = pBombDataManager->getFlyBombVector();
	// 获取炮台数据
	auto pFortDataManager = FortDataManager::getInstance();
	auto pos = pFortDataManager->getForPosition();
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	for (auto i = 0; i < vFlyBomb.size(); i++)
	{
		auto pBombSprite = vFlyBomb[i];
		auto pMoveTo = MoveTo::create(0.5f, Vec2(pos.x, _contentSize.height * SCREEN_BUTTOM_COFFICIENT + 10 / fScalingRatio));
		pBombSprite->runAction(pMoveTo);
		pBombSprite->setBombSpeed(Vec2(0, 0));
	}
	m_nMoveBombNum = 0;
	// 清空飞行Bomb数组数据
	pBombDataManager->clearFlyBomb();
	// 改变游戏状态
	GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_AddBack);
	// 关闭按钮
	setRecyclingBombEnable(false);
}

void BB_GameScene::createRecyclingBombButton()
{
	// 创建回收Bomb的按钮
	m_pRecyclingButton = MenuItemImage::create("res/BB_Bomb/Image/bb_recycling_button.png", "res/BB_Bomb/Image/bb_recycling_button.png",
		CC_CALLBACK_1(BB_GameScene::menuRecyclingBombCallBack, this));
	//// 获取第一个图片的尺寸
	auto pNode1 = m_pRecyclingButton->getNormalImage();
	auto size = pNode1->getContentSize();
	auto pNode2 = m_pRecyclingButton->getSelectedImage();
	pNode2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pNode2->setScale(1.5f);
	pNode2->setPosition(size*0.5f);
	auto menuRecycling = Menu::create(m_pRecyclingButton, NULL);
	menuRecycling->setContentSize(Size(0, 0));
	menuRecycling->setPosition(0, 0);
	// 设置按钮位置
	m_pRecyclingButton->setPosition(_contentSize.width - size.width*1.2f*0.5f, size.height *1.2f*0.5f);
	m_pGameLayer->addChild(menuRecycling, 3);
	// 关闭按钮
	setRecyclingBombEnable(false);
}

void BB_GameScene::setRecyclingBombEnable(const bool& b)
{
	if (b)
	{
		m_pRecyclingButton->setOpacity(1 * 255);
	}
	else
	{
		m_pRecyclingButton->setOpacity(0.5f * 255);
	}
	m_pRecyclingButton->setEnabled(b);
}
