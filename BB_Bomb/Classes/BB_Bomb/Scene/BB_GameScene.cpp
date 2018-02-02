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
#include "../UI/ActionFactoryNode.h"
#include "extensions/cocos-ext.h"  

#include "PhysicsShapeCache.h"

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
		// 设置游戏的背景的列数
		GameDeploy::getInstance()->setGameColumnNun(_contentSize.width);
		// 添加一个游戏开始自定义事件
		_eventDispatcher->addCustomEventListener(START_GAME_EVENT, CC_CALLBACK_1(BB_GameScene::onStartGameEvent, this));
		// 添加保存记录事件回调函数
		_eventDispatcher->addCustomEventListener(SAVE_GAME_SCHEDULE_EVENT, CC_CALLBACK_1(BB_GameScene::onSaveGameScheduleCallBack, this));
		// 开启碰撞回调事件回调函数
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(BB_GameScene::onContactBegin, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


		// 设置添加背景回调函数
		BackgroundDataManager::getInstance()->setAddBackgroundCallBack(CC_CALLBACK_1(BB_GameScene::addBackgroundCallBack, this));


		initHomeLayer();

		// 初始化刚体缓存
		PhysicsShapeCache::getInstance()->addShapesWithFile("res/BB_Bomb/PhysicsBody/physics.plist", CC_CONTENT_SCALE_FACTOR());
		bRet = true;
	} while (0);
	return bRet;
}

