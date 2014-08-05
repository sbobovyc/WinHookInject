/*
 * lua_execute.cpp
 *
 *  Created on: Jul 10, 2014
 *      Author: sbobovyc
 */

// How to call function from lua: http://code.google.com/p/zester/wiki/Lua_C

#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include <stdlib.h>
#include <string.h>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#ifdef DLL_LINKED
#include "loadable.hpp"
extern int luaopen_libloadable(lua_State* l);
#endif


static int internal_function_1(lua_State* l) {
    std::cout << "[Exe] " << "Called " << __FUNCTION__ << std::endl;
    lua_pushnumber(l, 10);
    return 1;
}

static int internal_function_2(lua_State* l) {
    std::cout << "[Exe] " << "Called " << __FUNCTION__ << std::endl;
    lua_pushnumber(l, 20);
    return 1;
}

int register_internal_functions(lua_State* l) {
	lua_register(l, "internal_function_1", internal_function_1);
	lua_register(l, "internal_function_2", internal_function_2);
    return 1;
}


void lua_error_handler(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}



int main(void) {
	char buff[256];
	int error;
	lua_State *L = luaL_newstate(); /* opens Lua */
	luaL_openlibs(L); /* Load Lua libraries */
	printf("This is the address of lua state: 0x%p\n", L);
	printf("This is the address of luaL_newstate: 0x%p\n", luaL_newstate);
	printf("This is the address of luaL_loadstring: 0x%p\n", luaL_loadstring);
	printf("This is the address of luaL_loadfilex: 0x%p\n", luaL_loadfilex);
	printf("This is the address of luaL_loadbufferx: 0x%p\n", luaL_loadbufferx);
	printf("This is the address of lua_pcallk: 0x%p\n", lua_pcallk);

	FILE * fstate = fopen("luastate", "w");
	fprintf(fstate, "%i", (int)L);
	fclose(fstate);

	error = luaL_dostring(L, "print(\"[Exe] This is a dostring print\")");
	if(error) {
		lua_error_handler(L,"%s", lua_tostring(L, -1));
	}

	// register functions in exe
	register_internal_functions(L);

	#ifdef DLL_LINKED
	// register functions in dll
	luaopen_libloadable(L);
	error = luaL_dofile(L, "use_loadable.lua");
	if(error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); /* pop error message from the stack */
	}
	#endif

	error = luaL_dofile(L, "use_internal.lua");
	if(error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); /* pop error message from the stack */
	}

	//This function should be executed by the DLL, this is only for testing
	//error = luaL_dofile(L, "dump_global_state.lua");

	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadbuffer(L, buff, strlen(buff),
				"line") || lua_pcall(L, 0, 0, 0);
		if (error) {
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1); /* pop error message from the stack */
		}
	}

	lua_close(L);
	return 0;
}
