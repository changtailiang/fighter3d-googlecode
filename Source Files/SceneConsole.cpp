#include "SceneConsole.h"
#include "SceneGame.h"
#include "SceneMenu.h"
#include "SceneTest.h"

#include "../App Framework/Application.h"
#include "../App Framework/Input/InputMgr.h"
#include "InputCodes.h"
#include "../Graphics/OGL/AnimSkeletal.h"

#include "../Utils/Debug.h"
#include "../Utils/Stat.h"
#include "../Utils/GraphicsModes.h"

using namespace Scenes;

bool SceneConsole :: Create(int left, int top, unsigned int width, unsigned int height, IScene *prevScene)
{
    IScene::Create(left, top, width, height, prevScene);

    font = g_FontMgr.GetFont("Courier New", 12);

    InitInputMgr();

    history      = '>';
    histLines    = 1;
    scroll_v     = 0;

    FL_visible = !PrevScene;

    Performance.Clear();

    return true;
}

void SceneConsole :: InitInputMgr()
{
    InputMgr &im = g_InputMgr;
    im.SetScene(Name);

    im.Key2InputCode_SetIfKeyFree(VK_RETURN, IC_Accept);
    im.Key2InputCode_SetIfKeyFree(VK_ESCAPE, IC_Reject);
    im.Key2InputCode_SetIfKeyFree(VK_F11,    IC_FullScreen);
    im.Key2InputCode_SetIfKeyFree(VK_BACK,   IC_Con_BackSpace);
#ifdef WIN32
    im.Key2InputCode_SetIfKeyFree(VK_OEM_3,  IC_Console);
#else
    im.Key2InputCode_SetIfKeyFree('`',       IC_Console);
#endif
    im.Key2InputCode_SetIfKeyFree(VK_UP,    IC_Con_LineUp);
    im.Key2InputCode_SetIfKeyFree(VK_DOWN,  IC_Con_LineDown);
    im.Key2InputCode_SetIfKeyFree(VK_PRIOR, IC_Con_PageUp);
    im.Key2InputCode_SetIfKeyFree(VK_NEXT,  IC_Con_PageDown);
    im.Key2InputCode_SetIfKeyFree(VK_HOME,  IC_Con_FirstPage);
    im.Key2InputCode_SetIfKeyFree(VK_END,   IC_Con_LastPage);

    im.Key2InputCode_SetIfKeyFree(VK_LEFT,  IC_Con_StatPrevPage);
    im.Key2InputCode_SetIfKeyFree(VK_RIGHT, IC_Con_StatNextPage);

    KeyNextPage = "[" + g_InputMgr.GetKeyName(g_InputMgr.Input2KeyCode(IC_Con_StatPrevPage)) + "] or [" +
        g_InputMgr.GetKeyName(g_InputMgr.Input2KeyCode(IC_Con_StatNextPage)) + "]";

    im.SetScene(PrevScene->Name);
#ifdef WIN32
    im.Key2InputCode_SetIfKeyFree(VK_OEM_3,  IC_Console);
#else
    im.Key2InputCode_SetIfKeyFree('`',       IC_Console);
#endif
}

void SceneConsole :: Destroy()
{
	IScene::Destroy();
    g_FontMgr.Release(font);
    font = HFont();
}

void SceneConsole :: Enter()
{
    IScene::Enter();

    FL_justOpened = true;
    g_InputMgr.Buffer.clear();
    g_InputMgr.mouseWheel = 0;

    if (FL_visible)
    {
        FL_overlayInput = false;
        FL_overlayClock = false;
    }
    else
    {
        FL_overlayInput = true;
        FL_overlayClock = true;
        PrevScene->Enter();
    }
}

void SceneConsole :: Exit()
{
    if (PrevScene) PrevScene->Exit();
}

void SceneConsole :: Resize(int left, int top, unsigned int width, unsigned int height)
{
	//if (PrevScene) PrevScene->Resize(left, top, width, height);
	IScene::Resize(left, top, width, height);

	const Graphics::OGL::Font* pFont = g_FontMgr.GetFont(font);
	pageSize = (int)(height/2.0f/pFont->LineH()) - 3;
	if (scroll_v > histLines-1-pageSize) scroll_v = histLines-1-pageSize;
}

