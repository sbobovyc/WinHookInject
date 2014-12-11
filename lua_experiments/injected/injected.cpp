/*
 * injected.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: sbobovyc
 */

#include <windows.h>
#include <string.h>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

DWORD g_threadID;
HINSTANCE g_hModule;
DWORD WINAPI MyThread(LPVOID);

static void stackDump (lua_State *L) {
  int i;
  int top = lua_gettop(L);
  for (i = 1; i <= top; i++) {  /* repeat for each level */
    int t = lua_type(L, i);
    switch (t) {

      case LUA_TSTRING:  /* strings */
        printf("`%s'", lua_tostring(L, i));
        break;

      case LUA_TBOOLEAN:  /* booleans */
        printf(lua_toboolean(L, i) ? "true" : "false");
        break;

      case LUA_TNUMBER:  /* numbers */
        printf("%g", lua_tonumber(L, i));
        break;

      default:  /* other values */
        printf("%s", lua_typename(L, t));
        break;

    }
    printf("  ");  /* put a separator */
  }
  printf("\n");  /* end the listing */
}

BOOL WINAPI DllMain (HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            // Code to run when the DLL is loaded
            g_hModule = hDll;
    		DisableThreadLibraryCalls(hDll);
            CreateThread(NULL, NULL, &MyThread, NULL, NULL, &g_threadID);
            //MyThread(0);
            break;

        case DLL_PROCESS_DETACH:
            // Code to run when the DLL is freed
            break;

        case DLL_THREAD_ATTACH:
            // Code to run when a thread is created during the DLL's lifetime
            break;

        case DLL_THREAD_DETACH:
            // Code to run when a thread ends normally.
            break;
    }
    return TRUE;
}

DWORD WINAPI MyThread(LPVOID)
{
	lua_State *L;
	OutputDebugStr("This is in the injected dll");
	FILE * fstate = fopen("luastate", "r");
	char filebuff[255];
		 /* get a line, up to 80 chars from fr.  done if NULL */
	if(fgets(filebuff, 80, fstate) != NULL) {
		// hook luaL_newstate
		 L = (lua_State *)atoi(filebuff);
	} else {
		OutputDebugStr("Failed to read file");
		FreeLibraryAndExitThread(g_hModule, 0);
		return 0;
	}



    char buff[] = "print(\"[Injected Dll] This is from the injected dll!\")";
    int error = luaL_dostring(L, buff);
	if(error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); /* pop error message from the stack */
	}
	error = luaL_dofile(L, "C:\\Users\\sbobovyc\\workspace\\WinHookInject\\lua_experiments\\lua_simple_app\\dump_global_state.lua");
	if(error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); /* pop error message from the stack */
	}
    OutputDebugStr("This is after lua call");

    while(1);
	FreeLibraryAndExitThread(g_hModule, 0);
	return 0;

}
