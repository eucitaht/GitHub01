#include "HelloWorldScene.h"

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
bool HelloWorld::init(){
    if (!Layer::init()){
        return false;
    }
 
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int visibleSizeWidth =visibleSize.width/8;
    
    auto _background_image = cocos2d::Sprite::create("bg2.jpg");
    //    background_image -> setPosition(CCPointZero);
    _background_image -> setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    // get height
    _background_imageHeight = _background_image -> getContentSize().height;
    addChild(_background_image);
    
    auto label = cocos2d::Label::create("0",  "Arial", 30);
    label -> retain();
    label -> setPosition(visibleSize.width-50,visibleSize.height-50);
    label -> setTag(4);
    this -> addChild(label);
    
    
    //技能buttons
    auto _btn1 = cocos2d::Sprite::create("111.png");
    _btn1 -> setScale(0.5);
    _btn1 -> setPosition(Vec2(visibleSizeWidth,visibleSize.height/7));

    this -> addChild(_btn1);
    
    auto _btn2 = Sprite::create("222.png");
    _btn2 -> setScale(0.5);
    _btn2 -> setPosition(Vec2(visibleSizeWidth*2,visibleSize.height/7));
    this -> addChild(_btn2);
    
    auto _btn3 = Sprite::create("333.png");
    _btn3 -> setScale(0.5);
    _btn3 -> setPosition(Vec2(visibleSizeWidth*3,visibleSize.height/7));
    this -> addChild(_btn3);
    
    auto _player = Sprite::create("NabooBomber_archigraphs.png",Rect(0, 0, 256, 256));
    _player -> setScale(0.5);
    _player -> setPosition(Vec2(_player->getContentSize().width/10, visibleSize.height/2));
    this -> addChild(_player,0);
    
    _skills.pushBack(_btn1);
    _skills.pushBack(_btn2);
    _skills.pushBack(_btn3);

    
    //定时器？1秒后执行？
    this->schedule(schedule_selector(HelloWorld::gameLogic),0.5);
    this->schedule(schedule_selector(HelloWorld::scheduleUpdate));
    //每秒定时更新记录数据

    return true;
}

void HelloWorld::addTarget(int type){
    //保存敌机sprite 数组 获得随机数 生成 sprite
    //获得可见大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto _target = Sprite::create("DridStarFighter_archigraphs.png",Rect(0,0,256,256));
    _target -> setScale(0.5);
    //当前精灵的y
    int minY = _target->getContentSize().height/2;
    //场景高度 － 当前精灵的y
    int maxY = visibleSize.height - _target->getContentSize().height/20;
    int rangeY = maxY - minY;
    //获得随机高度 下限是 当前精灵的y
    int actualY = (rand()% rangeY)+minY;
    
    _target -> setPosition(Vec2(visibleSize.width+_target->getContentSize().width/2,actualY));
  
    
    // key
    _target -> setTag(1);
    //add 进入数组
    _targets.pushBack(_target);
    
    
    int minDuration = (int)3.0;
    int maxDuration = (int)6.0;
    int rangeDuration = maxDuration-minDuration;
    int actualDuration = (rand()% rangeDuration) + minDuration;
        //即时Action
    //生成🏁终点
    CCFiniteTimeAction* actionMove = CCMoveTo::create((float)actualDuration, ccp(0 - _target->getContentSize().width/2,actualY));
    //执行完终点之后 在进行remove
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    
    this -> addChild(_target);
    //先到终点 callback执行remove
    _target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
    }


void HelloWorld::spriteMoveFinished(CCNode* sender){
    CCSprite* sprite = (CCSprite *)sender;
    this->removeChild(sprite, true);
    
    if (sprite->getTag()==1) {
        _targets.eraseObject(sprite);
    }else if(sprite->getTag()==2){
        _projectiles.eraseObject(sprite);
    }
}

void HelloWorld::gameLogic(float ft){
    int type = rand()% 2;
    this->addTarget(type);
}

