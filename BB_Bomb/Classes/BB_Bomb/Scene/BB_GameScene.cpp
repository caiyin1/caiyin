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
#include "../Manager/DataManager.h"
#include "../../bailinUtils/RapidJsonMacro.h"
#include "../Util/archiveMacro.h"
#include "../StructData/BlockData.h"
#include "../dialog/ReadRecordDialog.h"
#include "../UI/PraiseActionNode.h"

#include "../dialog/ResultDialog.h"
#include "../dialog/StartDialog.h"
#include "../dialog/HelpDialog.h"

#include "PhysicsShapeCache.h"

#ifdef ANALYSIS_TALKINGDATA
#include "TalkingDataAnalysis.h"
#endif

USING_NS_CC_EXT;
USING_NS_CC;

// 开始对话框的控件名称，为了方便获取对应的实例
#define START_DIALOG "StartDialog"


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
		// pRet->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
		// 关闭子弹碰撞的自动检测
		pRet->getPhysicsWorld()->setAutoStep(false);
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


		// 初始化刚体缓存
		PhysicsShapeCache::getInstance()->addShapesWithFile("res/BB_Bomb/PhysicsBody/physics.plist", CC_CONTENT_SCALE_FACTOR());
		// 设置DataManager类中强制游戏结束的回调函数
		DataManager::getInstance()->setGameOverCallBack([this]() {
			m_nScore = 0;
			handleGameOver();
		});
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
	// 创建开始对话框
	auto pStartDialog = StartDialog::create()->setCallbackStartBtn([]() {
		// 发送游戏开始的事件
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(START_GAME_EVENT);
	});
	pStartDialog->setName(START_DIALOG);
	pStartDialog->showModalDialog();

	// 检测是否有存档
	auto pDataManager = DataManager::getInstance();
	if (pDataManager->isGameRecord())
	{
		auto pDlg = ReadRecordDialog::create();
		pDlg->showModalDialog();
		pDlg->setOkButtonCallBack([this]() {
			auto pStartDialog = dynamic_cast<StartDialog*>(getChildByName(START_DIALOG));
			if (pStartDialog) pStartDialog->closeDialog();

			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(START_GAME_EVENT);
			// 读取文档
			auto pDataManager = DataManager::getInstance();
			if (pDataManager->readRecord())
			{
				//显示读取文档
				showGameRecordScreen();
			}
		});
	}
}

void BB_GameScene::createTouchListener()
{
	m_pEventListenerTouch = EventListenerTouchOneByOne::create();
	// 
	m_pEventListenerTouch->onTouchBegan = CC_CALLBACK_2(BB_GameScene::onTouchBegin, this);
	m_pEventListenerTouch->onTouchMoved = CC_CALLBACK_2(BB_GameScene::onTouchMoved, this);
	m_pEventListenerTouch->onTouchEnded = CC_CALLBACK_2(BB_GameScene::onTouchEnded, this);
	m_pEventListenerTouch->onTouchCancelled = CC_CALLBACK_2(BB_GameScene::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pEventListenerTouch, this);
}

bool BB_GameScene::onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	// 获取游戏状态
	auto gameStatus = GameStatusManager::getInstance()->getGameStatus();
	if (gameStatus == GameStatusManager::GameStatus::kStatus_ReadyShoot)
	{
		if (m_bIsPressed) return false;

		m_bIsPressed = true;

		m_fPosX = touch->getLocation().x;
		auto pos = touch->getLocation();

		// 设置箭头向量的位置
		// 添加提示箭头
		m_pFortNode->addArrow(pos);
		return true;
	}
	else
	{
		return false;
	}
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

	m_dRoundTime = 0;

	if (UserDataManager::getInstance()->getHighScore() <= 0)
		HelpDialog::create()->showModalDialog();
}

