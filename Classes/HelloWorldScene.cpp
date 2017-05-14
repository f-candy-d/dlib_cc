#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "dlib_cc/mobius_strip/mobius_strip.h"
#include "dlib_cc/lts_map/buffer.h"
#include "dlib_cc/fast_shift_table/fast_shift_table.h"
#include <iostream>

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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    dlib_cc::lts_map_unit::Buffer buff(5 ,5, 444);
    // buff.DisableQueuingAfterWrite(true);
    // dlib_cc::fast_shift_table<int> buff(5, 5, 444);
    for(int y = 5 - 1; 0 <= y; --y)
    {
        for(int x = 0; x < 5; ++x)
        {
            std::cout << buff(x, y) << ", ";
        }

        std::cout << '\n';
    }
    std::cout << '\n';

    for(int y = 5 - 1; 0 <= y; --y)
    {
        for(int x = 0; x < 5; ++x)
        {
            buff.Write(x, y, 10 + y * 5 + x);
            // buff(x, y) = 10 + y * 5 + x;
            std::cout << buff.Read(x, y) << ", ";
            // std::cout << buff(x, y) << ", ";
        }

        std::cout << '\n';
    }

    std::cout << "buff.Read(4, 3) = " << buff.Read(4, 3) << '\n';
    std::cout << "buff.Read(2, 1) = " << buff.Read(2, 1) << '\n';
    std::cout << "buff.Read(2, 4) = " << buff.Read(2, 4) << '\n';

    // std::cout << "buff.Read(4, 3) = " << buff(4, 3) << '\n';
    // std::cout << "buff.Read(2, 1) = " << buff(2, 1) << '\n';
    // std::cout << "buff.Read(2, 4) = " << buff(2, 4) << '\n';

    std::cout << "in que => ";
    for(auto a : buff.rendering_queue())
    {
        std::cout << a << ", ";
    }

    std::cout << '\n';

    std::cout << "log using unordered-iterator" << '\n';

    int c = 0;
    for(auto v : buff)
    {
        std::cout << v << ", ";
        ++c;
        if(c == 5)
        {
            std::cout << '\n';
            c = 0;
        }
    }

    std::cout << '\n';

    std::cout << "log using row-iterator (row 4)" << '\n';
    std::cout << "row 4 => ";
    for(auto itr = buff.begin_row(4); itr != buff.end_row(4); ++itr)
    {
        std::cout << *itr << ", ";
    }

    std::cout << "\n\n";

    std::cout << "log using column-iterator (column 3)" << '\n';
    std::cout << "column 3 => ";
    for(auto itr = buff.begin_column(3); itr != buff.end_column(3); ++itr)
    {
        std::cout << *itr << ", ";
    }

    std::cout << "\n\n";

    auto rt = buff.loading_range();
    std::cout << "rect(l,b,r,t) => " <<  rt.left << "," << rt.bottom << "," << rt.right << "," << rt.top << '\n';
    std::cout << "size(rect) => " << rt.size() << '\n';
    std::cout << "size => " << buff.Size() << '\n';

    buff.ShiftColumns(-2);
    buff.ShiftRows(2);
    // buff.shift_columns(-2);
    // buff.shift_rows(2);

    std::cout << "shifted" << '\n';

    for(int y = 5 - 1; 0 <= y; --y)
    {
        for(int x = 0; x < 5; ++x)
        {
            std::cout << buff.Read(x, y) << ", ";
            // std::cout << buff(x, y) << ", ";
        }

        std::cout << '\n';
    }
    std::cout << '\n';

    std::cout << "log using row-iterator (row 4)" << '\n';
    std::cout << "row 4 => ";
    for(auto itr = buff.begin_row(4); itr < buff.end_row(4); ++itr)
    {
        std::cout << *itr << ", ";
    }

    std::cout << "\n\n";


    std::cout << "log using column-iterator (column 3)" << '\n';
    std::cout << "[column 3]\n";
    for(auto itr = buff.begin_column(3); itr != buff.end_column(3); ++itr)
    {
        std::cout << *itr << " : (" << itr.position.x << ", " << itr.position.y << ")\n";
    }

    std::cout << "\n";

    std::cout << "log using row-iterator (row 4, reverse)" << '\n';
    std::cout << "[row 4 reverse]\n";
    for(auto itr = buff.end_row(4) - 1; itr != buff.begin_row(4) - 1; --itr)
    {
        std::cout << *itr << " : (" << itr.position.x << ", " << itr.position.y << ")\n";
    }

    std::cout << "\n";

    std::cout << "log using column-iterator (column 3, reverse)" << '\n';
    std::cout << "column 3 => ";
    for(auto itr = buff.end_column(3) - 1; itr != buff.begin_column(3) - 1; --itr)
    {
        std::cout << *itr << ", ";
    }

    std::cout << "\n\n";

    std::cout << "*(buff.begin_row(4) + 2) = 99" << '\n';
    auto itr = buff.begin_row(4);
    *(itr + 2) = 99;

    std::cout << "log using row-iterator (row 4)" << '\n';
    std::cout << "row 4 => ";
    for(auto itr = buff.begin_row(4); itr < buff.end_row(4); ++itr)
    {
        std::cout << *itr << ", ";
    }

    std::cout << "\n\n";

    std::cout << "*(buff.begin_column(3) + 2) = 77" << '\n';
    auto itrc = buff.begin_column(3);
    *(itrc + 2) = 77;

    std::cout << "log using column-iterator (column 3)" << '\n';
    std::cout << "column 3 => ";
    for(auto itr = buff.begin_column(3); itr != buff.end_column(3); ++itr)
    {
        std::cout << *itr << ", ";
    }

    std::cout << "\n\n";

    for(int y = 5 - 1; 0 <= y; --y)
    {
        for(int x = 0; x < 5; ++x)
        {
            std::cout << buff.Read(x, y) << ", ";
            // std::cout << buff(x, y) << ", ";
        }

        std::cout << '\n';
    }
    std::cout << '\n';

    rt = buff.loading_range();
    std::cout << "rect(l,b,r,t) => " <<  rt.left << "," << rt.bottom << "," << rt.right << "," << rt.top << '\n';
    std::cout << "size(rect) => " << rt.size() << '\n';
    std::cout << "size => " << buff.Size() << '\n';

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
