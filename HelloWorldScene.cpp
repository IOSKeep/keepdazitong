#include "HelloWorldScene.h"
#include "ui/cocosGUI.h"

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


    
    //设置背景
    setBackground();
    
    //统计结果
    countCorrentPersent();
    
    
    //初始化结果
    amountChar = 0;
    correctChar = 0;
    //设置难度 主要控制下落的速度
    setDifficulty();
    
    
    
    //所有定时器
    
    
    //下落字母定时器
    
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::createRandomChar), 0.5);
    
    this->scheduleUpdate();
    
    
    //初始化字母表            q,    w
    chArray  = new char[36]{'0', '1', '2', '3','4',  '5', '6', '7', '8', '9',
        'A' , 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
        };
    
    
    //控制键盘监听
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
            if((*it)->getColor() != Color3B::GREEN)
            {
                std::string tmpStr = (*it)->getString();
                char tmpCh = tmpStr[0];
                if(tmpCh == ch)
                {
                    //正确把字体设置为绿色
                    (*it)->setColor(Color3B::GREEN);
                    
                    
                    //添加粒子效果
                    auto position = (*it)->getPosition();
                    auto rightParticle = ParticleSystemQuad::create("right.plist");
                    rightParticle->setPositionType(ParticleSystem::PositionType::FREE);
                    rightParticle->setPosition(position);
                    rightParticle->setAutoRemoveOnFinish(true);
                    this->addChild(rightParticle);
                    
                    //正确的数量加一
                    correctChar++;
                    
                }

            }
            
        }
  
    };
    auto dispather = Director::getInstance()->getEventDispatcher();
    dispather->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    
    //
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
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto sprite1 = Sprite::create("bgMap5.png");
    sprite1->setScale(visibleSize.width/sprite1->getContentSize().width, 1);
    sprite1->setPosition(sprite1->getContentSize().width/2, sprite1->getContentSize().height/2);
    
    
    auto move1 = MoveTo::create(10, Vec2(sprite1->getContentSize().width/2,
                                         -(sprite1->getContentSize().height/2 - visibleSize.height)));
    auto place = Place::create(Vec2(sprite1->getContentSize().width/2,
                                    sprite1->getContentSize().height/2));
    this->addChild(sprite1);
    auto sequence = Sequence::create(move1, place, NULL);
    auto repeat = RepeatForever::create(sequence);
    sprite1->runAction(repeat);
    
}
void HelloWorld::setDifficulty()
{
    
}
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
    for(j = 0; j < onScreeCharCount; j++)
    {
        for(i = j; i < 36 && j < onScreeCharCount; i++)
        {
            auto tmpLable = dequeLabel[j];
            std::string tmpStr = tmpLable->getString();  //
            char tmpChar = tmpCharArray[i];
            if(tmpChar == tmpStr[0])
            {
                char tmp = tmpCharArray[i];
                tmpCharArray[i] = tmpCharArray[j];
                tmpCharArray[j] = tmp;
                break;
            }
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
    
    //产生的字母总数加一
    amountChar++;
//    ss.clear();
//    ss << amountChar;
//    auto labelCorrent = (Label* )this->getChildByTag(001);
//    labelCorrent->setTTFConfig(ss.str());

    
    
}
//统计正确率
void HelloWorld::countCorrentPersent()
{
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    auto label1= Label::createWithTTF("amount: ", "fonts/arial.ttf", 24);
//    
//    
//    std::stringstream ss;
//    ss << amountChar;
//    auto labelAmount = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 24);
//   // this->addChild(labelAmount, 0, 001);
//    
//    auto label2 = Label::createWithTTF("corrent: ","fonts/arial.ttf", 24);
//    
//    ss.clear();
//    auto correntLabel = Label::createWithTTF(ss.str(), "fonts/arial.ttf", 24);
//    
//    auto hbox = ui::HBox::create(Size(600, 400));
//;
//    label1->setPosition(hbox->getPosition().x+ label1->getContentSize().width/2,
//                        hbox->getPosition().y + label1->getContentSize().height/2);
//    labelAmount->setPosition(label1->getPosition().x + label1->getContentSize().width/2,
//                             label1->getPosition().y + label1->getContentSize().height/2);
//    hbox->addChild(label1);
//    hbox->addChild(labelAmount);
//    hbox->addChild(label2);
//    hbox->addChild(correntLabel);
    
//    
//    hbox->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
//    this->addChild(hbox);
    
    
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