IScene & SceneConsole :: Scene_Set(IScene& scene, bool fl_destroyPrevious)
{
    if (PrevScene && fl_destroyPrevious)
     {
        if (!scene.IsDestroyed() || scene.Create(0, 0, Width, Height))
        {
            PrevScene->Exit();
            PrevScene->Destroy();
            delete PrevScene;
            PrevScene = &scene;
            scene.Enter();
        }
     }
     else
     if (!scene.IsDestroyed() || scene.Create(0, 0, Width, Height, PrevScene))
     {
         if (PrevScene) PrevScene->Exit();
         PrevScene = &scene;
         scene.Enter();
     }

    if (PrevScene)
    {
        g_InputMgr.SetScene(PrevScene->Name);
#ifdef WIN32
        g_InputMgr.Key2InputCode_SetIfKeyFree(VK_OEM_3, IC_Console);
#else
        g_InputMgr.Key2InputCode_SetIfKeyFree('`',      IC_Console);
#endif
    }

    return *this;
}


void SceneConsole :: AppendConsole(std::string text)
{
    history += text;

    const char *start = text.c_str();
    const char *end;

    while ((end = strchr(start, '\n')))
    {
        ++histLines;
        start = end+1;
    }
    if (scroll_v < histLines -1-pageSize) scroll_v = histLines-1-pageSize;
}

bool SceneConsole :: Update(float T_delta)
{
    Performance.Update(T_delta);

    T_carretTick += T_delta;
    while (T_carretTick > 0.5f)
    {
        T_carretTick -= 0.5f;
        FL_carretVisible = !FL_carretVisible;
    }

    InputMgr &im = g_InputMgr;

    if (im.InputDown_GetAndRaise(IC_Console))
    {
        if (FL_visible && !FL_overlayInput && PrevScene)
        {
            FL_overlayInput = true;
            FL_overlayClock = true;
            PrevScene->Enter();
        }
        else
        {
            FL_visible = true;
            Enter();
        }
        return true;
    }
    else
    if (im.InputDown_GetAndRaise(IC_Reject))
    {
        if (PrevScene)
        {
            if (FL_visible)
            {
                FL_visible = false;
                PrevScene->Enter();
            }
            else
            {
                im.InputDown_Set(IC_Reject, true);
                return PrevScene->Update(0.f);
            }
        }
        else
            g_Application.Destroy();
        return true;
    }

    if (!FL_visible)
    {
        im.SetScene(PrevScene->Name);
        PrevScene->Update(T_delta);
        return true;
    }

    if (FL_overlayClock)
    {
        im.SetScene(PrevScene->Name, !FL_overlayInput);
        bool res = PrevScene->Update(T_delta);
        if (FL_overlayInput)
            return res;
        im.SetScene(Name, false);
    }

    im.SetScene(Name);

    if (im.InputDown_GetAndRaise(IC_Con_StatPrevPage))
    {
        --curStatPage;
        if (curStatPage < -1)
            curStatPage = g_StatMgr.pages.size()-1;
    }
    if (im.InputDown_GetAndRaise(IC_Con_StatNextPage))
    {
        ++curStatPage;
        if (curStatPage >= (int)g_StatMgr.pages.size())
            curStatPage = -1;
    }

    if (im.InputDown_GetAndRaise(IC_FullScreen))
    {
        if (g_Application.MainWindow_Get().IsFullScreen())
            g_Application.MainWindow_Get().FullScreen_Set(Config::WindowX, Config::WindowY, false);
        else
            g_Application.MainWindow_Get().FullScreen_Set(Config::FullScreenX, Config::FullScreenY, true);
        return true;
    }
    else
    if (im.InputDown_GetAndRaise(IC_Accept))
    {
        AppendConsole(currCmd);
        std::string output;
        bool result = ShellCommand(currCmd, output);
        if (PrevScene) result |= PrevScene->ShellCommand(currCmd, output);
        if (!result)
            AppendConsole("\nUnknown command: " + currCmd);
        else
            AppendConsole(output);

        if (history.size())
            AppendConsole("\n>");
        else
            AppendConsole(">");

        currCmd.clear();
    }
    else
    if (im.InputDown_Get(IC_Con_LineUp) || im.mouseWheel > 0)
    {
        if (im.mouseWheel > 100) im.mouseWheel -= 100;
        else                     im.mouseWheel = 0;
        if (scroll_v) --scroll_v;
        else          im.mouseWheel = 0;
    }
    else
    if (im.InputDown_Get(IC_Con_LineDown) || im.mouseWheel < 0)
    {
        if (im.mouseWheel < -100) im.mouseWheel += 100;
        else                      im.mouseWheel = 0;
        if (scroll_v < histLines -1 -pageSize) ++scroll_v;
        else                                   im.mouseWheel = 0;
    }
    else
    if (im.InputDown_GetAndRaise(IC_Con_PageUp))
    {
        if (scroll_v > pageSize) scroll_v -= pageSize;
        else                     scroll_v = 0;
    }
    else
    if (im.InputDown_GetAndRaise(IC_Con_PageDown))
    {
        if (scroll_v < histLines -1 -2*pageSize) scroll_v += pageSize;
        else                                     scroll_v = histLines-1-pageSize;
    }
    else
    if (im.InputDown_GetAndRaise(IC_Con_FirstPage))
        scroll_v = 0;
    else
    if (im.InputDown_GetAndRaise(IC_Con_LastPage))
        scroll_v = histLines-1-pageSize;
    else if (im.InputDown_GetAndRaise(IC_Con_BackSpace))
    {
        if (currCmd.length())
            currCmd.erase(currCmd.end()-1);
    }
    else if (g_InputMgr.Buffer.length())
    {
        if (FL_justOpened) // skip the key that has opened the console
            FL_justOpened = false;
        else
            currCmd += g_InputMgr.Buffer;
        g_InputMgr.Buffer.clear();
        if (scroll_v < histLines -1-pageSize) scroll_v = histLines-1-pageSize;
        if (scroll_v < 0) scroll_v = 0;
    }

    return true;
}

