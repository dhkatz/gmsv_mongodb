#ifndef GMSV_MONGODB_CLIENT_HPP
#define GMSV_MONGODB_CLIENT_HPP

#include "MongoDB.hpp"

int new_client(lua_State* state);

int destroy_client(lua_State* state);

int client_command(lua_State* state);

int client_uri(lua_State* state);

int client_default_database(lua_State* state);

int client_list_databases(lua_State* state);

int client_database(lua_State* state);

int client_collection(lua_State* state);

extern int ClientMetaTableId;

#endif //GMSV_MONGODB_CLIENT_HPP
