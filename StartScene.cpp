#include "StartScene.h"
#include "ui/cocosGUI.h"
#include "EndScene.h"

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    shiWuShu=0;
    chengGongShu=0;
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           [](Ref* pSender){
                                               Scene* scene = EndScene::createScene();
                                               Director::getInstance()->pushScene(scene);
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

    //载入图片
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tuPian.plist");
    auto texture = Director::getInstance()->getTextureCache()->addImage("tuPian.pvr.ccz");
    ziMuBatchNode = SpriteBatchNode::createWithTexture(texture);
    
    //添加背景
    auto bg = Sprite::createWithSpriteFrameName("bg3.png");
    bg->setScale(visibleSize.width/bg->getContentSize().width, visibleSize.height/bg->getContentSize().height);
    bg->setPosition(Vec2(visibleSize.width/2+origin.x,visibleSize.height/2+origin.y));
    this->addChild(bg);
    
    //载入字母
    this->schedule(CC_SCHEDULE_SELECTOR(StartScene::scheduleCallBack), 0.8);
     //添加字母
    this->addChild(ziMuBatchNode);
    
    //移除运动出屏幕的孩子
    this->scheduleUpdate();
    
    //随机颜色
    
    
    auto el = EventListenerKeyboard::create();
    el->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event){
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        CCLOG("%d",keyCode);
        if (int(keyCode)==12||int(keyCode)==13)
        {
            released=false;
        }
        if (released==false)
        {
            {
                for (int i =0 ; i<26; i++)
                {
                    if(int(keyCode)==i+124)
                    {
                        Node *child = ziMuBatchNode->getChildByTag(i+127);
                        if (child == nullptr)
                        {
                            CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", i+127);
                        }
                        else
                        {
                            auto system = ParticleSystemQuad::create("gotIt.plist");
                            system->setPosition(child->getPosition());
                            this->addChild(system);
                            ziMuBatchNode->removeChildByTag(i+127);
                            chengGongShu++;
                        }
                    }
                }
          UserDefault::getInstance()->setIntegerForKey("chengGongShu",chengGongShu);
                CCLOG("%d", UserDefault::getInstance()->getIntegerForKey("chengGongShu"));
            }
        }
        else
        {
            for (int i =0 ; i<26; i++)
            {
                if(int(keyCode)==i+124)
                {
                    Node *child = ziMuBatchNode->getChildByTag(i+101);
                    if (child == nullptr)
                    {
                        CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", i+101);
                    }
                    else
                    {
                        
                        auto system = ParticleSystemQuad::create("gotIt.plist");
                        system->setPosition(child->getPosition());
                        this->addChild(system);
                        ziMuBatchNode->removeChildByTag(i+101);
                        chengGongShu++;
                    }

                }
            }
     
            UserDefault::getInstance()->setIntegerForKey("chengGongShu",chengGongShu);
            
            CCLOG("%d", UserDefault::getInstance()->getIntegerForKey("chengGongShu"));
        }
    };
    el->onKeyReleased = [this](EventKeyboard::KeyCode keyCode,Event* event){
        CCLOG("%d",keyCode);
        released=true;
    };
    
    auto dis = Director::getInstance()->getEventDispatcher();
    dis->addEventListenerWithSceneGraphPriority(el,this);

    

    return true;
}

void StartScene::sColor(Sprite* s)
{
    int yanSe = arc4random()%8;
    int fangXiang = arc4random()%10;
    int rotation = arc4random()%135;
    Action* rotate;
    if (fangXiang<=5) {
        rotate = RotateBy::create(4, rotation);
    }
    else
    {
        rotate = RotateBy::create(4, -rotation);
    }
    s->runAction(rotate);
    switch (yanSe) {
        case 1:
            s->setColor(Color3B::GREEN);
            break;
        case 2:
            s->setColor(Color3B::GRAY);
            break;
        case 3:
            s->setColor(Color3B::MAGENTA);
            break;
        case 4:
            s->setColor(Color3B::ORANGE);
            break;
        case 5:
            s->setColor(Color3B::RED);
            break;
        case 6:
            s->setColor(Color3B::WHITE);
            break;
        case 7:
            s->setColor(Color3B::YELLOW);
            break;
        default:
            break;
    }
}

void StartScene::update(float delta)
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto vec = ziMuBatchNode->getDescendants();
    for (std::vector<Sprite*>::iterator iter=vec.begin(); iter!=vec.end();iter++)
    {
        if ((*iter)->getPositionY()<origin.y) {
            ziMuBatchNode->removeChild(*iter, false);
            shiWuShu++;
            UserDefault::getInstance()->setIntegerForKey("shiWuShu",shiWuShu);
            CCLOG("%d", UserDefault::getInstance()->getIntegerForKey("shiWuShu"));
        }
    }
}

