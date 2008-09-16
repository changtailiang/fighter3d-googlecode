#ifndef __incl_Graphics_OGL_Button_h
#define __incl_Graphics_OGL_Button_h

#include "Utils.h"
#include "Font.h"

namespace Graphics { namespace OGL {
    using namespace Graphics::OGL;
    
    class Button
    {
        bool   mOver;
        
      public:
        xFLOAT X,Y,X2,Y2;
        
        int    Action;
        char  *Text;
        bool   RadioBox;
        bool   Down;
        xColor Background;
        
        Button(const Button &button)
        {
            X  = button.X; X2 = button.X2;
            Y  = button.Y; Y2 = button.Y2;
            Action   = button.Action;
            mOver    = button.mOver;
            RadioBox = button.RadioBox;
            Down     = button.Down;
            Text     = strdup(button.Text);
            memcpy(&Background, &button.Background, sizeof(xColor));
        }
        Button(const char *text, xFLOAT x, xFLOAT y, xFLOAT w, xFLOAT h, int action, bool radioBox = false, bool down = false)
            : mOver(false), X(x), Y(y), X2(x+w), Y2(y+h), Action(action), RadioBox(radioBox), Down(down)
        {
            Text = strdup(text);
            Background.r = Background.g = Background.b = 0.3f; Background.a = 1.0f;
        }
        Button(const char *text, xFLOAT x, xFLOAT y, const Font* pFont, int action, bool radioBox = false, bool down = false)
            : mOver(false), X(x), Y(y)
            , X2(x + pFont->Length(text) + 6)
            , Y2(y + pFont->LineH())
            , Action(action), RadioBox(radioBox), Down(down)
        {
            Text = strdup(text);
            Background.r = Background.g = Background.b = 0.3f; Background.a = 1.0f;
        }
        ~Button()
        {
            if (Text) delete[] Text;
            Text = NULL;
        }

        bool HitTest(xFLOAT x, xFLOAT y)
        {
            return (x >= X && y >= Y && x <= X2 && y <= Y2);
        }
        bool Hover(xFLOAT x, xFLOAT y)
        {
            return mOver = HitTest(x, y);
        }
        bool Click(xFLOAT x, xFLOAT y)
        {
            mOver = false;
            return HitTest(x, y);        
        }

        void Render(const Font* pFont)
        {
            if (!Text) return;

            int fHeight = pFont->Size;

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            if (Down)
            {
                if (RadioBox)
                    glColor4f( 0.7f, 0.0f, 0.0f, 1.f );
                else
                    glColor4f( 0.0f, 0.7f, 0.0f, 1.f );
            }
            else
            if (mOver)
                glColor4f( 0.7f, 0.7f, 0.0f, 1.f );
            else
                glColor4fv( Background.col );

            glRectf(X, Y, X2, Y2);

            if (mOver)
                glColor4f( 0.f, 0.f, 0.f, 1.f );
            else
                glColor4f( 1.f, 1.f, 1.f, 1.f );
            pFont->Print(X+3.f, Y2-(Y2-Y-fHeight*0.5f)*0.5f, 0.f, Text);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glRectf(X, Y, X2, Y2);
        }
    };
    
} } // namespace Graphics::OGL

#endif