void BB_GameScene::initGameData()
{
	// 获取数据管理类
	auto pDataManager = DataManager::getInstance();
	// 设置玩家子弹
	pDataManager->safeModifyData(m_nBombNum, SAFE_BOMB_SIZE_KEY, 1, false);
	// 设置玩家得分
	pDataManager->safeModifyData(m_nScore, SAFE_PLAYER_SOCRE_KEY, 0, false);
	// 玩家本回合得分
	m_nRoundScore = 0;
	addRoundScore(0, true);
	// 本回合得分级别
	m_nRoundLevel = 1;
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

	m_bIsPressed = false;
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
	// 创建背景 
	auto pGameBackgroundSprite = Sprite::create("res/BB_Bomb/Image/bb_game_background.png");
	// 获取游戏配置
	auto pGameDeploy = GameDeploy::getInstance();
	pGameDeploy->setGameLayerSize(pGameBackgroundSprite->getContentSize());
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
	auto fortNodeSize = m_pFortNode->getContentSize();
	m_pFortNode->setPosition(interfaceSize.width * 0.5f, -fortNodeSize.height * 0.1f);
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
	auto pScoreBackgroundLayer = LayerColor::create();
	pScoreBackgroundLayer->setColor(Color3B::RED);
#if COCOS2D_VERSION > 0x0030330
	pScoreBackgroundLayer->setIgnoreAnchorPointForPosition(false);
#else
	pScoreBackgroundLayer->ignoreAnchorPointForPosition(false);
#endif
	pScoreBackgroundLayer->setContentSize(Size(interfaceSize.width * 0.2985f, interfaceSize.height * 0.0524f));
	pScoreBackgroundLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pScoreBackgroundLayer->setPosition(gameBackgroundRect.size.width * 0.565f, gameBackgroundRect.size.height * 0.955f);
	pScoreBackgroundLayer->setOpacity(0);
	m_pGameLayer->addChild(pScoreBackgroundLayer);
	auto scoreBackgroundLayerSize = pScoreBackgroundLayer->getContentSize();

	m_pScoreLabel = Label::createWithBMFont(GOLDEN_SCORE_BMFNT, StringUtils::format("%d", m_nScore));
	m_pScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pScoreLabel->setPosition(scoreBackgroundLayerSize * 0.5f);
	m_pScoreLabel->setColor(Color3B::WHITE);
	pScoreBackgroundLayer->addChild(m_pScoreLabel);
	auto scoreLabelSize = m_pScoreLabel->getContentSize();
	handleScaleScoreLabel();
	// 初始化玩家子弹的数量Label 
	auto pBombNumBackgroundLayer = LayerColor::create();
	// pBombNumBackgroundLayer->setColor(Color3B::RED);
	// pBombNumBackgroundLayer->setOpacity(255);
	pBombNumBackgroundLayer->setContentSize(Size(interfaceSize.width * 0.1904f, interfaceSize.height * 0.0524f));
	pBombNumBackgroundLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBombNumBackgroundLayer->setPosition(interfaceSize.width * 0.2148f, interfaceSize.height * 0.955f);
#if COCOS2D_VERSION > 0x0030330
	pBombNumBackgroundLayer->setIgnoreAnchorPointForPosition(false);
#else
	pBombNumBackgroundLayer->ignoreAnchorPointForPosition(false);
#endif
	m_pGameLayer->addChild(pBombNumBackgroundLayer);
	m_pBombNumLabel = Label::createWithBMFont(GOLDEN_SCORE_BMFNT, StringUtils::format("%d", m_nBombNum));
	m_pBombNumLabel->setColor(Color3B::GREEN);
	m_pBombNumLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto pBombNumBackground = Sprite::create("res/BB_Bomb/Image/bb_bomb_background.png");
	auto bombNumBackgroundSize = pBombNumBackground->getContentSize();
	pBombNumBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBombNumBackground->setPosition(interfaceSize.width * 0.082f, interfaceSize.height * 0.961f);
	m_pGameLayer->addChild(pBombNumBackground);
	// 添加子弹数量Label
	m_pBombNumLabel->setPosition(pBombNumBackgroundLayer->getContentSize() * 0.5f);
	pBombNumBackgroundLayer->addChild(m_pBombNumLabel);

	// 创建边界
	// 底边
	auto pBottomBarSprite = Sprite::create("res/BB_Bomb/Image/bb_bar.png");
	auto barSize = pBottomBarSprite->getContentSize();
	pBottomBarSprite->setScale(_contentSize.width / barSize.width);
	pBottomBarSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	// pBottomBarSprite->setPosition(0, interfaceSize.height * 0.130f);
	pBottomBarSprite->setPosition(0, interfaceSize.height * SCREEN_BUTTOM_COFFICIENT);
	m_pGameLayer->addChild(pBottomBarSprite);
	// 上边
	//auto pTopBarSprite = Sprite::create("res/BB_Bomb/Image/bb_bar.png");
	//pTopBarSprite->setScale(_contentSize.width / barSize.width);
	//pTopBarSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	//// pTopBarSprite->setPosition(0, interfaceSize.height * 0.846f);
	//pTopBarSprite->setPosition(0, interfaceSize.height * SCREEN_TOP_COFFICIENT);
	//m_pGameLayer->addChild(pTopBarSprite);
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

	// 下边
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
	if (gameStatus != GameStatusManager::GameStatus::kStatus_Result &&
		gameStatus != GameStatusManager::GameStatus::kStatus_None)
	{
		m_dRoundTime += dt;
	}

	if (gameStatus != GameStatusManager::GameStatus::kStatus_Result && gameStatus != GameStatusManager::GameStatus::kStatus_None && gameStatus != GameStatusManager::GameStatus::kStatus_WaitMoveEnd)
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

void BB_GameScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *ev)
{
	m_bIsPressed = false;
}

