#include "AppDelegate.h"
#include "MenuScene.h"
//#include "GameScene.h"
//#include "RoleScene.h"

USING_NS_CC;

/*
static Size designResolutionSize = cocos2d::Size(960, 640);
static Size smallResolutionSize = cocos2d::Size(480, 320);
static Size mediumResolutionSize = cocos2d::Size(1024, 768);
static Size largeResolutionSize = cocos2d::Size(2048, 1536); // */

Director *      g_director;
FileUtils *     g_file;
Size            g_win_size;
Size            g_vis_size;
//Settings
//char            g_server_ip[32] = "127.0.0.1";
char            g_server_ip[32] = "192.168.191.3";
int             g_volume = 50;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    g_director = Director::getInstance();
    g_file = FileUtils::getInstance();

    auto glview = g_director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Game");
        g_director->setOpenGLView(glview);
    }

	g_vis_size = g_director->getVisibleSize();
    g_win_size = g_director->getWinSize();

    // turn on display FPS
    g_director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    g_director->setAnimationInterval(1.0 / 60);

    auto screenSize = glview->getFrameSize();
    auto designSize = Size(960, 640);

    if (screenSize.height > 320)
    {
        auto resourceSize = Size(960, 640);
        g_director->setContentScaleFactor(resourceSize.height/designSize.height);
    }

    // Set the design resolution
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);


    /*
    Size frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {
    g_director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {
    g_director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {
    g_director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    } // */
    {
        g_file->addSearchPath("scene/dialog_scene");
        g_file->addSearchPath("scene/menu_scene");
        g_file->addSearchPath("scene/room_scene");
        g_file->addSearchPath("scene/room_list_scene");
        g_file->addSearchPath("scene/role_scene");
        g_file->addSearchPath("scene/setting_scene");
        g_file->addSearchPath("scene/game_scene");

        g_file->addSearchPath("Particle3D/materials");
        g_file->addSearchPath("Particle3D/scripts");
        g_file->addSearchPath("Particle3D/textures");

        g_file->addSearchPath("character");
    }

    // Enable Remote Console
    auto console = g_director->getConsole();
    console->listenOnTCP(5678);

    // create a scene. it's an autorelease object
    auto scene = MenuScene::createScene();
    //auto scene = GameScene::createScene();
    //auto scene = RoleScene::createScene();
    // run
    g_director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
