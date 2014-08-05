/*
 * loadable.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sbobovyc
 *  http://www.acamara.es/blog/2012/08/creating-a-lua-5-2-dynamic-link-library-of-c-functions/
 */

#include <iostream>

#include "loadable.hpp"

/*
 * Library functions
 */



static int funval = 10;

static int function_1(lua_State* l)
{
    std::cout << "[DLL] Function 1" << std::endl;
    std::cout << "[DLL] It's value is: " << funval << std::endl;
    lua_pushnumber(l, funval);
    return 1;
}

static int function_2(lua_State* l)
{
    std::cout << "[DLL] Function 2" << std::endl;
    std::cout << "[DLL] It's value is: " << 2*funval << std::endl;
    lua_pushnumber(l, 2*funval);
    return 1;
}

/*
 * Registering functions
 */

static const struct luaL_Reg cfunctions[] = {
    {"fun1", function_1},
    {"fun2", function_2},
    {NULL, NULL}
};

int luaopen_libloadable(lua_State* l)
{
    luaL_newlibtable(l, cfunctions);
    luaL_setfuncs(l, cfunctions, 0);
    return 1;
}



