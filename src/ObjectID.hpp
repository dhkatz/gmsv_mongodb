#ifndef GMSV_MONGODB_OBJECTID_HPP
#define GMSV_MONGODB_OBJECTID_HPP

#include "MongoDB.hpp"

int new_objectid(lua_State* L);

int objectid_tostring(lua_State* L);

int objectid_eq(lua_State* L);

int objectid_data(lua_State* L);

int objectid_hash(lua_State* L);

extern int ObjectIDMetaTableId;

#endif //GMSV_MONGODB_OBJECTID_HPP