void BB_GameScene::onEnter()
{
	Scene::onEnter();

	createTouchListener();
	// 创建背景补边
	auto pS9BackgroundMakeUp = ui::Scale9Sprite::create("res/BB_Bomb/Image/bb_background_make_up.png");
	pS9BackgroundMakeUp->setPreferredSize(_contentSize);
	pS9BackgroundMakeUp->setPosition(_contentSize * 0.5f);
	addChild(pS9BackgroundMakeUp, -1);

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
	// 获取玩家数据
	auto pUserData = UserDataManager::getInstance();


	// 添加背景图片
	auto pBackgroundSprite = Sprite::create("res/BB_Bomb/Image/bb_home_background.png");
	auto backgroundRect = pBackgroundSprite->getBoundingBox();
	pBackgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBackgroundSprite->setScale(_contentSize.width / backgroundRect.size.width);
	// 获取界面尺寸
	auto interfaceRect = pBackgroundSprite->getBoundingBox();
	// 设置界面尺寸
	GameDeploy::getInstance()->setGameLayerSize(interfaceRect.size);
	// 设置界面的Layer
	m_pHomeLayer->setContentSize(interfaceRect.size);
	m_pHomeLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
#if COCOS2D_VERSION > 0x0030330
	m_pHomeLayer->setIgnoreAnchorPointForPosition(false);
#else
	m_pHomeLayer->ignoreAnchorPointForPosition(false);
#endif
	m_pHomeLayer->setPosition(_contentSize * 0.5f);
	addChild(m_pHomeLayer);

	// 添加一个点击任意一处 开始
	auto pStartMenu = Menu::create();
	auto pStartButton = MenuItemImage::create();
	pStartButton->setContentSize(_contentSize);
	pStartButton->setCallback(CC_CALLBACK_1(BB_GameScene::menuStartCallBack, this));
	pStartMenu->addChild(pStartButton);
	m_pHomeLayer->addChild(pStartMenu);

	pBackgroundSprite->setPosition(interfaceRect.size * 0.5f);
	pBackgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	backgroundRect = pBackgroundSprite->getBoundingBox();
	m_pHomeLayer->addChild(pBackgroundSprite);
	// 获取背景的Rect
	backgroundRect = pBackgroundSprite->getBoundingBox();
	//// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	// 字体的大小
	float fFontSize = 30 / fScalingRatio;
	auto pScoreLabel = Label::createWithSystemFont(bailinText("最高分"), FNT_NAME, fFontSize);
	auto scoreRect = pScoreLabel->getBoundingBox();
	// 添加蛇历史最高分
	auto nHighScore = pUserData->getHighScore();
	auto pHistoryScoreLabel = Label::createWithBMFont(GOLDEN_SCORE_BMFNT, StringUtils::format("%d", nHighScore));
	auto historyScoreRect = pHistoryScoreLabel->getBoundingBox();
	// 创建存放最高分的Sprite
	auto pS9HisoryScoreSprite = ui::Scale9Sprite::create("res/BB_Bomb/Image/bb_score_background.png");
	// 计算Label间的间距
	float fSpacing = 5 / fScalingRatio;
	pS9HisoryScoreSprite->setPreferredSize(Size(_contentSize.width * 0.5f, scoreRect.size.height + historyScoreRect.size.height + fSpacing * 2));
	auto historyScoreNodeSize = pS9HisoryScoreSprite->getContentSize();
	float fHistoryScoreScale = 1.0f;
	while (historyScoreRect.size.width + historyScoreRect.size.height * 1.0f > historyScoreNodeSize.width)
	{
		pHistoryScoreLabel->setScale(fHistoryScoreScale);
		fHistoryScoreScale -= 0.01f;
		historyScoreRect = pHistoryScoreLabel->getBoundingBox();
	}
	// 设置Label的位置	
	pScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	pScoreLabel->setPosition(historyScoreNodeSize.width * 0.5f, historyScoreNodeSize.height - fSpacing);
	pHistoryScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	auto fSize = historyScoreNodeSize.width * 0.5f;
	pHistoryScoreLabel->setPosition(historyScoreNodeSize.width * 0.5f, fSpacing);
	pHistoryScoreLabel->setPosition(fSize, 0);
	// 添加label
	pS9HisoryScoreSprite->addChild(pScoreLabel);
	pS9HisoryScoreSprite->addChild(pHistoryScoreLabel);

	pS9HisoryScoreSprite->setColor(Color3B::BLACK);
	pS9HisoryScoreSprite->setOpacity(0.5f * 255);
	pS9HisoryScoreSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pS9HisoryScoreSprite->setPosition(backgroundRect.size.width * 0.5f, backgroundRect.size.height * 0.22f);
	m_pHomeLayer->addChild(pS9HisoryScoreSprite);

	// 创建点击任意开始提示
	auto pStartLabel = Label::createWithSystemFont(bailinText(" > 点击任意开始 < "), FNT_NAME, 30 / fScalingRatio);
	auto startLabelSize = pStartLabel->getContentSize();
	auto pS9StartSprite = ui::Scale9Sprite::create("res/BB_Bomb/Image/bb_score_background.png");
	pS9StartSprite->setPreferredSize(Size(_contentSize.width * 0.5f, startLabelSize.height + 2 * fSpacing));
	auto s9StartSpriteSize = pS9StartSprite->getContentSize();
	// 设置提示位置位置
	pStartLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	pStartLabel->setPosition(s9StartSpriteSize.width * 0.5f, fSpacing);
	pS9StartSprite->addChild(pStartLabel);
	// 设置提示背景
	pS9StartSprite->setColor(Color3B::BLACK);
	pS9StartSprite->setOpacity(0.5f * 255);
	pS9StartSprite->setPosition(interfaceRect.size.width * 0.5f, interfaceRect.size.height * 0.13f);
	m_pHomeLayer->addChild(pS9StartSprite);
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
	m_nBombNum = 1;
	m_nScore = 1;
	m_nMoveBombNum = 0;
	m_nAlreadyShootBombNum = 0;
	// 得分label的缩放比例
	m_fScoreLabelScale = 1.0f;
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
	// 获取游戏配置
	auto pGameDeploy = GameDeploy::getInstance();
	// 获取有效界面尺寸
	auto interfaceSize = pGameDeploy->getInterfaceSize();
	m_pGameLayer = Layer::create();
	m_pGameLayer->setContentSize(interfaceSize);
	m_pGameLayer->setPosition(_contentSize * 0.5f);
#if COCOS2D_VERSION > 0x0030330
	m_pGameLayer->setIgnoreAnchorPointForPosition(false);
#else
	m_pGameLayer->ignoreAnchorPointForPosition(false);
#endif
	m_pGameLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	addChild(m_pGameLayer);
	// 创建背景 
	auto pGameBackgroundSprite = Sprite::create("res/BB_Bomb/Image/bb_game_background.png");
	auto gameBackgroundRect = pGameBackgroundSprite->getBoundingBox();
	pGameBackgroundSprite->setScale(_contentSize.width / gameBackgroundRect.size.width);
	pGameBackgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pGameBackgroundSprite->setPosition(interfaceSize * 0.5f);
	m_pGameLayer->addChild(pGameBackgroundSprite);

	// 创建碰撞边界线刚体
	createBroundLine();

	// 初始化炮台
	m_pFortNode = FortNode::create();
	m_pFortNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_pFortNode->setPosition(interfaceSize.width * 0.5f, 0);
	m_pGameLayer->addChild(m_pFortNode, 1);
	// 设置炮台位置
	FortDataManager::getInstance()->setFortPosition(m_pFortNode->getPosition());
	// 初始化子弹Node
	m_pBombNode = Node::create();
	m_pBombNode->setContentSize(interfaceSize);
	m_pBombNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pBombNode->setPosition(interfaceSize * 0.5f);
	m_pGameLayer->addChild(m_pBombNode);
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	// 初始化食物Node
	m_pFoodNode = Node::create();
	m_pFoodNode->setContentSize(interfaceSize);
	m_pFoodNode->setPosition(interfaceSize * 0.5f);
	m_pFoodNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pGameLayer->addChild(m_pFoodNode, 2);
	// 初始化Block的Node
	m_pBlockNode = Node::create();
	m_pBlockNode->setContentSize(interfaceSize);
	m_pBlockNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pBlockNode->setPosition(interfaceSize * 0.5f);
	m_pGameLayer->addChild(m_pBlockNode);
	// 初始化BlackHole的Node
	m_pBlackHoleNode = Node::create();
	m_pBlackHoleNode->setContentSize(interfaceSize);
	m_pBlackHoleNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pBlackHoleNode->setPosition(interfaceSize * 0.5f);
	m_pGameLayer->addChild(m_pBlackHoleNode);

	// 初始化玩家得分Label
	float fLenght = 20 / fScalingRatio;
	m_pScoreLabel = Label::createWithBMFont(GOLDEN_SCORE_BMFNT, StringUtils::format("%d", m_nScore));
	m_pScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pScoreLabel->setPosition(gameBackgroundRect.size.width * 0.5f, gameBackgroundRect.size.height * 0.948f);
	m_pScoreLabel->setColor(Color3B::WHITE);
	pGameBackgroundSprite->addChild(m_pScoreLabel);
	auto scoreLabelSize = m_pScoreLabel->getContentSize();
	handleScaleScoreLabel();
	// 初始化玩家子弹的数量Label 
	m_pBombNumLabel = Label::createWithBMFont(BLOCK_NUM_BMFNT, StringUtils::format("x%d", m_nBombNum));
	m_pBombNumLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto pBombNumBackground = Sprite::create("res/BB_Bomb/Image/bb_bomb_background.png");
	auto bombNumBackgroundSize = pBombNumBackground->getContentSize();
	pBombNumBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	pBombNumBackground->setPosition(0, interfaceSize.height * 0.95f);
	m_pGameLayer->addChild(pBombNumBackground, 2);
	// 添加子弹数量Label
	m_pBombNumLabel->setPosition(bombNumBackgroundSize.width * 0.5f, bombNumBackgroundSize.height* 0.5f);
	pBombNumBackground->addChild(m_pBombNumLabel);

	// 创建边界
	// 底边
	auto pBottomBarSprite = Sprite::create("res/BB_Bomb/Image/bb_bar.png");
	auto barSize = pBottomBarSprite->getContentSize();
	pBottomBarSprite->setScale(_contentSize.width / barSize.width);
	pBottomBarSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	// pBottomBarSprite->setPosition(0, interfaceSize.height * 0.130f);
	pBottomBarSprite->setPosition(0, interfaceSize.height * SCREEN_BUTTOM_COFFICIENT);
	m_pGameLayer->addChild(pBottomBarSprite);
	// 上边
	auto pTopBarSprite = Sprite::create("res/BB_Bomb/Image/bb_bar.png");
	pTopBarSprite->setScale(_contentSize.width / barSize.width);
	pTopBarSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	// pTopBarSprite->setPosition(0, interfaceSize.height * 0.846f);
	pTopBarSprite->setPosition(0, interfaceSize.height * SCREEN_TOP_COFFICIENT);
	m_pGameLayer->addChild(pTopBarSprite);
	// 创建按钮
	createRecyclingBombButton();
}

