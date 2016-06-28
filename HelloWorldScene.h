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
    
    void setBackground();
    
    //随机位置下落随机生成字符
    void createRandomChar();
    
    //统计正确的字符
    void countCorrect();
    
    //统计正确率
    void countCorrentPersent();
    
    //设置难度 主要控制下落的速度
    void setDifficulty();
    
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