void BB_GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	m_bIsPressed = false;
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
	// 重置本回合得分
	addRoundScore(0, true);
	// 重置本回合得分等级
	m_nRoundLevel = 1;
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
		DataManager::getInstance()->safeModifyData(m_nScore, SAFE_PLAYER_SOCRE_KEY, 1, true);
		addRoundScore(1);
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
		DataManager::getInstance()->safeModifyData(m_nBombNum, SAFE_BOMB_SIZE_KEY, 1, true);
		m_pBombNumLabel->setString(bailinText(StringUtils::format("x%d", m_nBombNum)));
		// 检车是否要缩放子弹
		handleScaleBombSizeLabel();
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
		DataManager::getInstance()->safeModifyData(m_nScore, SAFE_PLAYER_SOCRE_KEY, 1, true);
		addRoundScore(1);
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
	pNode->setPositionY(interfaceSize.height * SCREEN_TOP_COFFICIENT - fSize * 0.5f);
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
		auto pBlockNode = dynamic_cast<BlockNode*>(vBlock.at(i));
		auto xx = pBlockNode->getAnchorPoint();
		auto blockPos = pBlockNode->getPosition();
		float fY = blockPos.y - fSize;
		pBlockNode->setLastBlockPosition(blockPos);
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
		GameStatusManager::getInstance()->setGameStatus(GameStatusManager::GameStatus::kStatus_WaitMoveEnd);
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
		auto pBombNode = BombDataManager::getInstance()->addBombNode();
		pBombNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pBombNode->setPosition(pos.x, pos.y + fortNodeSize.height * 0.5f);
		pBombNode->setBombSpeed(Vec2(0.0f, 0.0f));
		m_pBombNode->addChild(pBombNode);
		pBombNode->setVisible(false);
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

	// 检测是否打破记录
	auto pUserData = UserDataManager::getInstance();
	int nHistoryHighScore = pUserData->getHighScore();
	if (nHistoryHighScore < m_nScore)
	{
		pUserData->setHighScore(m_nScore);
		nHistoryHighScore = m_nScore;
	}
	// 记录玩家本局得分
	pUserData->setPlayerScore(m_nScore);

#ifdef ANALYSIS_TALKINGDATA
	std::unordered_map<std::string, std::string> eventData;

	// 记录用户的一局游戏时间
	eventData.insert(std::make_pair("RoundTime", StringUtils::format("%ld", (long)m_dRoundTime)));
	// 记录用户的当局分数
	eventData.insert(std::make_pair("Score", StringUtils::format("%d", m_nScore)));
	// 记录用户的球数量
	eventData.insert(std::make_pair("Balls", StringUtils::format("%d", m_nBombNum)));
	// 记录用户的下降层数量
	eventData.insert(std::make_pair("Floors", StringUtils::format("%d", GameDeploy::getInstance()->getBlockHp() - 1)));

	// 发送记录数据
	TalkingDataAnalysis::getInstance()->event("GameOver", eventData);
