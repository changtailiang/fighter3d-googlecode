#include "ModelMgr.h"
#include "../Utils/Filesystem.h"

HModel ModelMgr :: GetModel( const char* name )
{
    // insert/find
    NameIndexInsertRc rc =
		m_NameIndex.insert( std::make_pair( Filesystem::GetFullPath(name), HModel() ) );
    if ( rc.second )
    {
        // this is a new insertion
        Model3dx* mod = m_HandleMgr.Acquire( rc.first->second );
        if ( !mod->Create( rc.first->first.c_str() ) )
        {
            m_HandleMgr.Release( rc.first->second );
            m_NameIndex.erase( rc.first );
            //IncReferences( rc.first->second );
            //DeleteModel( rc.first->second );
            assert (false);
            return HModel();
        }
    }
    Lock(rc.first->second);

    if (IsHandleValid(rc.first->second))
        return ( rc.first->second );
    else
    {
        assert (false);
        return HModel();
    }
}
