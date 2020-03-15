#ifndef GMSV_MONGODB_DATABASE_HPP
#define GMSV_MONGODB_DATABASE_HPP

#include "MongoDB.hpp"

int destroy_database(lua_State* state);

int database_command(lua_State* state);

int database_copy(lua_State* state);

int database_drop(lua_State* state);

int database_name(lua_State* state);

int database_user_add(lua_State* state);

int database_user_remove(lua_State* state);

int database_collection_create(lua_State* state);

int database_collection_get(lua_State* state);

int database_collection_exists(lua_State* state);

extern int DatabaseMetaTableId;

#endif //GMSV_MONGODB_DATABASE_HPP