#endif

	auto pDlg = ResultDialog::create();
	pDlg->setComfirmBtnClickCallback([this]() {
		// 删除游戏界面
		m_pGameLayer->removeFromParent();
		// 弹出游戏开始的对话框
		StartDialog::create()->setCallbackStartBtn([]() {
			// 发送游戏开始的事件
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(START_GAME_EVENT);
		})->showModalDialog();
	});
	pDlg->showModalDialog();
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
	menuRecycling->setPosition(interfaceSize.width - size.width * 1.1f * 0.5f, interfaceSize.height * 0.955);
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
		DataManager::getInstance()->safeModifyData(m_nBombNum, SAFE_BOMB_SIZE_KEY, -1, true);
		// 刷新子弹个数
		m_pBombNumLabel->setString(bailinText(StringUtils::format("子弹 : %d", m_nBombNum)));
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

	auto scoreRect = m_pScoreLabel->getBoundingBox();
	auto scoreBackgroundLayerSize = m_pScoreLabel->getParent()->getContentSize();
	if (scoreRect.size.width > scoreBackgroundLayerSize.width)
	{
		m_pScoreLabel->setScale(scoreBackgroundLayerSize.width / scoreRect.size.width);
	}
}

void BB_GameScene::handleScaleBombSizeLabel()
{
	if (m_pBombNumLabel == nullptr) return;

	auto bombNumLabelRect = m_pBombNumLabel->getBoundingBox();
	auto scoreBackgroundLayerSize = m_pBombNumLabel->getParent()->getContentSize();
	if (bombNumLabelRect.size.width > scoreBackgroundLayerSize.width)
	{
		m_pBombNumLabel->setScale(scoreBackgroundLayerSize.width / bombNumLabelRect.size.width);
	}
}

void BB_GameScene::onSaveGameScheduleCallBack(cocos2d::Ref* spender)
{
	// 回调 保存游戏
	CCLOG("on onSaveGameScheduleCallBack");
	// 获取游戏状态
	auto gameStatus = GameStatusManager::getInstance()->getGameStatus();
	if (gameStatus == GameStatusManager::GameStatus::kStatus_Result ||
		gameStatus == GameStatusManager::GameStatus::kStatus_None)
	{
		// 游戏结束状态不存档
		return;
	}
	auto pDataManager = DataManager::getInstance();
	// 保存游戏
	dataChangeJson();

}

