#include "xAnimationMgr.h"
#include "../../Utils/Filesystem.h"

HAnimation xAnimationMgr :: GetAnimation    ( const char* name )
{
    // insert/find
    NameIndexInsertRc rc =
        m_NameIndex.insert( std::make_pair( Filesystem::GetFullPath(name), HAnimation() ) );
    if ( rc.second )
    {
        // this is a new insertion
        xAnimationH* ani = m_HandleMgr.Acquire( rc.first->second );
        if ( !ani->Create( rc.first->first.c_str() ) )
        {
            m_HandleMgr.Release( rc.first->second );
            m_NameIndex.erase( rc.first );
            //IncReferences( rc.first->second );
            //DeleteAnimation( rc.first->second );
            assert (false);
            return HAnimation();
        }
    }
    Lock(rc.first->second);

    if (IsHandleValid(rc.first->second))
        return ( rc.first->second );
    else
    {
        assert (false);
        return HAnimation();
    }
}
