#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PaiGow/PaiGow.h"
using namespace cocos2d;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
	//Ìí¼Ó±³¾°»­²¼
	auto pBg = LayerColor::create(Color4B::BLUE, _contentSize.width, _contentSize.height);
	addChild(pBg);

	auto p = PaiGow::create("PaiGow/Brank.png", "PaiGow/Red.png", "PaiGow/White.png", 1, true);
	//if (p)
	//{
	//	p->setTag(100);
	//	//p->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//	 p->setPosition(_contentSize*0.5f);

	//	const auto& size = p->getContentSize();
	//	CCLOG("size: %f, %f", size.width, size.height);
	//	addChild(p);
	//}

	//auto pLabel = Label::createWithSystemFont("Hello", "", 60);
	//pLabel->setPosition(_contentSize * 0.5f);
	//addChild(pLabel);
	
	Vector<PaiGow *> spritepais;
	for (auto i = 0; i <= 20; i++)
	{
		log("%d", i);
		auto spritepai = PaiGow::create("paigow/brank.png", "paigow/red.png", "paigow/white.png", i, true);
		spritepais.pushBack(spritepai);
	}


	int i = 0;
	for (auto sprite : spritepais)
	{
		sprite->setScale(0.47f);
		sprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		sprite->setPosition(Vec2((visibleSize.width - sprite->getContentSize().width * 0.5f * 7) / 2 + sprite->getContentSize().width *0.50f * (i % 7) ,
								visibleSize.height - (sprite->getContentSize().height * 0.50f * (int)(i / 7 ))));
		addChild(sprite);
		i++;

	}
   
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	static int nNum = 0;
	auto p = dynamic_cast<PaiGow*>(getChildByTag(100));
	
	if (p)
	{
		if (nNum > 20) nNum = 0;
		p->setSurface(nNum++, nNum % 2);
	}
    //Close the cocos2d-x game scene and quit the application
//    Director::getInstance()->end();
//
//    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