void BB_GameScene::showGameRecordScreen()
{
	auto pDataManager = DataManager::getInstance();
	auto& jsonDoc = pDataManager->getJsonDoc();
	// 获取游戏配置
	auto pGameDepolye = GameDeploy::getInstance();
	// 设置块块生命值
	pGameDepolye->setBlockHp(json_check_int32(jsonDoc, JSON_BLOCK_HP));
	// 设置块块概率
	pGameDepolye->setAddCubeBlockProbaility(json_check_int32(jsonDoc, JSON_CUBE_PROBABILITY));
	pGameDepolye->setTriangleBlockAddProbaility(json_check_int32(jsonDoc, JSON_TRIANGLE_PROBABILITY));
	pGameDepolye->setOctagonBlockProbaility(json_check_int32(jsonDoc, JSON_OCTAGON_PROBABILITY));
	pGameDepolye->setBlackHoleBlockAddProbaility(json_check_int32(jsonDoc, JSON_BLACK_HOLE_PROBABILITY));
	// 玩家得分
	m_nScore = 0;
	DataManager::getInstance()->safeModifyData(m_nScore, SAFE_PLAYER_SOCRE_KEY, json_check_int32(jsonDoc, JSON_PLAYER_SCORE), false);
	m_pScoreLabel->setString(StringUtils::format("%d", m_nScore));
	handleScaleScoreLabel();
	// 游戏单局时间
	m_dRoundTime = json_check_double(jsonDoc, JSON_GAME_ROUND_TIME);
	// 已经发射的子弹
	m_nAlreadyShootBombNum = json_check_int32(jsonDoc, JSON_ALREADY_SHOOT_BOMB_NUM);
	// 子弹数
	m_nBombNum = json_check_int32(jsonDoc, JSON_BOMB_SIZE);
	DataManager::getInstance()->safeModifyData(m_nBombNum, SAFE_BOMB_SIZE_KEY, m_nBombNum, false);
	m_pBombNumLabel->setString(StringUtils::format("%d", m_nBombNum));
	handleScaleBombSizeLabel();
	// 读取块块
	const rapidjson::Value& vBlockDataValue = jsonDoc[JSON_BLOCK_DATA];
	addRecordedBlockToScreen(vBlockDataValue);
	// 读取食物
	const rapidjson::Value& vFoodDataValue = jsonDoc[JSON_FOOD_DATA];
	addRecordedFoodToScreen(vFoodDataValue);
	// 读取飞行中的子弹
	const rapidjson::Value& vFlyBombValue = jsonDoc[JSON_FLY_BOMB_DATA];
	addRecordedFlyBombToScreen(vFlyBombValue);
	// 炮台位置
	moveFort(json_check_float(jsonDoc, JSON_FORT_POSX));
	// 已经生成的食物
	BackgroundDataManager::getInstance()->setFoodSize(json_check_int32(jsonDoc, JSON_ALREADY_GENERATED_FOOD_SIZE));
	// 获取飞行中的子弹和未发射的子弹
	m_nMoveBombNum = json_check_int32(jsonDoc, JSON_FLY_BOMB_SIZE);
	// 读取本局得分
	pDataManager->safeModifyData(m_nRoundScore, SAFE_ROUND_SCORE, json_check_int32(jsonDoc, JSON_ROUND_SCORE), true);
	// 读取本局得分等级
	m_nRoundLevel = json_check_int32(jsonDoc, JSON_ROUND_SCORE_LEVEL);
	// 读取子弹发射时的速度
	const rapidjson::Value& bombSpeedValue = jsonDoc[JSON_SHOOT_BOMB_SPEED];
	FortDataManager::getInstance()->setBombSpeed(Vec2(json_check_float(bombSpeedValue, JSON_FLY_BOMB_SPEEDX),
		json_check_float(bombSpeedValue, JSON_FLY_BOMB_SPEEDY)));
	// 获取游戏状态
	auto pGameStatus = GameStatusManager::getInstance();
	GameStatusManager::GameStatus gameStatus = (GameStatusManager::GameStatus)json_check_int32(jsonDoc, JSON_GAME_STATUS);
	if (gameStatus == GameStatusManager::GameStatus::kStatus_ReadyShoot || gameStatus == GameStatusManager::GameStatus::kStatus_None ||
		gameStatus == GameStatusManager::GameStatus::kStatus_Shoot || gameStatus == GameStatusManager::GameStatus::kStatus_WaitMoveEnd)
	{
		pGameStatus->setGameStatus(GameStatusManager::GameStatus::kStatus_AddBomb);
		handleAddBomb();
	}
	pGameStatus->setGameStatus(gameStatus);
}

void BB_GameScene::addRecordedBlockToScreen(const rapidjson::Value& vBlockDataValue)
{
	for (int i = 0; i < vBlockDataValue.Capacity(); i++)
	{
		const rapidjson::Value& blockDataValue = vBlockDataValue[i];
		BlockData blockData;
		blockData.m_eType = (BlockData::Type)json_check_int32(blockDataValue, JSON_BLOCK_TYPE);
		blockData.m_nHP = json_check_int32(blockDataValue, JSON_BLOCK_HP);
		auto pBlockNode = BlockNode::create(blockData);
		pBlockNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		const rapidjson::Value& blockPosValue = blockDataValue[JSON_BLOCK_POINT];
		float posx = json_check_float(blockPosValue, JSON_BLOCK_POINT_X);
		pBlockNode->setPosition(json_check_float(blockPosValue, JSON_BLOCK_POINT_X), json_check_float(blockPosValue, JSON_BLOCK_POINT_Y));
		switch (blockData.m_eType)
		{
		case BlockData::Type::Type_Cube:
		case BlockData::Type::Type_Octagon:
		case BlockData::Type::Type_Triangle:
			m_pBlockNode->addChild(pBlockNode);
			break;
		case BlockData::Type::Type_BlackHole:
			m_pBlackHoleNode->addChild(pBlockNode);
			break;
		default:
			break;
		}
	}
}

