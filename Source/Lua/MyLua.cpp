#include "MyLua.h"

Lua::Lua(const char *theName) : Object(theName)
{
	luaState = lua_open ();

	 luaL_openlibs   ( luaState );
}

Lua::~Lua()
{
	lua_close     ( luaState );
}

bool Lua::isOk() const
{
	return luaState != NULL;
}

void Lua::getGlobal(const char *name)
{
	lua_getglobal(luaState, name);
}

int Lua::getGlobalParametr(const char *name)
{
	getGlobal(name);
	return (int)lua_tonumber(luaState, -1);
}

bool Lua::doFile(const char *filename)
{
	if(luaL_dofile(luaState, filename))
		return false;
	return true;
}