void HelloWorld::onEnter() {
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    //点击 获得坐标?
    listener->onTouchBegan = [=](cocos2d::Touch* touch,cocos2d::Event* event){
        return true;
    };
    //点击后 执行 callback
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    //加入监听组
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto touchPoint = touch->getLocation();
    bool bl = true;
    int skillType;
    //遍历sprite坐标
    for (int i=0; i<_skills.size(); i++) {
       Vec2 vec2 = touchPoint;
       Vec2 vec2BySprite = _skills.at(i) -> getPosition();
        if((vec2.x > vec2BySprite.x-35&&vec2.x < vec2BySprite.x+35)&&(vec2.y > vec2BySprite.y-35&&vec2.y < vec2BySprite.y+35)){
            skillType=i;
            bl = false;
            break;
        }
    }

    if (bl) {
        CCParticleSystem *system = CCParticleSystemQuad::create("click01.plist");
        system -> setPosition(Vec2(touchPoint.x, touchPoint.y));
        system -> setAutoRemoveOnFinish(true);
        this -> addChild(system);
        
        CCParticleSystem *system1 = CCParticleSystemQuad::create("liuxing.plist");
        system1 -> setPosition(Vec2(touchPoint.x, touchPoint.y));
        system1 -> setAutoRemoveOnFinish(true);
        this -> addChild(system1);
        
        auto projectile = Sprite::create("xuehua.png", Rect(0, 0, 256, 256));
        projectile -> setScale(0.3);
        projectile->setPosition(Point(20, visibleSize.height / 2));
        // Determine offset of location to projectile
        int offX = touchPoint.x - projectile->getPosition().x; int offY = touchPoint.y - projectile->getPosition().y;
        // Bail out if we are shooting down or backwards
        if (offX <= 0) return;
        // Ok to add now - we've double checked position
        this->addChild(projectile);
        
        
        projectile -> setTag(2);
        _projectiles.pushBack(projectile);
        
        // Determine where we wish to shoot the projectile to
        int realX = visibleSize.width + (projectile->getContentSize().width / 2);
        float ratio = (float)offY / (float)offX; int realY = (realX * ratio) + projectile->getPosition().y;
        auto realDest = Point(realX, realY);
        // Determine the length of how far we're shooting
        int offRealX = realX - projectile->getPosition().x; int offRealY = realY - projectile->getPosition().y;
        float length = sqrtf((offRealX*offRealX) + (offRealY*offRealY));
        float velocity = 960 / 1;
        // 960pixels/1sec
        float realMoveDuration = length / velocity;
        // Move projectile to actual endpoint
        projectile->runAction(Sequence::create(MoveTo::create(realMoveDuration, realDest), CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveFinished, this)), NULL));
    }else{
        setSkillShow(skillType);
    }
}


void HelloWorld::setSkillShow(int i){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    CCParticleSystem *system3;
    if (i==0) {
        system3 = CCParticleSystemQuad::create("jiguang.plist");
        system3 -> setPosition(Vec2(0,visibleSize.height/2));
    }else if(i==1){
        system3 = CCParticleSystemQuad::create("fangyu01.plist");
        system3 -> setPosition(Vec2(0,visibleSize.height/2));
    }else if(i==2){
        system3 = CCParticleSystemQuad::create("aoe01.plist");
        system3 -> setPosition(Vec2(visibleSize.width/2,visibleSize.height));
    }
    system3 -> setAutoRemoveOnFinish(true);
    this -> addChild(system3);

}

void HelloWorld::scheduleUpdate(float f){
    Vector<cocos2d::Sprite*> _targetDels;
    Vector<cocos2d::Sprite*> _projectilesDels;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    for (int i=0; i<_projectiles.size(); i++) {
        auto projectile = _projectiles.at(i);
        //坐标 xy  当前坐标 － 宽度 ...
        auto projectileRect = Rect(projectile->getPositionX() - projectile->getContentSize().width / 2, projectile->getPositionY() - projectile->getContentSize().height/2,  projectile->getContentSize().width/4, projectile->getContentSize().height/4);
        for (int j=0; j<_targets.size(); j++) {
            auto target = _targets.at(j);
            auto targetRect = Rect(target->getPositionX() - target->getContentSize().width/2, target->getPositionY() - target->getContentSize().height/2, target->getContentSize().width/4, target->getContentSize().height/4);
          
            if (projectileRect.intersectsRect(targetRect)) {
                _targetDels.pushBack(target);
            }
        };
        for (Sprite* t : _targetDels) {
            _targets.eraseObject(t); this->removeChild(t); }
        if (_targetDels.size() >0) {
            _projectilesDels.pushBack(projectile); }
        _targetDels.clear(); }
    for (const auto& p : _projectilesDels) {
        auto touchPoint = p->getPosition();
        CCParticleSystem *system1 = CCParticleSystemQuad::create("liuxing.plist");
        system1 -> setPosition(Vec2(touchPoint.x, touchPoint.y));
        system1 -> setAutoRemoveOnFinish(true);
        this -> addChild(system1);
        _projectiles.eraseObject(p);
        this->removeChild(p);
        _count++;
        this -> removeChildByTag(4);
        std::string s = std::to_string(_count);
        auto label = cocos2d::Label::create(s,"Arial", 30);
        label -> retain();
        label -> setPosition(visibleSize.width-50,visibleSize.height-50);
        label -> setTag(4);
        this -> addChild(label);
    }
    _projectilesDels.clear();
}

void HelloWorld::initBackGround(){
    auto _background_image = cocos2d::Sprite::create("background_image01.jpg");
    //    background_image -> setPosition(CCPointZero);
    _background_image -> setPosition(Vec2(0, 0));
    // get height
    _background_imageHeight = _background_image -> getContentSize().height;
    addChild(_background_image,-10);
}


void HelloWorld::menuCloseCallback(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
  
    
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
