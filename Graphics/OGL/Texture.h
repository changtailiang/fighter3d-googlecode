#ifndef __incl_Graphics_OGL_Texture_h
#define __incl_Graphics_OGL_Texture_h

#include <string>
#include "../../Utils/Resource.h"
#include "ogl.h"

namespace Graphics { namespace OGL {
    
    struct Texture : public Resource
    {
        unsigned int ID_GLTexture; // GL Texture identifier

    public:
        std::string  Name;         // for reconstruction
        bool         FL_MipMap;    // generate mip maps?
        unsigned int Width;        // width
        unsigned int Height;       // height

        Texture() { Clear(); }

        virtual void Clear() {
            Resource::Clear();
            Name.clear();
            ID_GLTexture = 0;
        }

        virtual bool Create();
        virtual bool Create( const std::string& name, bool fl_mipmap )
        {
            Name      = name;
            FL_MipMap = fl_mipmap;
            return Create();
        }

        virtual void Dispose();
        virtual void Invalidate()
        { ID_GLTexture = 0; }
        virtual bool IsDisposed()
        { return ID_GLTexture == 0; }

        void Bind()
        { glBindTexture(GL_TEXTURE_2D, ID_GLTexture ); }
        
        virtual const std::string &Identifier() { return Name; }
    };
    
} } // namespace Graphics::OGL

#endif
