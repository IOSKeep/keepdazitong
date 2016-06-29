#include "HelloWorldScene.h"
#include "StartScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //载入图片纹理
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tuPian.plist");
    
    //载入背景
    auto bg = Sprite::createWithSpriteFrameName("bg2.png");
    bg->setAnchorPoint(Point(0.5,0.5));
    bg->setScale(visibleSize.width/bg->getContentSize().width,visibleSize.height/bg->getContentSize().height);
    bg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
    this->addChild(bg);
    
    //载入开始按钮
    //MenuItemSprite用来创建由sprite创建的图片按钮
    Sprite* png1 = Sprite::createWithSpriteFrameName("restart-hd.png");
    png1->setScale(0.5);
    Sprite* png2 = Sprite::createWithSpriteFrameName("restart.png");
    png2->setColor(Color3B::RED);
    auto startItem = MenuItemSprite::create(
                                           png1,
                                           png2,
                                           [](Ref* pSender){
                                               Scene* scene = StartScene::createScene();
                                               Director::getInstance()->pushScene(scene);
                                            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                                               exit(0);
                                            #endif

                                           });
    startItem->setPosition(origin.x + visibleSize.width/2,origin.y+png2->getContentSize().height);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(startItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    //载入标题
    auto label = Label::createWithTTF("catch the char", "fonts/Marker Felt.ttf", 24);
    
    label->enableOutline(Color4B::GREEN,2);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);

    
    return true;
}

