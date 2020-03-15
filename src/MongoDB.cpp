#include "MongoDB.hpp"

int ClientMetaTableId, DatabaseMetaTableId, CollectionMetaTableId;

GMOD_MODULE_OPEN() {
    mongoc_init();

    ClientMetaTableId = LUA->CreateMetaTable("MongoDBClient");

    LUA->Push(-1);
        LUA->SetField(-2, "__index");

        LUA->PushCFunction(destroy_client);
        LUA->SetField(-2, "__gc");

        LUA->PushCFunction(client_command);
        LUA->SetField(-2, "Command");

        LUA->PushCFunction(client_uri);
        LUA->SetField(-2, "Uri");

        LUA->PushCFunction(client_list_databases);
        LUA->SetField(-2, "DatabaseList");

        LUA->PushCFunction(client_database);
        LUA->SetField(-2, "Database");

        LUA->PushCFunction(client_collection);
        LUA->SetField(-2, "Collection");

    LUA->Pop();

    DatabaseMetaTableId = LUA->CreateMetaTable("MongoDBDatabase");

        LUA->Push(-1);
        LUA->SetField(-2, "__index");

        LUA->PushCFunction(destroy_database);
        LUA->SetField(-2, "__gc");

        LUA->PushCFunction(database_name);
        LUA->SetField(-2, "Name");

        LUA->PushCFunction(database_copy);
        LUA->SetField(-2, "Copy");

        LUA->PushCFunction(database_drop);
        LUA->SetField(-2, "Drop");

        LUA->PushCFunction(database_command);
        LUA->SetField(-2, "Command");

        LUA->PushCFunction(database_user_add);
        LUA->SetField(-2, "AddUser");

        LUA->PushCFunction(database_user_remove);
        LUA->SetField(-2, "RemoveUser");

        LUA->PushCFunction(database_collection_exists);
        LUA->SetField(-2, "HasCollection");

        LUA->PushCFunction(database_collection_get);
        LUA->SetField(-2, "GetCollection");

        LUA->PushCFunction(database_collection_create);
        LUA->SetField(-2, "AddCollection");

    LUA->Pop();

    CollectionMetaTableId = LUA->CreateMetaTable("MongoDBCollection");

        LUA->Push(-1);
        LUA->SetField(-2, "__index");

        LUA->PushCFunction(destroy_collection);
        LUA->SetField(-2, "__gc");

        LUA->PushCFunction(collection_command);
        LUA->SetField(-2, "Command");

        LUA->PushCFunction(collection_find);
        LUA->SetField(-2, "Find");

        LUA->PushCFunction(collection_insert);
        LUA->SetField(-2, "Insert");

        LUA->PushCFunction(collection_update);
        LUA->SetField(-2, "Update");

        LUA->PushCFunction(collection_remove);
        LUA->SetField(-2, "Remove");

    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushString("mongodb");
    LUA->PushCFunction(new_client);
    LUA->SetTable(-3);

    return 0;
}

GMOD_MODULE_CLOSE() {
    mongoc_cleanup();

    return 0;
}
