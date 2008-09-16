#include "TextureMgr.h"
#include "../../Utils/Filesystem.h"

HTexture TextureMgr :: GetTexture( const char* name )
{
    // insert/find
    NameIndexInsertRc rc =
        m_NameIndex.insert( std::make_pair( Filesystem::GetFullPath(name), HTexture() ) );
    if ( rc.second )
    {
        // this is a new insertion
        Graphics::OGL::Texture* tex = m_HandleMgr.Acquire( rc.first->second );
        if ( !tex->Create( rc.first->first, true ) )
        {
            m_HandleMgr.Release( rc.first->second );
            m_NameIndex.erase( rc.first );
            //assert (false);
            return HTexture();
        }
    }
    Lock(rc.first->second);

    if (IsHandleValid(rc.first->second))
        return ( rc.first->second );
    else
    {
        assert (false);
        return HTexture();
    }
}
