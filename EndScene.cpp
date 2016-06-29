#include "EndScene.h"
#include "StartScene.h"
Scene* EndScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EndScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool EndScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tuPian.plist");
    
    auto bg = Sprite::createWithSpriteFrameName("bg2.png");
    bg->setScale(visibleSize.width/bg->getContentSize().width, visibleSize.height/bg->getContentSize().height);
    bg->setPosition(Vec2(visibleSize.width/2+origin.x,visibleSize.height/2+origin.y));
    this->addChild(bg);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           [](Ref* Psender){
                                               Director::getInstance()->end();
                                               
                                            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                                               exit(0);
                                            #endif
                                           });
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto back1 = Sprite::createWithSpriteFrameName("backmenu-hd.png");
    back1->setScale(0.5);
    auto back2 = Sprite::createWithSpriteFrameName("backmenu.png");
    back2->setColor(Color3B::GREEN);
    auto backtItem = MenuItemSprite::create(
                                           back1,
                                           back2,
                                           [](Ref* Psender){
                                               Scene* scene = StartScene::createScene();
                                               Director::getInstance()->replaceScene(scene);
                                               
                                            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                                               exit(0);
                                            #endif
                                           });
    
    backtItem->setPosition(Vec2(origin.x + backtItem->getContentSize().width/2 ,
                                origin.y + back2->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu1 = Menu::create(backtItem, NULL);
    menu1->setPosition(Vec2::ZERO);
    this->addChild(menu1, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    std::stringstream s1;
    s1<<"Miss:"<<UserDefault::getInstance()->getIntegerForKey("shiWuShu");
    auto label1 = Label::createWithTTF(s1.str(), "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label1->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2 + label1->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label1, 1);
    
    std::stringstream s2;
    s2<<"GotIt:"<<UserDefault::getInstance()->getIntegerForKey("chengGongShu");
    auto label2 = Label::createWithTTF(s2.str(), "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label2->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2 - label2->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label2, 2);

    UserDefault::getInstance()->flush();
        
    return true;
}