bool SceneConsole :: ShellCommand(std::string &cmd, std::string &output)
{
    if (cmd == "?" || cmd == "help")
    {
        output.append("\n\
  Available shell comands for [console]:\n\
    Full command        | Short command | Description\n\
    ------------------------------------------------------------------------\n\
    help                | ?             | print this help screen\n\
    clear_console       | clr           | clear console history\n\
    zero_fps_counters   | clrfps        | clears min & max fps counters\n\
    terminate           | qqq           | terminate application\n\
    `                   | `             | enable full overlay mode\n\
    toggle_clock        | tcl           | toggles the clock overlay mode\n\
    ------------------------------------------------------------------------\n\
    graphical_mode_list | gml           | list possible pixel formats\n\
    opengl_extensions   | ext           | list available OpenGL extensions\n\
    status              | status        | show execution status\n\
    ------------------------------------------------------------------------\n\
    log message         | log message   | adds given message to the log file\n\
    log_tail            | tail          | displays tail of the log file\n\
    log_read            | read          | displays the log file\n\
    log_clear           | clrlog        | remove the log file\n\
    ------------------------------------------------------------------------\n\
    ");
        return true;
    }
    if (cmd == "clr" || cmd == "clear_console")
    {
        history.clear();
        histLines = 1;
        scroll_v = 0;
        return true;
    }
    if (cmd == "qqq" || cmd == "terminate")
    {
        g_Application.Destroy();
        return true;
    }
    if (cmd == "tcl" || cmd == "toggle_clock")
    {
        FL_overlayClock = !FL_overlayClock;
        if (FL_overlayClock)
            output.append("\nThe clock is ON.\n");
        else
            output.append("\nThe clock is OFF.\n");
        return true;
    }
    if (cmd == "gml" || cmd == "graphical_mode_list")
    {
#ifdef WIN32
        GraphicsModes gm(g_Application.MainWindow_Get().HDC());

        int gm_count = gm.CountModes();
        PIXELFORMATDESCRIPTOR* pfds = gm.ListModes();

        for (int i=0; i<gm_count; ++i)
        {
            output.append("\n  Pixel Format " + itos(i+1) + '\n');
            output.append(gm.ModeToString(pfds[i]));
        }

        delete[] pfds;
#else
        output.append("\nImplemented only for Windows\n");
#endif
        return true;
    }
    if (cmd == "ext" || cmd == "opengl_extensions")
    {
        char *extensions = strdup((char *) glGetString(GL_EXTENSIONS)); // Fetch Extension String
        int len=strlen(extensions);
        for (int i=0; i<len; i++)              // Separate It By Newline Instead Of Blank
            if (extensions[i]==' ') extensions[i]='\n';
        output.append("\nAvailable OpenGL extensions:\n");
        output.append(extensions);
        delete[] extensions;

        return true;
    }
    if (cmd == "status")
    {
    std::stringstream ss;
    std::string txt = "\n\
speed = *", txt2;
    ss << Config::Speed; ss >> txt2; txt += txt2 + "\n\
test  = ";
    ss.clear();
    ss << Config::TestCase;  ss >> txt2; txt += txt2 + '\n';
    AppendConsole(txt);
    if (Graphics::OGL::g_AnimSkeletal.HardwareEnabled())
        output.append("hardware skeletal animation ENABLED\n");
    else
        output.append("hardware skeletal animation DISABLED\n");
    return true;
    }
    if (cmd == "clrfps" || cmd == "zero_fps_counters")
    {
        Performance.FPSmin = 1000.f;
        Performance.FPSmax = 0;
        return true;
    }
    if (cmd.substr(0, 4) == "log ")
    {
        logEx(0, true, cmd.substr(4).c_str());
        return true;
    }
    if (cmd == "tail" || cmd == "log_tail")
    {
        char *res = log_tail();
        output.append("\nLog tail:\n");
        output.append(res);
        return true;
    }
    if (cmd == "read" || cmd == "log_read")
    {
        char *res = log_read();
        output.append("\nLog file:\n");
        output.append(res);
        return true;
    }
    if (cmd == "clrlog" || cmd == "log_clear")
    {
        log_clear();
        return true;
    }
    if (cmd.substr(0, 6) == "scene ")
    {
        IScene *newScene = NULL;
        if (cmd == "scene test") newScene = new SceneTest();
        else
        if (cmd == "scene game") newScene = new SceneGame();
        else
        if (cmd == "scene menu") newScene = new SceneMenu();
        if (!newScene) return true;
        if (PrevScene)
        {
            PrevScene->Destroy();
            delete PrevScene;
            PrevScene = NULL;
        }
        PrevScene = newScene;
        PrevScene->Create(Left, Top, Width, Height);
        if (FL_visible)
        {
            FL_overlayClock = false;
            FL_overlayInput = false;
            g_InputMgr.SetScene(Name);
        }
        return true;
    }
    return false;
}

