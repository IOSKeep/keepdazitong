#include "HelloWorldScene.h"
#include "ui/cocosGUI.h"

USING_NS_CC;
static int dif=1;
static int num;
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
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    //    // add a "close" icon to exit the progress. it's an autorelease object
    //    auto closeItem = MenuItemImage::create(
    //                                           "CloseNormal.png",
    //                                           "CloseSelected.png",
    //                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    //
    //	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
    //                                origin.y + closeItem->getContentSize().height/2));
    //
    //    // create menu, it's an autorelease object
    //    auto menu = Menu::create(closeItem, NULL);
    //    menu->setPosition(Vec2::ZERO);
    //    this->addChild(menu, 1);
    //
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    //
    //    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    //
    //
    //    // position the label on the center of the screen
    //    label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                            origin.y + visibleSize.height - label->getContentSize().height));
    //
    //    // add the label as a child to this layer
    //    this->addChild(label, 1);
    //
    //    // add "HelloWorld" splash screen"
    //    auto sprite = Sprite::create("HelloWorld.png");
    //
    //
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    //
    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    
    //设置背景
    setBackground();
    
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::createRandomChar), 2.5);
    //设置难度 主要控制下落的速度
    
    setDifficulty();
    
    
    
    //所有定时器
    
    
    //下落字母定时器
    
    this->scheduleUpdate();
    
    
    //初始化字母表            q,    w
    chArray  = new char[36]{'0', '1', '2', '3','4',  '5', '6', '7', '8', '9',
        'A' , 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
    };
    
    
    //控制键盘监听
    
    //ch[256] ={0};ewq
    //char ch[256]; ch[(int)q] = 1;
    //ch[(int)t] = 0;
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* e){
        //拿到输入的键值
        int keyCodeInt = (int)keyCode;
        char ch;
        CCLOG("%d", keyCodeInt);
        if(keyCodeInt >= 76 && keyCodeInt <= 85)
        {
            ch = this->chArray[keyCodeInt-76];
        }
        else if(keyCodeInt >= 124 && keyCodeInt <= 149)
        {
            ch = this->chArray[keyCodeInt-124 + 10];
        }
        else
        {
            return;
        }
        
        for(std::deque<Label* >::iterator it = dequeLabel.begin(); it != dequeLabel.end(); it++)
        {
            std::string tmpStr = (*it)->getString();
            char tmpCh = tmpStr[0];
            if(tmpCh == ch)
            {
                (*it)->setColor(Color3B::GREEN);
            }
        }
        
    };
    auto dispather = Director::getInstance()->getEventDispatcher();
    dispather->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
void HelloWorld::update(float delta)
{
    
    //清除不在屏幕上的字母
    while(!dequeLabel.empty())
    {
        Label* label = dequeLabel.front();
        if(label->getPosition().y == (-label->getContentSize().height/2))
        {
            //如果它的位置到达了目的点就移除它
            Label* tmpLabel = dequeLabel.front();
            dequeLabel.pop_front();
            this->removeChild(tmpLabel);
        }
        else
        {
            break;
        }
    }
}
void HelloWorld::setBackground()
{
    
}
//设置难度 主要控制下落的速度
void HelloWorld::setDifficulty()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto slider=ui::Slider::create();
    
    slider->setPosition(Point( visibleSize.width-20 ,origin.y+ visibleSize.height/2));
    slider->setMaxPercent(100);
    slider->setRotation(270);
    slider->loadBarTexture("Slider_Back.png");
    slider->loadSlidBallTextures("SliderNode_Normal.png");
    slider->loadProgressBarTexture("Slider_PressBar.png");
    slider->addEventListener([](Ref* ref,cocos2d::ui::Slider::EventType e){
        num= static_cast<cocos2d::ui::Slider*>(ref)->getPercent();
        
        CCLOG("%d",num);
    });
    auto mouse=EventListenerMouse::create();
    mouse->onMouseMove=[](EventMouse* event){};
    mouse->onMouseDown=[](EventMouse* event){};
    mouse->onMouseUp=[slider, this](EventMouse* event){
        if(num<25)
        {
            slider->setPercent(0);
            dif=1;
        }
        else if (num<75)
        {
            slider->setPercent(50);
            dif=2;
        }
        else
        {
            slider->setPercent(100);
            dif=3;
        }
        this->unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::createRandomChar));
        this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::createRandomChar), 3.5-(double)dif);
        
        
    };
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(mouse, this);
    this->addChild(slider,1,101);
    
    
};

//返回设置难度，默认难度为1
int getDifficulty()
{
    return dif;
};
//随机位置下落随机生成字符
//76 - 85
//124 - 149
void HelloWorld::createRandomChar(float delta)
{
    
    
    //得到屏幕上的字母数量
    int onScreeCharCount = dequeLabel.size();
    char* tmpCharArray  = (char* )malloc(36 * sizeof(char));
    memcpy(tmpCharArray, chArray, 36 * sizeof(char));
    int i = 0, j = 0;
    
    for(i = 0; i < 36 && j < onScreeCharCount; i++)
    {
        auto tmpLable = dequeLabel[j];
        std::string tmpStr = tmpLable->getString();  //
        char tmpChar = tmpCharArray[i];
        if(tmpChar == tmpStr[0])
        {
            char tmp = tmpCharArray[i];
            tmpCharArray[i] = tmpCharArray[j];
            tmpCharArray[j] = tmp;
            j++;
        }
    }
    //处屏幕上的字母外随机剩下的字母
    int index = clock() % (36 - onScreeCharCount) + onScreeCharCount;
    char ch = tmpCharArray[index];
    
    free(tmpCharArray);
    
    std::stringstream ss;
    
    ss << ch;
    std::string str = ss.str();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    
    auto label = Label::createWithTTF(str, "fonts/Marker Felt.ttf", 32);
    
    //一行字母可放的最多数量
    int labelCount = visibleSize.width / label->getContentSize().width;
    
    //一行的随机位置
    srand((unsigned int)time(NULL));
    int randomX = (rand() % labelCount) * label->getContentSize().width + label->getContentSize().width/2;
    
    
    label->setPosition(randomX , visibleSize.height + label->getContentSize().height/2);
    auto move = MoveTo::create(5, Vec2(randomX, -label->getContentSize().height/2));
    label->runAction(move);
    this->addChild(label);
    
    //把产生的label加入到一个队列里面
    dequeLabel.push_back(label);
    
}
//统计正确率
void HelloWorld::countCorrentPersent()
{
    
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