void BB_GameScene::addRecordedFoodToScreen(const rapidjson::Value& vFoodDataValue)
{
	for (int i = 0; i < vFoodDataValue.Capacity(); i++)
	{
		const rapidjson::Value& foodDataValue = vFoodDataValue[i];
		FoodData foodData;
		foodData.m_eFoodType = (FoodData::FoodType)json_check_int32(foodDataValue, JSON_FOOD_TYPE);
		auto pFoodNode = FoodNode::create(foodData);
		pFoodNode->setPosition(json_check_float(foodDataValue, JSON_FOOD_POSX), json_check_float(foodDataValue, JSON_FOOD_POSY));
		m_pFoodNode->addChild(pFoodNode);
	}
}

void BB_GameScene::addRecordedFlyBombToScreen(const rapidjson::Value& vFlyBombDataValue)
{
	auto pBombDataManager = BombDataManager::getInstance();
	for (int i = 0; i < vFlyBombDataValue.Size(); i++)
	{
		const rapidjson::Value& flyBombValue = vFlyBombDataValue[i];
		auto pBombNode = pBombDataManager->addBombNode();
		pBombNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pBombNode->setPosition(json_check_float(flyBombValue, JSON_FLY_BOMB_POSX), json_check_float(flyBombValue, JSON_FLY_BOMB_POSY));
		pBombNode->setBombSpeed(Vec2(json_check_float(flyBombValue, JSON_FLY_BOMB_SPEEDX), json_check_float(flyBombValue, JSON_FLY_BOMB_SPEEDY)));
		m_pBombNode->addChild(pBombNode);
		pBombDataManager->addFlyBombToVector(pBombNode);
	}
	// m_nAlreadyShootBombNum = vFlyBombDataValue.Capacity();
}

