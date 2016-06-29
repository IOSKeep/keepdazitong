#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    //设置背景
    
    void setBackground();  //胡凯翔
    
    //随机位置下落随机生成字符
    void createRandomChar(float delta);
    
    //字母表
    char *chArray;
    
    //在屏幕上的字符数组
    char *resultCharArray;
    
    std::deque<cocos2d::Label* > dequeLabel;
    //统计正确的字符
    void countCorrect();
    
    //所有字符数
    int amountChar;
    //所有正确的字符数
    int correctChar;
    //正确率
    int correntPersent;
    
    //统计正确率
    void countCorrentPersent();
    
    //设置难度 主要控制下落的速度
    void setDifficulty();
    
    //扫描队列
    void update(float delta);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
