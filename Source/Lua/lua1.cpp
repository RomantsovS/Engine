#include    <stdio.h>
#include    <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

#include    "lua.h"
#include    "lauxlib.h"
#include    "lualib.h"

#ifdef  __cplusplus
};
#endif

#include    "tolua++.h"
#include    "ex_6.h"
#include    "ex_6_reg.h"

A :: A ( const std::string& theName ) 
{
    name = theName;
    type = 1;
    x    = 1;
}

double  A :: foo ()
{
    printf ( "A::foo\n" );
    
    return type * x;
}

B :: B ( const std::string& theName ) : A ( theName )
{
    type = 2;
    x    = 1;
}

double  B :: foo ()
{
    printf ( "B::foo\n" );
    
    return type * x + 0.7;
}

int main ()
{
    lua_State * lua = lua_open ();          // create Lua context
    
    if ( lua == NULL )
    {
        printf ( "Error creating Lua context.\n" );
        
        return 1;
    }
    
    A * a = new B ( "Test B object" );
    
    printf ( "type = %d, x = %g, name = \'%s\'\n", a -> type, a -> x, a -> name.c_str () );
    
    luaL_openlibs   ( lua );                                // open standart libraries
    tolua_ex_6_open ( lua );
    
    if ( luaL_loadfile ( lua, "test-7.lua" ) )
        printf ( "Error opening test-7.lua\n" );
        
    lua_pcall             ( lua, 0, LUA_MULTRET, 0 );       
    lua_getfield          ( lua, LUA_GLOBALSINDEX, "foo" ); // push global function f on stack
    tolua_pushusertype    ( lua, (void *)a, "A" );          // push first argument on stack
    lua_pcall             ( lua, 1, 1, 0 );                 // call function taking 1 argsuments and getting one return value
    
    A * ptr = (A *) tolua_tousertype ( lua, -1, NULL );
                                                            // get return value and print it
    printf ( "Returned:\ntype = %d, x = %g, name = \'%s\'\n", ptr -> type, ptr -> x, ptr -> name.c_str () );
        
    lua_close     ( lua );                                  // close Lua context

	getchar();
    
    return 0;
}