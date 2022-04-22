#ifndef __LUA__
#define __LUA__

#include "../Foundation/Object.h"


#ifdef  __cplusplus
extern "C" {
#endif

#include    "lua.h"
#include    "lauxlib.h"
#include    "lualib.h"

#ifdef  __cplusplus
};
#endif

//#include    "tolua++.h"

class Lua : public Object
{
public:
	Lua(const char *theName);
	~Lua();
	virtual	bool	isOk () const;
	bool loadFile(const char *filename);
	bool doFile(const char *filename);
	void getGlobal(const char *name);
	int getGlobalParametr(const char *name);
private:
	lua_State * luaState;
};


#endif