void BB_GameScene::createBroundLine()
{
	auto pGameDeploy = GameDeploy::getInstance();
	float fScalingRatio = pGameDeploy->getScalingRatio();
	// 加粗刚体
	float fLenght = 50 / fScalingRatio;
	auto interfaceSize = pGameDeploy->getInterfaceSize();
	// 上边
	auto pTopBroundLine = PhysicsBody::createEdgeSegment(Vec2(0, interfaceSize.height * SCREEN_TOP_COFFICIENT + fLenght),
		Vec2(interfaceSize.width, interfaceSize.height * SCREEN_TOP_COFFICIENT + fLenght), PhysicsMaterial(0.0f, 1.0f, 0.0f), fLenght);
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
	// auto pLeftBroundLine = PhysicsBody::createEdgeSegment(Vec2(0, interfaceSize.height * 0.13f),
	// Vec2(0, interfaceSize.height * 0.846f), PhysicsMaterial(0.0f, 1.0f, 0.0f));
	auto pLeftBroundLine = PhysicsBody::createEdgeSegment(Vec2(-fLenght, 0),
		Vec2(-fLenght, interfaceSize.height * SCREEN_TOP_COFFICIENT), PhysicsMaterial(0.0f, 1.0f, 0.0f), fLenght);
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
	auto pRightBroundLine = PhysicsBody::createEdgeSegment(Vec2(interfaceSize.width + fLenght, 0),
		Vec2(interfaceSize.width + fLenght, interfaceSize.height * SCREEN_TOP_COFFICIENT), PhysicsMaterial(0.0f, 1.0f, 0.0f), fLenght);
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
	auto pBottomBroundLine = PhysicsBody::createEdgeSegment(Vec2(0, interfaceSize.height * SCREEN_BUTTOM_COFFICIENT),
		Vec2(interfaceSize.width, interfaceSize.height * SCREEN_BUTTOM_COFFICIENT), PhysicsMaterial(0.0f, 0.0f, 0.0f));
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
	if (gameStatus != GameStatusManager::GameStatus::kStatus_Result && gameStatus != GameStatusManager::GameStatus::kStatus_None)
	{
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
	// 获取极限角度
	auto fLimitAngle = GameDeploy::getInstance()->getArrowLimitAngle();
	if (fAngle < fLimitAngle || fAngle > 180 - fLimitAngle)
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
	m_nMoveBombNum = pFortMangager->getBombSize();
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
			pos = m_pGameLayer->convertToWorldSpace(pos);
			pParticleRatio->setPosition(pos);
			pParticleRatio->setCascadeColorEnabled(true);
			pParticleRatio->setScale(1 / fScalingRatio);
			pParticleRatio->setLife(4);
			pParticleRatio->setAutoRemoveOnFinish(true);
			addChild(pParticleRatio);
		}
		// 得分加一
		m_nScore++;
		m_pScoreLabel->setString(StringUtils::format("%d", m_nScore));
		handleScaleScoreLabel();
	}
	break;
	case BOUNDING_TAG_NUM:
	{
		if (contac.getContactData()->normal.y >= 0)
		{
			// 子弹从下往上穿不判断碰撞
			return false;
		}
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
			// 处理黑洞Block
			handleBlackHoleBlack();
		}
		a->setVelocity(Vec2(0, 0));
		// 关闭刚体