void StartScene::scheduleCallBack(float delta)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        int count = visibleSize.width/Sprite::createWithSpriteFrameName("a.png")->getContentSize().width;
        int ziMuHao = arc4random()%52;
        int x = arc4random()%count;
        int isShow =arc4random()%100;
        ziMu = Sprite::createWithSpriteFrameName("a.png");
        auto move =
        ::create(4,Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2+visibleSize.height*1.5)));
        if (isShow>=20)
        {
            switch (ziMuHao)
            {
                case 1:
                    ziMu = Sprite::createWithSpriteFrameName("a.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);
                    ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,101);
                    break;
                case 2:
                    ziMu = Sprite::createWithSpriteFrameName("b.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->   getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);
                    ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,102);
                    break;
                case 3:
                    ziMu = Sprite::createWithSpriteFrameName("c.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->   getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,103);
                    break;
                case 4:
                    ziMu = Sprite::createWithSpriteFrameName("d.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,104);
                    break;
                case 5:
                    ziMu = Sprite::createWithSpriteFrameName("e.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,105);
                    break;
                case 6:
                    ziMu = Sprite::createWithSpriteFrameName("f.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,106);
                    break;
                case 7:
                    ziMu = Sprite::createWithSpriteFrameName("g.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,107);
                    break;
                case 8:
                    ziMu = Sprite::createWithSpriteFrameName("h.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,108);
                    break;
                case 9:
                    ziMu = Sprite::createWithSpriteFrameName("i.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,109);
                    break;
                case 10:
                    ziMu = Sprite::createWithSpriteFrameName("j.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,110);
                    break;
                case 11:
                    ziMu = Sprite::createWithSpriteFrameName("k.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,111);
                    break;
                case 12:
                    ziMu = Sprite::createWithSpriteFrameName("l.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,112);
                    break;
                case 13:
                    ziMu = Sprite::createWithSpriteFrameName("m.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,113);
                    break;
                case 14:
                    ziMu = Sprite::createWithSpriteFrameName("n.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,114);
                    break;
                case 15:
                    ziMu = Sprite::createWithSpriteFrameName("o.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,115);
                    break;
                case 16:
                    ziMu = Sprite::createWithSpriteFrameName("p.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,116);
                    break;
                case 17:
                    ziMu = Sprite::createWithSpriteFrameName("q.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,117);
                    break;
                case 18:
                    ziMu = Sprite::createWithSpriteFrameName("r.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,118);
                    break;
                case 19:
                    ziMu = Sprite::createWithSpriteFrameName("s.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,119);
                    break;
                case 20:
                    ziMu = Sprite::createWithSpriteFrameName("t.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,120);
                    break;
                case 21:
                    ziMu = Sprite::createWithSpriteFrameName("u.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,121);
                    break;
                case 22:
                    ziMu = Sprite::createWithSpriteFrameName("v.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,122);
                    break;
                case 23:
                    ziMu = Sprite::createWithSpriteFrameName("w.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,123);
                    break;
                case 24:
                    ziMu = Sprite::createWithSpriteFrameName("x.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,124);
                    break;
                case 25:
                    ziMu = Sprite::createWithSpriteFrameName("y.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,125);
                    break;
                case 26:
                    ziMu = Sprite::createWithSpriteFrameName("z.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,126);
                    break;
                case 27:
                    ziMu = Sprite::createWithSpriteFrameName("a1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,127);
                    break;
                case 28:
                    ziMu = Sprite::createWithSpriteFrameName("b1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,128);
                    break;
                case 29:
                    ziMu = Sprite::createWithSpriteFrameName("c1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,129);
                    break;
                case 30:
                    ziMu = Sprite::createWithSpriteFrameName("d1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,130);
                    break;
                case 31:
                    ziMu = Sprite::createWithSpriteFrameName("e1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,131);
                    break;
                case 32:
                    ziMu = Sprite::createWithSpriteFrameName("f1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,132);
                    break;
                case 33:
                    ziMu = Sprite::createWithSpriteFrameName("g1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,133);
                    break;
                case 34:
                    ziMu = Sprite::createWithSpriteFrameName("h1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,134);
                    break;
                case 35:
                    ziMu = Sprite::createWithSpriteFrameName("i1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,135);
                    break;
                case 36:
                    ziMu = Sprite::createWithSpriteFrameName("j1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,136);
                    break;
                case 37:
                    ziMu = Sprite::createWithSpriteFrameName("k1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,137);
                    break;
                case 38:
                    ziMu = Sprite::createWithSpriteFrameName("l1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,138);
                    break;
                case 39:
                    ziMu = Sprite::createWithSpriteFrameName("m1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,139);
                    break;
                case 40:
                    ziMu = Sprite::createWithSpriteFrameName("n1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,140);
                    break;
                case 41:
                    ziMu = Sprite::createWithSpriteFrameName("o1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,141);
                    break;
                case 42:
                    ziMu = Sprite::createWithSpriteFrameName("p1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,142);
                    break;
                case 43:
                    ziMu = Sprite::createWithSpriteFrameName("q1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,143);
                    break;
                case 44:
                    ziMu = Sprite::createWithSpriteFrameName("r1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,144);
                    break;
                case 45:
                    ziMu = Sprite::createWithSpriteFrameName("s1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,145);
                    break;
                case 46:
                    ziMu = Sprite::createWithSpriteFrameName("t1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,146);
                    break;
                case 47:
                    ziMu = Sprite::createWithSpriteFrameName("u1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,147);
                    break;
                case 48:
                    ziMu = Sprite::createWithSpriteFrameName("v1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,148);
                    break;
                case 49:
                    ziMu = Sprite::createWithSpriteFrameName("w1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,149);
                    break;
                case 50:
                    ziMu = Sprite::createWithSpriteFrameName("x1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,150);
                    break;
                case 51:
                    ziMu = Sprite::createWithSpriteFrameName("y1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,151);
                    break;
                case 52:
                    ziMu = Sprite::createWithSpriteFrameName("z1.png");this->sColor(ziMu);
                    location = Director::getInstance()->convertToGL(Point(ziMu->getContentSize().width/2.5+x*ziMu->getContentSize().width,ziMu->getContentSize().height/2));
                    ziMu->setPosition(location);ziMu->runAction(move);
                    ziMuBatchNode->addChild(ziMu,1,152);
                    break;
                default:
                    break;
            }
        }
    }

