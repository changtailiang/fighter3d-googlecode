#ifndef __incl_FontMgr_h
#define __incl_FontMgr_h

#include "../Utils/Singleton.h"
#include "../Utils/Manager.h"
#include "OGL/Font.h"

#ifdef WIN32
#   include "D3D/Font.h"
#endif

#define g_FontMgr FontMgr::GetSingleton()

class FontMgr;
typedef Handle <FontMgr> HFont;

class FontMgr : public Singleton<FontMgr>, public Manager<Graphics::OGL::Font, HFont>
{
public:
// Font management.
    HFont GetFont   ( const char* name, int size );

// Font query.
    const std::string& GetName( HFont hfnt ) const
        {  return ( m_HandleMgr.DereferenceNoValidation( hfnt )->Name );  }
    int GetSize( HFont hfnt ) const
        {  return ( m_HandleMgr.DereferenceNoValidation( hfnt )->Size );  }
    Graphics::OGL::Font* GetFont( HFont hfnt )
        {  return ( m_HandleMgr.Dereference( hfnt ) );  }
};

#endif
