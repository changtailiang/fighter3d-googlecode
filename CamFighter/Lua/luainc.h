
#ifndef __LUA_INC_H__
#define __LUA_INC_H__

extern "C"
{
    #ifdef WIN32
       #include <lua.h>
       #include <lauxlib.h>
       #include <lualib.h>
    #else
       #include <lua5.1/lua.h>
       #include <lua5.1/lauxlib.h>
       #include <lua5.1/lualib.h>
    #endif
}

#endif // __LUA_INC_H__