void BB_GameScene::dataChangeJson()
{
	rapidjson::Document saveJson;
	saveJson.SetObject();
	rapidjson::Document::AllocatorType& allocte = saveJson.GetAllocator();
	// rapidjson::Value saveJson(rapidjson::kArrayType);
	// 玩家得分
	saveJson.AddMember(JSON_PLAYER_SCORE, m_nScore, allocte);
	// 玩家子弹数
	saveJson.AddMember(JSON_BOMB_SIZE, m_nBombNum, allocte);
	// 块块概率
	auto pGameDeploy = GameDeploy::getInstance();
	// 正方形块块生成概率
	saveJson.AddMember(JSON_CUBE_PROBABILITY, pGameDeploy->getAddCubeBlockProbaility(), allocte);
	// 八边形
	saveJson.AddMember(JSON_OCTAGON_PROBABILITY, pGameDeploy->getOctagonBlockProbaility(), allocte);
	// 三角形
	saveJson.AddMember(JSON_TRIANGLE_PROBABILITY, pGameDeploy->getTriangleBlockAddProbaility(), allocte);
	// 黑洞
	saveJson.AddMember(JSON_BLACK_HOLE_PROBABILITY, pGameDeploy->getBlackHoleBlockAddProbaility(), allocte);
	// 块块的生命值
	saveJson.AddMember(JSON_BLOCK_HP, pGameDeploy->getBlockHp(), allocte);
	// 游戏经过时间
	saveJson.AddMember(JSON_GAME_ROUND_TIME, m_dRoundTime, allocte);
	// 写入本局得分
	saveJson.AddMember(JSON_ROUND_SCORE, m_nRoundScore, allocte);
	// 本局得分的等级
	saveJson.AddMember(JSON_ROUND_SCORE_LEVEL, m_nRoundLevel, allocte);
	// 获取游戏状态管理类
	auto pGameStatusManager = GameStatusManager::getInstance();
	// 写入游戏状态
	auto gameStatus = pGameStatusManager->getGameStatus();
	if (gameStatus == GameStatusManager::GameStatus::kStatus_WaitMoveEnd)
	{
		// 等待动画结束状态要特殊处理
		saveJson.AddMember(JSON_GAME_STATUS, (int)GameStatusManager::GameStatus::kStatus_RollScreen, allocte);
	}
	else
	{
		saveJson.AddMember(JSON_GAME_STATUS, (int)gameStatus, allocte);
	}
	// 写入已经发送的子弹个数
	saveJson.AddMember(JSON_ALREADY_SHOOT_BOMB_NUM, m_nAlreadyShootBombNum, allocte);
	// 写入已经生成的食物个数
	saveJson.AddMember(JSON_ALREADY_GENERATED_FOOD_SIZE, BackgroundDataManager::getInstance()->getFoodSize(), allocte);
	// 写入炮台位置
	saveJson.AddMember(JSON_FORT_POSX, FortDataManager::getInstance()->getForPosition().x, allocte);
	// 写入飞行中的子弹和未发射的子弹
	saveJson.AddMember(JSON_FLY_BOMB_SIZE, m_nMoveBombNum, allocte);
	// 写入块块数据
	auto vBlokcNode = m_pBlockNode->getChildren();
	// 写入子弹的速度
	auto bombSpeed = FortDataManager::getInstance()->getBombSpeed();
	rapidjson::Value bombSpeedValue(rapidjson::kObjectType);
	bombSpeedValue.AddMember(JSON_FLY_BOMB_SPEEDX, bombSpeed.x, allocte);
	bombSpeedValue.AddMember(JSON_FLY_BOMB_SPEEDY, bombSpeed.y, allocte);
	saveJson.AddMember(JSON_SHOOT_BOMB_SPEED, bombSpeedValue, allocte);
	// 保存BlockData的json
	rapidjson::Value vBlockDataValue(rapidjson::kArrayType);
	if (gameStatus == GameStatusManager::GameStatus::kStatus_WaitMoveEnd)
	{
		// 特殊处理等待动画结束状态
		auto interfaceSize = GameDeploy::getInstance()->getInterfaceSize();
		for (int i = 0; i < vBlokcNode.size(); i++)
		{
			rapidjson::Value blockDataValue(rapidjson::kObjectType);
			auto pBlockNode = dynamic_cast<BlockNode*>(vBlokcNode.at(i));
			if (pBlockNode == nullptr)	continue;

			blockDataValue.AddMember(JSON_BLOCK_TYPE, (int)pBlockNode->getBlockType(), allocte);
			blockDataValue.AddMember(JSON_BLOCK_HP, pBlockNode->getBLockHp(), allocte);
			// 记录位置
			rapidjson::Value blockPoint(rapidjson::kObjectType);
			blockPoint.AddMember(JSON_BLOCK_POINT_X, pBlockNode->getPositionX(), allocte);
			blockPoint.AddMember(JSON_BLOCK_POINT_Y, pBlockNode->getLastBlockPosition().y, allocte);

			blockDataValue.AddMember(JSON_BLOCK_POINT, blockPoint, allocte);
			vBlockDataValue.PushBack(blockDataValue, allocte);
		}
	}
	else
	{
		for (int i = 0; i < vBlokcNode.size(); i++)
		{
			rapidjson::Value blockDataValue(rapidjson::kObjectType);
			auto pBlockNode = dynamic_cast<BlockNode*>(vBlokcNode.at(i));
			if (pBlockNode == nullptr)	continue;

			blockDataValue.AddMember(JSON_BLOCK_TYPE, (int)pBlockNode->getBlockType(), allocte);
			blockDataValue.AddMember(JSON_BLOCK_HP, pBlockNode->getBLockHp(), allocte);
			// 记录位置
			rapidjson::Value blockPoint(rapidjson::kObjectType);
			blockPoint.AddMember(JSON_BLOCK_POINT_X, pBlockNode->getPositionX(), allocte);
			blockPoint.AddMember(JSON_BLOCK_POINT_Y, pBlockNode->getPositionY(), allocte);
			blockDataValue.AddMember(JSON_BLOCK_POINT, blockPoint, allocte);
			vBlockDataValue.PushBack(blockDataValue, allocte);
		}
	}
	saveJson.AddMember(JSON_BLOCK_DATA, vBlockDataValue, allocte);
	// 写入食物的数据 
	auto vFoodNode = m_pFoodNode->getChildren();
	rapidjson::Value vFoodDataValue(rapidjson::kArrayType);
	for (int i = 0; i < vFoodNode.size(); i++)
	{
		auto pFoodNode = dynamic_cast<FoodNode*>(vFoodNode.at(i));
		if (pFoodNode == nullptr) continue;

		rapidjson::Value foodDataValue(rapidjson::kObjectType);
		// 食物状态
		foodDataValue.AddMember(JSON_FOOD_TYPE, (int)pFoodNode->getFoodType(), allocte);
		// 位置
		foodDataValue.AddMember(JSON_FOOD_POSX, pFoodNode->getPositionX(), allocte);
		foodDataValue.AddMember(JSON_FOOD_POSY, pFoodNode->getPositionY(), allocte);
		vFoodDataValue.PushBack(foodDataValue, allocte);
	}
	saveJson.AddMember(JSON_FOOD_DATA, vFoodDataValue, allocte);
	// 飞行状态的子弹
	auto vMoveBomb = BombDataManager::getInstance()->getFlyBombVector();
	rapidjson::Value vFlyBombValue(rapidjson::kArrayType);
	for (int i = 0; i < vMoveBomb.size(); i++)
	{
		auto pBombNode = vMoveBomb[i];
		rapidjson::Value bombDataValue(rapidjson::kObjectType);
		bombDataValue.AddMember(JSON_FLY_BOMB_POSX, pBombNode->getPositionX(), allocte);
		bombDataValue.AddMember(JSON_FLY_BOMB_POSY, pBombNode->getPositionY(), allocte);
		bombDataValue.AddMember(JSON_FLY_BOMB_SPEEDX, pBombNode->getPhysicsBody()->getVelocity().x, allocte);
		bombDataValue.AddMember(JSON_FLY_BOMB_SPEEDY, pBombNode->getPhysicsBody()->getVelocity().y, allocte);
		vFlyBombValue.PushBack(bombDataValue, allocte);
	}
	saveJson.AddMember(JSON_FLY_BOMB_DATA, vFlyBombValue, allocte);
	// 保存json文件
	DataManager::getInstance()->saveRecord(saveJson);
}

