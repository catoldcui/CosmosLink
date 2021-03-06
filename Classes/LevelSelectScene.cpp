#include "LevelSelectScene.h"
#include "PlayingGameScene.h"
#include "CosLogic.h"
#include "CosResource.h"
#include "VisibleRect.h"
#include "KUtils.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCLayer* NextAction();
CCLayer* BackAction();
CCLayer* RestartAction();

static int s_nActionIdx = -1;

static CCLayer* CreateLayer(int nIndex)
{
    CCLayer * pLayer = NULL;
	char tempStr[30] = {""};
	memset(tempStr,0,sizeof(tempStr));
    sprintf(tempStr,rcLevelCapture,nIndex+1);
	pLayer = new LevelSelectLayer(tempStr);
	cosmos::CosGame::getInstance()->chooseLevel(static_cast<cosmos::Game_Level>(nIndex));
    return pLayer;
}

static CCLayer* NextAction()
{
    ++s_nActionIdx;
    s_nActionIdx = s_nActionIdx % LAYER_COUNT;

    CCLayer* pLayer = CreateLayer(s_nActionIdx);
    pLayer->autorelease();

    return pLayer;
}

static CCLayer* BackAction()
{
    --s_nActionIdx;
    if( s_nActionIdx < 0 )
        s_nActionIdx += LAYER_COUNT;    

    CCLayer* pLayer = CreateLayer(s_nActionIdx);
    pLayer->autorelease();

    return pLayer;
}

/*
static CCLayer* EnterAction()
{
    CCLayer* pLayer = CreateLayer(s_nActionIdx);
    pLayer->autorelease();

    return pLayer;
}
*/


void LevelSelectScene::runThisTest()
{
	s_nActionIdx = -1;
    addChild(NextAction());

    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInT::create(0.5f, this));
}

LevelSelectLayer::LevelSelectLayer(std::string name):
	capture_name(name)
{
}

void LevelSelectLayer::onEnter()
{
	CCLayer::onEnter();

	// add menu
    CCMenuItemImage *item1 = CCMenuItemImage::create(rcLevelSelectBackNormal, rcLevelSelectBackSelect, this, menu_selector(LevelSelectLayer::backCallback) );
    CCMenuItemImage *item2 = CCMenuItemImage::create(rcLevelSelectEnterNormal, rcLevelSelectEnterSelect, this, menu_selector(LevelSelectLayer::enterCallback) );
    CCMenuItemImage *item3 = CCMenuItemImage::create(rcLevelSelectNextNormal, rcLevelSelectNextSelect, this, menu_selector(LevelSelectLayer::nextCallback) );

    CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

    menu->setPosition(CCPointZero);
	item1->setPosition(VisibleRect::left());
	item2->setPosition(VisibleRect::bottom());
	item3->setPosition(VisibleRect::right());

	item1->setAnchorPoint(ccp(0,0.5));
	item2->setAnchorPoint(ccp(0.5,0));
	item3->setAnchorPoint(ccp(1,0.5));

    addChild(menu, 1);

	CCSprite* pSprite = CCSprite::create(capture_name.c_str());

	// Place the sprite on the center of the screen
	//pSprite->setAnchorPoint(ccp(0,0));
	pSprite->setPosition(VisibleRect::center());

	// Add the sprite to HelloWorld layer as a child layer.
	addChild(pSprite, 1);

	// load font
	//create title
	CCLabelBMFont *pLabelTitle = CCLabelBMFont::labelWithString("选择关卡主题", rcImageFont45);
	pLabelTitle->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y-50));
	this->addChild(pLabelTitle,1);

	//load background
	CCSprite* pBack = CCSprite::create(rcLevelSelectBackGround);
	if(pBack){
		// Place the sprite on the center of the screen
		//pSprite->setAnchorPoint(ccp(0,0));
		pBack->setPosition(VisibleRect::center());

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pBack, 0);
	}
}

void LevelSelectLayer::onExit()
{
	CCLayer::onExit();
}

void LevelSelectLayer::enterCallback(CCObject* pSender)
{
	cosmos::CosGame *pGame = cosmos::CosGame::getInstance();
	pGame->startGame();
	KUtils::playSound(rcSoundGameStart);
	PlayingGameScene *pScene = new PlayingGameScene();
	if(pScene){
		pScene->runThisTest();
		pScene->release();
	}
}

void LevelSelectLayer::nextCallback(CCObject* pSender)
{
	CCScene* s = new LevelSelectScene();
    s->addChild( NextAction() );
	CCDirector::sharedDirector()->replaceScene(PageTransitionForward::create(0.6f,s));
	//播放音效
	KUtils::playSound(rcSoundBook);
    s->release();
}

void LevelSelectLayer::backCallback(CCObject* pSender)
{
	CCScene* s = new LevelSelectScene();
    s->addChild( BackAction() );
	CCDirector::sharedDirector()->replaceScene(PageTransitionBackward::create(0.6f,s));
	//播放音效
	SimpleAudioEngine::sharedEngine()->playEffect(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(rcSoundBook)).c_str());
    s->release();
}
