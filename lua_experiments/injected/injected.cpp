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

uintptr_t luaL_newstate_return = 0x00405CBE;
uintptr_t luaL_newstate_func = 0x00405C90;



#define JMP 0xE9
#define NOP 0x90

void  myLuaNewstateReturnHook(void) {

}

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

void returnHook(void * returnAddress, void * hookAddress) {
	DWORD oldProtection;
	VirtualProtect(returnAddress, 6, PAGE_EXECUTE_READWRITE, &oldProtection);
	DWORD JMPSize = ((DWORD)hookAddress - (DWORD)returnAddress - 5);
	BYTE JMP_hook[6] = {JMP, NOP, NOP, NOP, NOP, NOP};
	memcpy(&JMP_hook[1], &JMPSize, 4);
	//wsprintf(buf, "JMP: %x%x%x%x%x", JMP[0], JMP[1], JMP[2], JMP[3], JMP[4], JMP[5]);
	//OutputDebugString(debugStrBuf);
	// overwrite retn with JMP_hook
	memcpy((void *)returnAddress, (void *)JMP_hook, 6);
	// restore protection
	VirtualProtect((LPVOID)returnAddress, 6, oldProtection, NULL);
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
	OutputDebugStr("This is in the injected dll");
	// hook luaL_newstate
	lua_State *L = (lua_State *)0x3715B0;
    char buff[] = "print(\"[Injected Dll] This is from the injected dll!\")";
    int error = luaL_dostring(L, buff);
	//int status = loadfile(L, "C:\\Users\\sbobovyc\\workspace\\lua_testing\\Debug\\filetest.lua", NULL) || lua_pcallk(L, 0, LUA_MULTRET, 0, 0, NULL);
    //int status = luaL_loadbuffer(L, buff, strlen(buff), "line", "r") || lua_pcall(L, 0, 0, 0);
    //int status = luaL_dofile(L, "C:\\Users\\sbobovyc\\workspace\\WinHookInject\\lua_experiments\\lua_simple_app\\dump_global_state.lua");
	if(error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); /* pop error message from the stack */
	}
	error = luaL_dofile(L, "C:\\Users\\sbobovyc\\workspace\\WinHookInject\\lua_experiments\\lua_simple_app\\print_test.lua");
	error = luaL_dofile(L, "C:\\Users\\sbobovyc\\workspace\\WinHookInject\\lua_experiments\\lua_simple_app\\dump_global_state.lua");
	if(error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); /* pop error message from the stack */
	}
    OutputDebugStr("This is after lua call");

	//returnHook((void *)luaL_newstate_return, (void *)myLuaNewstateReturnHook);
    while(1);
	FreeLibraryAndExitThread(g_hModule, 0);
	return 0;

}
