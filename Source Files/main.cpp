#include "../App Framework/Application.h"
#include "../Utils/Debug.h"
#include "../Utils/Profiler.h"
#include "../Utils/Filesystem.h"
#include "../Graphics/OGL/Shader.h"

#include "SceneConsole.h"
#include "SceneGame.h"
#include "SceneTest.h"
#include "SceneMenu.h"

void Application_OnCreate     (Application& sender, void *receiver, bool &res);
void Application_OnInvalidate (Application& sender, void *receiver, bool &res);
void Application_OnDestroy    (Application& sender, void *receiver, bool &res);

int main( int argc, char **argv )
{
	Filesystem::WorkingDirectory = Filesystem::GetParentDir(argv[0]);
	Filesystem::SetSystemWorkingDirectory(Filesystem::WorkingDirectory);

    logEx(0, false, "***********************************");
    logEx(0, true, "Game started");

    Config::Load("Data/config.txt");
    Graphics::OGL::Shader::Load();

    IScene *scene = NULL;
    if (!strcmp(Config::Scene, "test")) scene = new Scenes::SceneTest();
    else
    if (!strcmp(Config::Scene, "game")) scene = new Scenes::SceneGame();
    else                                scene = new Scenes::SceneMenu();
    if (Config::EnableConsole)
    {
        IScene *prev = scene;
        scene = new Scenes::SceneConsole();
        scene->PrevScene = prev;
    }

    Application game;
    game.OnApplicationCreate.Set     ( Application_OnCreate     );
    game.OnApplicationInvalidate.Set ( Application_OnInvalidate );
    game.OnApplicationDestroy.Set    ( Application_OnDestroy    );
#ifndef NDEBUG
    int cres = game.Create("Camera Fighter - Debug", Config::WindowX, Config::WindowY, Config::FullScreen, *scene);
#else
    int cres = game.Create("Camera Fighter", Config::WindowX, Config::WindowY, Config::FullScreen, *scene);
#endif
    if (cres != Application::SUCCESS) return cres;

    int rres = game.Run();
    game.Destroy();

    Graphics::OGL::Shader::Unload();

    logEx(0, true, "Game finished");
    logEx(0, false, "***********************************");

    return rres;
}

#ifdef WIN32

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd)
{
	int argc;
	WCHAR EMPTY_WSTR = 0;
	LPWSTR *argv = ::CommandLineToArgvW(&EMPTY_WSTR, &argc);
	
    int   lenJ = 0;
    char *buff = new char[1000];
    buff[0] = 0;

    for (int i = 0; i < argc; ++i)
    {
	    int lenA = ::WideCharToMultiByte(CP_ACP, 0, argv[i], -1, 0, 0, NULL, NULL);
	    if (lenA)
	    {
		    char *part = new char[lenA];

		    ::WideCharToMultiByte(CP_ACP, 0, argv[i], -1, part, lenA, NULL, NULL);
            memcpy (buff+lenJ, part, lenA);

		    delete[] part;
	    }
        lenJ += lenA;
        buff[lenJ-1] = ' ';
        buff[lenJ]   = 0;
    }
    int res = main(1, &buff);
    delete[] buff;
    return res;
}

#endif

#include "../App Framework/Input/InputMgr.h"
#include "InputCodes.h"
#include "../MotionCapture/CaptureInput.h"
#include "../Multiplayer/NetworkInput.h"
#include "../Graphics/Textures/TextureMgr.h"
#include "../Graphics/FontMgr.h"
#include "../Graphics/OGL/AnimSkeletal.h"
#include "../Models/ModelMgr.h"
#include "../Models/lib3dx/xAnimationMgr.h"

using namespace Graphics::OGL;

void Application_OnCreate(Application& sender, void *receiver, bool &res)
{
    GLExtensions   ::Initialize();
    Profiler       ::CreateS(100);
    StatMgr        ::CreateS();
    g_StatMgr.Add(*new ProfilerPage());
    InputMgr       ::CreateS();
    g_InputMgr.Create(IC_CODE_COUNT);
    g_InputMgr.LoadKeyCodeMap("Data/keys.txt");
    g_InputMgr.LoadMap("Data/keyboard.txt");
    NetworkInput   ::CreateS();
    CaptureInput   ::CreateS();
    FontMgr        ::CreateS();
    TextureMgr     ::CreateS();
    Shader         ::CreateS();
    AnimSkeletal   ::CreateS();
    xAnimationMgr  ::CreateS();
    ModelMgr       ::CreateS();
    res = true;
}

void Application_OnInvalidate(Application& sender, void *receiver, bool &res)
{
    if (FontMgr::GetSingletonPtr())
        g_FontMgr.InvalidateItems();

    if (TextureMgr::GetSingletonPtr())
        g_TextureMgr.InvalidateItems();

    Shader::Invalidate();
    g_InputMgr.AllKeysUp();

    if (xAnimationMgr::GetSingletonPtr())
        g_AnimationMgr.InvalidateItems();

    if (ModelMgr::GetSingletonPtr())
        g_ModelMgr.InvalidateItems();

    res = true;
}

void Application_OnDestroy(Application& sender, void *receiver, bool &res)
{
    ModelMgr       ::DestroyS();
    xAnimationMgr  ::DestroyS();
    AnimSkeletal   ::DestroyS();
    Shader         ::DestroyS();
    TextureMgr     ::DestroyS();
    FontMgr        ::DestroyS();
    CaptureInput   ::DestroyS();
    NetworkInput   ::DestroyS();
    g_InputMgr.SaveMap("Data/keyboard.txt");
    InputMgr       ::DestroyS();
    StatMgr        ::DestroyS();
    Profiler       ::DestroyS();
    res = true;
}
