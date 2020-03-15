#ifndef GMSV_MONGODB_UTIL_HPP
#define GMSV_MONGODB_UTIL_HPP

#include "MongoDB.hpp"

int BSONToLua(GarrysMod::Lua::ILuaBase* LUA, const bson_t* bson);

const char* LuaToJSON(GarrysMod::Lua::ILuaBase* LUA, int ref);

bson_t* LuaToBSON(GarrysMod::Lua::ILuaBase* LUA, int ref);

#endif //GMSV_MONGODB_UTIL_HPP