#if COCOS2D_VERSION <= 0x0030330
		a->setEnable(false);
#else
		a->setEnabled(false);
#endif
		// 移动炮台
		if (m_bMoveFort)
		{
			auto pANode = a->getNode();
			auto pBombSprite = dynamic_cast<BombNode*>(pANode);
			// 从飞行的子弹数组中删除这个子弹
			pBombDataManager->removeFlyBombToVector(pBombSprite);
			pBombSprite->setOpacity(0);
			auto pos = a->getPosition();
			moveFort(pos.x);
			// 获取炮台位置
			auto fortPos = FortDataManager::getInstance()->getForPosition();
			auto fortNodeSize = m_pFortNode->getContentSize();
			auto pMoveTo = MoveTo::create(0.5f, Vec2(pos.x, fortPos.y + fortNodeSize.height * 0.5f));
			auto delayTime = DelayTime::create(0.1f);
			auto seq = Sequence::create(delayTime, pMoveTo, nullptr);
			pBombSprite->runAction(seq);
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
			if (pBombNode == nullptr)	break;
			pBombNode->setBombStatus(BombNode::BombStatus::Status_MoveTo);
			// 获取炮台位置
			auto fortPos = FortDataManager::getInstance()->getForPosition();
			auto fortNodeSize = m_pFortNode->getContentSize();
			auto pMoveTo = MoveTo::create(0.5f, Vec2(pos.x, fortPos.y + fortNodeSize.height * 0.5f));

			// 创建一个动画结束回调函数
			auto pFun = CallFuncN::create(CC_CALLBACK_1(BB_GameScene::hideBombCallBack, this, pBombNode));
			auto delayTime = DelayTime::create(0.1f);
			auto seq = Sequence::create(delayTime, pMoveTo, pFun, nullptr);
			pBombNode->runAction(seq);
		}
	}
	break;
	case FOOD_TAG_NUM:
	{
		if (pNode == NULL) break;;
		auto pFoodNode = dynamic_cast<FoodNode*>(pNode->getParent());
		// 关闭刚体
		pFoodNode->setBodyEnabled(false);
		auto pos = pFoodNode->getPosition();
		// 添加消失动画
		auto pFoodAnctionSprite = ActionFactoryNode::create(1);
		auto pFoodAnctionSpriteRect = pFoodAnctionSprite->getBoundingBox();
		pFoodAnctionSprite->setPosition(Vec2(pos.x - pFoodAnctionSpriteRect.size.width, pos.y));
		m_pFoodNode->addChild(pFoodAnctionSprite, 20);
		// 删除原来的食物
		pFoodNode->reomveFoodNode();
		// 子弹数量加一
		m_pBombNumLabel->setString(bailinText(StringUtils::format("x%d", ++m_nBombNum)));
	}
	break;
	case BLACK_HOLE_TAG_NUM:
	{
		if (pNode == nullptr)
		{
			break;
		}
		auto pBlockNode = dynamic_cast<BlockNode*>(pNode->getParent());
		pBlockNode->hitted();
		// 获取Block的Hp
		auto nHp = pBlockNode->getBLockHp();

		// 得分加一
		m_nScore++;
		m_pScoreLabel->setString(StringUtils::format("%d", m_nScore));
		handleScaleScoreLabel();

		if (nHp <= 0)
		{
			auto pos = pBlockNode->getPosition();
			pBlockNode->removeFromParent();
			// 播放爆炸效果
			auto pParticleRatio = ParticleSystemQuad::create("res/BB_Bomb/particle/bb_block_explode.plist");
			auto nNum = random(0, 60);
			pParticleRatio->setStartColorVar(Color4F(getBlockColorWithNumer(nNum)));
			pParticleRatio->setPosition(pos);
			pos = m_pGameLayer->convertToWorldSpace(pos);
			pParticleRatio->setCascadeColorEnabled(true);
			pParticleRatio->setScale(1 / fScalingRatio);
			pParticleRatio->setLife(4);
			pParticleRatio->setAutoRemoveOnFinish(true);
			addChild(pParticleRatio);
		}
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
				// FIXME: 这里在游戏结束最后的下降动画时操作炮台多点几下会在结算界面弹出后数组访问异常
				auto pBombNode = vBomb[m_nAlreadyShootBombNum];
				// 转移子弹到炮口
				//auto fortTopPos = getFortTopPos();
				//auto pMoveTo = MoveTo::create(0.01f, fortTopPos);
				//pBombNode->runAction(pMoveTo);
				//pBombNode->setPosition(fortTopPos);

				pBombNode->setBombSpeed(bombSpeed);
				pBombNode->setVisible(true);
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
		// 改变成滚动屏幕状态
		pGameStatus->setGameStatus(GameStatusManager::GameStatus::kStatus_RollScreen);
	}
}

