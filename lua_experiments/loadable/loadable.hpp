/*
 * loadable.hpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sbobovyc
 */

#ifndef LOADABLE_HPP_
#define LOADABLE_HPP_

#include <lua.hpp>

extern "C"
{
int __declspec(dllexport) luaopen_libloadable(lua_State* l);
} // end extern "C"

#endif /* LOADABLE_HPP_ */
