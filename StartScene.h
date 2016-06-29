#ifndef __StartScene_SCENE_H__
#define __StartScene_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class StartScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(StartScene);
    
    void scheduleCallBack(float delta);
    void update(float delta);
    void sColor(Sprite* s);
    
    Sprite* ziMu;
    Vec2 location;
    int shiWuShu;
    SpriteBatchNode* ziMuBatchNode;
    int chengGongShu;
    bool released;
};

#endif // __StartScene_SCENE_H__