void BB_GameScene::addBackgroundCallBack(cocos2d::Node* pNode)
{
	// 获取游戏配置
	auto pGameDeploy = GameDeploy::getInstance();
	auto interfaceSize = pGameDeploy->getInterfaceSize();
	float fSize = BLOCK_SIZE;
	pNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pNode->setPositionY(interfaceSize.height * 0.846f - fSize * 0.5f);
	int nNum = pNode->getTag();
	switch (nNum)
	{
	case BLOCK_TAG_NUM:
		m_pBlockNode->addChild(pNode);
		break;
	case FOOD_TAG_NUM:
		m_pFoodNode->addChild(pNode);
		break;
	case BLACK_HOLE_TAG_NUM:
		m_pBlackHoleNode->addChild(pNode);
		break;
	default:
		break;
	}
}

void BB_GameScene::handleRollScreen()
{
	auto pGameStatus = GameStatusManager::getInstance();
	bool bGameOver = true;
	if (pGameStatus->getGameStatus() != GameStatusManager::GameStatus::kStatus_RollScreen)
	{
		return;
	}
	// 所有内容下移
	float fSize = BLOCK_SIZE;
	auto vBlock = m_pBlockNode->getChildren();
	for (int i = 0; i < vBlock.size(); i++)
	{
		auto pBlockNode = vBlock.at(i);
		auto xx = pBlockNode->getAnchorPoint();
		auto blockPos = pBlockNode->getPosition();
		float fY = blockPos.y - fSize;
		auto pMoveTo = MoveTo::create(1, Vec2(blockPos.x, fY));
		auto delayTime = DelayTime::create(0.5f);
		if (fY - fSize*0.5f <= _contentSize.height * SCREEN_BUTTOM_COFFICIENT &&
			pGameStatus->getGameStatus() != GameStatusManager::GameStatus::kStatus_Result)
		{
			// 改变游戏状态
			pGameStatus->setGameStatus(GameStatusManager::GameStatus::kStatus_Result);
			// 游戏结束 添加一个动作结束回调函数
			CallFunc* pCallFunc = CallFunc::create(CC_CALLBACK_0(BB_GameScene::handleGameOver, this));
			FiniteTimeAction*  pFiniteTimeAction = Sequence::create(delayTime, pMoveTo, pCallFunc, NULL);
			pBlockNode->runAction(pFiniteTimeAction);
			bGameOver = false;
		}
		else
		{
			auto seq = Sequence::create(delayTime, pMoveTo, nullptr);
			pBlockNode->runAction(seq);
		}
	}
	// 下移动黑洞
	auto vBlackHole = m_pBlackHoleNode->getChildren();
	for (int i = 0; i < vBlackHole.size(); i++)
	{
		auto pBlackHoleNode = vBlackHole.at(i);
		auto blackHolePos = pBlackHoleNode->getPosition();
		float fY = blackHolePos.y - fSize;
		auto pMoveTo = MoveTo::create(1, Vec2(blackHolePos.x, fY));
		auto delayTime = DelayTime::create(0.5f);
		auto seq = Sequence::create(delayTime, pMoveTo, nullptr);
		pBlackHoleNode->runAction(seq);
		if (fY <= _contentSize.height * SCREEN_BUTTOM_COFFICIENT)
		{
			pBlackHoleNode->removeFromParent();
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
	// 如果游戏结束着不改变游戏状态
	if (bGameOver)
	{
		// 改变游戏状态
		GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_None);
		// 1.5秒后才能到下一阶段
		auto pDelayTime = DelayTime::create(1.5f);
		auto pCallFuncN = CallFuncN::create(CC_CALLBACK_1(BB_GameScene::setGameStatusCallBack, this, GameStatusManager::GameStatus::kStatus_AddBomb));
		auto pSeq = Sequence::create(pDelayTime, pCallFuncN, NULL);
		this->runAction(pSeq);
	}
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
	// 获取炮台位置
	auto pos = FortDataManager::getInstance()->getForPosition();
	auto fortNodeSize = m_pFortNode->getContentSize();
	// 创建子弹
	for (int i = vBomb.size(); i < m_nBombNum; i++)
	{
		auto pBombSprite = BombDataManager::getInstance()->addBombNode();
		pBombSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pBombSprite->setPosition(pos.x, pos.y + fortNodeSize.height * 0.5f);
		pBombSprite->setBombSpeed(Vec2(0.0f, 0.0f));
		m_pBombNode->addChild(pBombSprite);
		pBombSprite->setVisible(false);
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
	// 改变游戏状态
	GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_Result);
	// 获取游戏界面
	float fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	auto interfaceSize = GameDeploy::getInstance()->getInterfaceSize();
	// 显示结束界面
	m_pGameOverLayer = LayerColor::create();
	m_pGameOverLayer->setColor(Color3B::GRAY);
	m_pGameOverLayer->setOpacity(0.5 * 255);
	m_pGameOverLayer->ignoreAnchorPointForPosition(false);
	m_pGameOverLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pGameOverLayer->setPosition(_contentSize * 0.5f);
	addChild(m_pGameOverLayer, 2);

	// 检测是否打破记录
	auto pUserData = UserDataManager::getInstance();
	int nHistoryHighScore = pUserData->getHighScore();
	if (nHistoryHighScore < m_nScore)
	{
		pUserData->setHighScore(m_nScore);
		nHistoryHighScore = m_nScore;
	}

	// 结算界面背景
	auto pSettlementSprite = Sprite::create("res/BB_Bomb/Image/bb_settlement.png");
	auto settlementSize = pSettlementSprite->getContentSize();

	pSettlementSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pSettlementSprite->setScale(_contentSize.width * 0.8 / settlementSize.width);
	pSettlementSprite->setPosition(_contentSize * 0.5f);
	m_pGameOverLayer->addChild(pSettlementSprite);

	// 显示玩家得分
	auto pScoreLabel = Label::createWithBMFont(GOLDEN_SCORE_BMFNT, StringUtils::format("%d", m_nScore));
	float fScoreLabelScale = 1.5f;
	pScoreLabel->setScale(fScoreLabelScale);
	pScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto scoreLabelRect = pScoreLabel->getBoundingBox();
	// 对分数进行缩放
	while (scoreLabelRect.size.width + scoreLabelRect.size.height > interfaceSize.width * 0.663f)
	{
		fScoreLabelScale -= 0.01f;
		pScoreLabel->setScale(fScoreLabelScale);
		scoreLabelRect = pScoreLabel->getBoundingBox();
	}
	pScoreLabel->setPosition(settlementSize.width * 0.5f, settlementSize.height * 0.52f);
	pSettlementSprite->addChild(pScoreLabel);

	// 显示历史最高分
	auto pHistorHighScoreLabel = Label::createWithSystemFont(bailinText(StringUtils::format("最高分  %d", nHistoryHighScore)), FNT_NAME, 35 / fScalingRatio);
	pHistorHighScoreLabel->setTextColor(Color4B::WHITE);
	auto HighScoreLabelRect = pHistorHighScoreLabel->getBoundingBox();
	float highScoreLabelScale = 1.0f;
	// 对历史最高分进行缩放
	while (HighScoreLabelRect.size.width + HighScoreLabelRect.size.height > interfaceSize.width * 0.553f)
	{
		highScoreLabelScale -= 0.01f;
		pHistorHighScoreLabel->setScale(fScoreLabelScale);
		HighScoreLabelRect = pHistorHighScoreLabel->getBoundingBox();
	}
	pHistorHighScoreLabel->setPosition(settlementSize.width * 0.5f, settlementSize.height * 0.389f);
	pSettlementSprite->addChild(pHistorHighScoreLabel);

	// 创建返回首页的按钮
	auto pHomeButtom = MenuItemImage::create("res/BB_Bomb/Image/bb_home_bottom.png", "res/BB_Bomb/Image/bb_home_bottom.png", CC_CALLBACK_1(BB_GameScene::menuHomeCallBack, this));
	auto pFristSprite = pHomeButtom->getNormalImage();
	auto fristSize = pFristSprite->getContentSize();
	auto pLastSprite = pHomeButtom->getSelectedImage();
	pLastSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pLastSprite->setScale(1.08f);
	pLastSprite->setPosition(fristSize * 0.5f);

	auto pHomeMenu = Menu::create(pHomeButtom, nullptr);
	pHomeMenu->setContentSize(Size(0, 0));
	pHomeMenu->setPosition(settlementSize.width * 0.5f, settlementSize.height * 0.165f);
	pSettlementSprite->addChild(pHomeMenu);

	// 关闭update
	// unscheduleUpdate();
}

void BB_GameScene::menuHomeCallBack(cocos2d::Ref* spender)
{
	// 删除游戏界面
	m_pGameLayer->removeFromParent();
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
	auto pGameStatus = GameStatusManager::getInstance();
	if (pGameStatus->getGameStatus() != GameStatusManager::GameStatus::kStatus_Runnning)
	{
		return;
	}
	// 获取飞行中的子弹数组
	auto pBombDataManager = BombDataManager::getInstance();
	auto vFlyBomb = pBombDataManager->getFlyBombVector();
	// 获取炮台数据
	auto pFortDataManager = FortDataManager::getInstance();
	auto pos = pFortDataManager->getForPosition();
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	auto fortSize = m_pFortNode->getContentSize();
	for (auto i = 0; i < vFlyBomb.size(); i++)
	{
		auto pBombSprite = vFlyBomb[i];
		pBombSprite->setBombSpeed(Vec2(0, 0));
#if COCOS2D_VERSION <= 0x0030330
		pBombSprite->getPhysicsBody()->setEnable(false);
#else
		pBombSprite->getPhysicsBody()->setEnabled(false);
#endif
		auto pMoveTo = MoveTo::create(0.5f, Vec2(pos.x, pos.y + fortSize.height * 0.5f));
		// 创建一个动画结束回调函数
		auto pFun = CallFuncN::create(CC_CALLBACK_1(BB_GameScene::hideBombCallBack, this, pBombSprite));
		auto delayTime = DelayTime::create(0.1f);
		auto seq = Sequence::create(delayTime, pMoveTo, pFun, nullptr);
		pBombSprite->runAction(seq);
	}
	m_nMoveBombNum = 0;
	// 清空飞行Bomb数组数据
	pBombDataManager->clearFlyBomb();
	// 改变游戏状态
	pGameStatus->setGameStatus(GameStatusManager::GameStatus::kStatus_AddBack);
	// 关闭按钮
	setRecyclingBombEnable(false);
	// 处理黑洞Block
	handleBlackHoleBlack();
}

void BB_GameScene::createRecyclingBombButton()
{
	// 获取界面有效尺寸
	auto interfaceSize = GameDeploy::getInstance()->getInterfaceSize();
	// 创建回收Bomb的按钮
	m_pRecyclingButton = MenuItemImage::create("res/BB_Bomb/Image/bb_recycling_button.png", "res/BB_Bomb/Image/bb_recycling_button.png",
		CC_CALLBACK_1(BB_GameScene::menuRecyclingBombCallBack, this));
	//// 获取第一个图片的尺寸
	auto pNode1 = m_pRecyclingButton->getNormalImage();
	auto size = pNode1->getContentSize();
	auto pNode2 = m_pRecyclingButton->getSelectedImage();
	pNode2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pNode2->setScale(1.1f);
	pNode2->setPosition(size*0.5f);
	auto menuRecycling = Menu::create(m_pRecyclingButton, NULL);
	menuRecycling->setContentSize(Size(0, 0));
	// 设置按钮位置
	menuRecycling->setPosition(interfaceSize.width - size.width * 1.1f * 0.5f, interfaceSize.height * 0.95f);
	m_pGameLayer->addChild(menuRecycling, 2);
	// 关闭按钮
	setRecyclingBombEnable(false);
}

void BB_GameScene::setRecyclingBombEnable(const bool& b)
{
	if (m_pRecyclingButton == nullptr)
	{
		return;
	}
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

void BB_GameScene::handleBlackHoleBlack()
{
	auto vBlackHole = m_pBlackHoleNode->getChildren();
	// 获取子弹管理类
	auto pBombManager = BombDataManager::getInstance();
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	for (int i = 0; i < vBlackHole.size(); i++)
	{
		auto pNode = vBlackHole.at(i);
		auto pBlackHoleNode = dynamic_cast<BlockNode*> (pNode);
		auto pos = pBlackHoleNode->getPosition();
		// 删除黑洞Blockk
		pBlackHoleNode->removeFromParent();
		// 添加粒子爆炸效果
		auto pParticleRatio = ParticleSystemQuad::create("res/BB_Bomb/particle/bb_block_explode.plist");
		auto nNum = random(0, 60);
		pParticleRatio->setStartColorVar(Color4F(getBlockColorWithNumer(nNum)));
		pParticleRatio->setPosition(pos);
		// pos = m_pGameLayer->convertToWorldSpace(pos);
		// pos = m_pGameLayer->convertToWorldSpace(pos);
		pParticleRatio->setCascadeColorEnabled(true);
		pParticleRatio->setScale(1 / fScalingRatio);
		pParticleRatio->setLife(4);
		pParticleRatio->setAutoRemoveOnFinish(true);
		m_pGameLayer->addChild(pParticleRatio);

		auto pBombActuon = ActionFactoryNode::create(-1);
		pBombActuon->setPosition(pos.x - BLOCK_SIZE*0.5f, pos.y);
		m_pGameLayer->addChild(pBombActuon, 2);

		// 删除子弹
		pBombManager->removeBomb();
		// 子弹数量减一
		// 刷新子弹个数
		m_pBombNumLabel->setString(bailinText(StringUtils::format("子弹 : %d", --m_nBombNum)));
	}
}

void BB_GameScene::hideBombCallBack(Ref* speder, BombNode* pBombNode)
{
	if (pBombNode == nullptr)
	{
		return;
	}
	pBombNode->setVisible(false);
}



void BB_GameScene::setGameStatusCallBack(cocos2d::Ref* spender, const GameStatusManager::GameStatus& eStatus)
{
	auto pGameStatua = GameStatusManager::getInstance();
	if (pGameStatua == nullptr) return;

	pGameStatua->setGameStatus(eStatus);
}

void BB_GameScene::handleScaleScoreLabel()
{
	if (m_pScoreLabel == nullptr) return;

	// 获取屏幕有效尺寸
	auto interfaceSize = GameDeploy::getInstance()->getInterfaceSize();
	// 处理字体超出边宽范围缩小
	auto scoreRect = m_pScoreLabel->getBoundingBox();
	// 0.45 是按照显示框的根据屏幕的尺寸计算出来的
	while (scoreRect.size.width + scoreRect.size.height > interfaceSize.width * 0.45f)
	{
		m_fScoreLabelScale -= 0.01f;
		m_pScoreLabel->setScale(m_fScoreLabelScale);
		scoreRect = m_pScoreLabel->getBoundingBox();
	}
}

void BB_GameScene::onSaveGameScheduleCallBack(cocos2d::Ref* spender)
{
	// 回调 保存游戏
	CCLOG("on onSaveGameScheduleCallBack");
}