void BB_GameScene::addRoundScore(int nNum, bool bInitial)
{
	auto pDataManger = DataManager::getInstance();
	if (bInitial)
	{
		pDataManger->safeModifyData(m_nRoundScore, SAFE_ROUND_SCORE, nNum, false);
	}
	else
	{
		do
		{
			pDataManger->safeModifyData(m_nRoundScore, SAFE_ROUND_SCORE, nNum, true);
			if (m_nRoundLevel > 3)
			{
				break;
			}
			// 判断是否添加动画
			auto interfaceSize = GameDeploy::getInstance()->getInterfaceSize();
			int nBombNum = BombDataManager::getInstance()->getBombVector().size();

			if (m_nRoundScore >= m_nRoundLevel * nBombNum * 7)
			{
				auto pPraiseActionNode = PraiseActionNode::create((PraiseActionNode::Type)(m_nRoundLevel - 1));
				pPraiseActionNode->setAddedCallBack([this](cocos2d::Node* pLabelBMFontBonus, int nNum) {
					// 添加分数的移动动画
					auto pos = m_pScoreLabel->getParent()->getPosition();
					// 获取坐标
					auto labelBMFontBonusPos = pLabelBMFontBonus->getPosition();
					// 装换坐标
					labelBMFontBonusPos = m_pGameLayer->convertToNodeSpace(pLabelBMFontBonus->getParent()->convertToWorldSpace(labelBMFontBonusPos));

					pLabelBMFontBonus->retain();
					pLabelBMFontBonus->removeFromParent();
					pLabelBMFontBonus->setPosition(labelBMFontBonusPos);
					pLabelBMFontBonus->setOpacity(255);
					m_pGameLayer->addChild(pLabelBMFontBonus);
					pLabelBMFontBonus->release();

					pLabelBMFontBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
					auto pMoveTo = MoveTo::create(1, pos);
					auto pShowSocreFun = CallFunc::create([nNum, this]() {

						// 添加得分
						DataManager::getInstance()->safeModifyData(m_nScore, SAFE_PLAYER_SOCRE_KEY, nNum);
						m_pScoreLabel->setString(StringUtils::format("%d", m_nScore));
						handleScaleScoreLabel();
					});
					auto pSequence = Sequence::create(pMoveTo, pShowSocreFun, RemoveSelf::create(), nullptr);
					pLabelBMFontBonus->runAction(pSequence);

				});
				pPraiseActionNode->setPosition(interfaceSize.width * 0.5f, interfaceSize.height * 0.2f);
				m_pGameLayer->addChild(pPraiseActionNode, 100);
				m_nRoundLevel += 1;
			}
		} while (0);
	}
	CCLOG("addRound m_nRoundScore(%d)", m_nRoundScore);
}
