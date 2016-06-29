#ifndef __EndScene_SCENE_H__
#define __EndScene_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class EndScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuBackCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(EndScene);
};

#endif // __EndScene_SCENE_H__