bool SceneConsole::Render()
{
    if (PrevScene) PrevScene->Render();
    if (!FL_visible) return true;

    Profile("Render console");

    GLint cHeight = Height/2;

    glDisable(GL_DEPTH_TEST);                      // Disable depth testing
    Graphics::OGL::Shader::SetLightType(xLight_NONE);
    Graphics::OGL::Shader::EnableTexturing(Graphics::OGL::xState_Off);
    glDisable (GL_POLYGON_SMOOTH);

    const Graphics::OGL::Font* pFont = g_FontMgr.GetFont(font);
    float lineHeight = pFont->LineH();

    {
        Profile("Shell");

        // Set projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, Width, 0, cHeight, 0, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glViewport(Left, Top+cHeight, Width, cHeight); // Set viewport

        // Draw background
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable (GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4ub( 0, 0, 0, 128 );
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f((GLfloat)Width, 0.0f);
            glVertex2f((GLfloat)Width, (GLfloat)cHeight);
            glVertex2f(0.0f, (GLfloat)cHeight);
        glEnd();
        // Draw scrollbar
        glColor4ub( 255, 255, 255, 128 );
        float size = (float)(pageSize+1) / histLines;
        if (size < 1)
        {
            size *= cHeight-2*lineHeight;

            float position = (float)(scroll_v) / histLines;
            position = (cHeight-2*lineHeight)*(1-position);

            glBegin(GL_QUADS);
                glVertex2f((GLfloat)Width-10.0f, 0.0f);
                glVertex2f((GLfloat)Width, 0.0f);
                glVertex2f((GLfloat)Width, (GLfloat)cHeight-2*lineHeight);
                glVertex2f(Width-10.0f, (GLfloat)cHeight-2*lineHeight);

                glVertex2f((GLfloat)Width-10.0f, position-size);
                glVertex2f((GLfloat)Width, position-size);
                glVertex2f((GLfloat)Width, position);
                glVertex2f(Width-10.0f, position);
            glEnd();
        }
        glDisable(GL_BLEND);

        glColor4ub( 255, 255, 255, 255 );
        pFont->PrintF(0.0f, (float)cHeight-lineHeight, 0.0f,
            "Console    MinFPS: %u MeanFPS: %2u MaxFPS: %u T_world: %4.3f",
            (int)Performance.FPSmin, (int)Performance.FPSsnap, (int)Performance.FPSmax,
            Performance.T_world);
        pFont->PrintF(0.0f, (float)cHeight-2*lineHeight, 0.0f,
            "   Num culled elements: %3u diffuse: %u shadows: %u culled: %u zP: %u zF: %u zFs: %u zFf: %u zFb: %u",
            (int)Performance.CulledElements, Performance.CulledDiffuseElements,
            Performance.Shadows.shadows, Performance.Shadows.culled, Performance.Shadows.zPass, Performance.Shadows.zFail,
            Performance.Shadows.zFailS, Performance.Shadows.zFailF, Performance.Shadows.zFailB);

	    glScissor(0, cHeight, Width, cHeight);                 // Define Scissor Region
        glEnable(GL_SCISSOR_TEST);                             // Enable Scissor Testing

        pFont->Print(0.0f, cHeight-3*lineHeight, 0.0f, cHeight-3*lineHeight, scroll_v, history.c_str());
        pFont->Print(currCmd.c_str());
        if (FL_carretVisible && scroll_v >= histLines -1-pageSize)
            pFont->Print("_");

        glDisable(GL_SCISSOR_TEST);
    }
    {
        Profile("Stats");

        glViewport(Left, Top, Width, cHeight); // Set viewport

        if (curStatPage >= 0 && curStatPage >= (int)g_StatMgr.pages.size())
            curStatPage = g_StatMgr.pages.size() - 1;
        if (curStatPage >= 0)
        {
            StatPage_Base &page = *g_StatMgr.pages[curStatPage];

            // Draw background
            glEnable (GL_BLEND);
            glColor4ub( 0, 0, 0, 128 );
            glBegin(GL_QUADS);
                glVertex2f(0.0f, 0.0f);
                glVertex2f((GLfloat)Width, 0.0f);
                glVertex2f((GLfloat)Width, (GLfloat)cHeight);
                glVertex2f(0.0f, (GLfloat)cHeight);
            glEnd();
            glDisable(GL_BLEND);

            glColor4ub( 255, 255, 255, 255 );

            glBegin(GL_LINES);
            {
                glVertex2f(0.0f, (GLfloat)cHeight-1);
                glVertex2f((GLfloat)Width, (GLfloat)cHeight-1);
            }
            glEnd();

            pFont->PrintF(0.0f, cHeight-lineHeight, 0.0f, "Page %d/%d - %s - Press %s to change",
                curStatPage+1, g_StatMgr.pages.size(), page.Name.c_str(), KeyNextPage.c_str());

            const Vec_string &lines = page.GetLines();
            Vec_string::const_iterator LN_curr = lines.begin(),
                                       LN_last = lines.end();
            xFLOAT x = 0.f;
            xFLOAT y = cHeight-lineHeight*2;
            xFLOAT w = 0.f;

            for (; LN_curr != LN_last; ++LN_curr)
            {
                const char *text = LN_curr->c_str();
                w = max( w, pFont->Length(text) );

                glRasterPos2f(x, y);
                pFont->Print(text);
                y -= lineHeight;
                if (y < 0.f)
                {
                    x += w + 20.f;
                    glBegin(GL_LINES);
                    {
                        glVertex2f(x - 10.f, y+lineHeight);
                        glVertex2f(x - 10.f, cHeight-lineHeight*1.5f);
                    }
                    glEnd();

                    w = 0.f;
                    y = cHeight-lineHeight*2.f;
                }
            }
        }
    }

    // Flush the buffer to force drawing of all objects thus far
    glFlush();

    return true;
}
