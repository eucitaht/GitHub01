#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h" 
#include "SimpleAudioEngine.h"
//#include "gui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

using namespace cocos2d::extension;

//class HelloWorld : public cocos2d::LayerColor{
class HelloWorld : public cocos2d::Layer{
private:
    //创建精灵数组
    cocos2d::Vector<cocos2d::Sprite*> _targets;
    cocos2d::Vector<cocos2d::Sprite*> _projectiles;
    cocos2d::Vector<cocos2d::Sprite*> _skills;
    //创建敌机数组
    cocos2d::Vector<cocos2d::Sprite*> _enemys;
    int _background_imageHeight;
    int _count;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    //init background_image
    void initBackGround();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    //添加敌人并移动
    void addTarget(int type);
    //销毁敌人
    void spriteMoveFinished(cocos2d::CCNode* sender);
    //执行
    void gameLogic(float ft);
    //触碰监听
    void onEnter();
    //加载飞行精灵 飞行算法 设置终点MoveTo 销毁 ..直接copy
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    //触碰
    void scheduleUpdate(float f);
    
    //选择技能
    void setSkillShow(int i);
//    virtual bool ccTouchBegan(cocos2d::Touch* touch, cocos2d::CCEvent* event);//按下
    void updateCount(float f);
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
