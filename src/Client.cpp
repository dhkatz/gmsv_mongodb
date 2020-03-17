#include "Client.hpp"

#define CHECK_CLIENT() \
    auto client = LUA->GetUserType<mongoc_client_t>(1, ClientMetaTableId); \
    if (client == nullptr) return 0;

/**
 * Create a new MongoDB C client.
 * @param LUA Lua state
 * @return
 */
LUA_FUNCTION(new_client) {
    auto uri = LUA->CheckString(1);
    auto name = LUA->CheckString(2);

    auto client = mongoc_client_new(uri);

    if (!client)  {
        LUA->ThrowError("MongoDB failed to create a client!");
        return 0;
    }

    mongoc_client_set_appname(client, name);

    bson_error_t error;

    if (!mongoc_client_command_simple(client, "admin", BCON_NEW("ping", BCON_INT32(1)), nullptr, nullptr, &error)) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushUserType(client, ClientMetaTableId);

    return 1;
}

/**
 * Used by Lua to perform client garbage collection.
 * @param LUA Lua state
 * @return
 */
LUA_FUNCTION(destroy_client) {
    CHECK_CLIENT()

    mongoc_client_destroy(client);

    return 0;
}

/**
 * Run a BSON command on the client.
 * @param LUA Lua state
 * @return
 */
LUA_FUNCTION(client_command) {
    CHECK_CLIENT()

    auto database = LUA->CheckString(2);
    LUA->CheckType(3, GarrysMod::Lua::Type::Table);

    auto ref = LUA->ReferenceCreate();

    auto command = LuaToBSON(LUA, ref);
    bson_t reply;
    bson_error_t error;

    bool success = mongoc_client_command_simple(client, database, command, nullptr, &reply, &error);
    bson_destroy(command);
    if (!success) {
        LUA->ThrowError(error.message);
        bson_destroy(&reply);
        return 0;
    }

    auto replyRef = BSONToLua(LUA, &reply);
    bson_destroy(&reply);
    LUA->ReferencePush(replyRef);

    return 1;
}

/**
 * Retrieve the client's current URI.
 * @param LUA Lua state
 * @return
 */
LUA_FUNCTION(client_uri) {
    CHECK_CLIENT()

    auto uri = mongoc_client_get_uri(client);

    LUA->PushString(mongoc_uri_get_string(uri));

    return 1;
}

LUA_FUNCTION(client_default_database) {
    CHECK_CLIENT()

    auto database = mongoc_client_get_default_database(client);

    LUA->PushUserType(database, DatabaseMetaTableId);

    return 1;
}

/**
 * Get a list of the MongoDB databases.
 * @see client_database
 * @param LUA
 * @return
 */
LUA_FUNCTION(client_list_databases) {
    CHECK_CLIENT()

    auto cursor = mongoc_client_find_databases_with_opts(client, nullptr);

    const bson_t* bson;

    LUA->CreateTable();

    auto table = LUA->ReferenceCreate();

    for (int i = 0; mongoc_cursor_next(cursor, &bson); ++i) {
        LUA->ReferencePush(table);
            LUA->PushNumber(i + 1);
            LUA->ReferencePush(BSONToLua(LUA, bson));
        LUA->SetTable(-3);
    }

    mongoc_cursor_destroy(cursor);

    LUA->ReferencePush(table);

    return 1;
}

/**
 * Retrieve a MongoDB database and pass it to Lua
 * @param LUA
 * @return
 */
LUA_FUNCTION(client_database) {
    CHECK_CLIENT()

    auto database = LUA->CheckString(2);
    auto db = mongoc_client_get_database(client, database);

    bson_error_t error;
    bool success = mongoc_database_command_simple(db, BCON_NEW("ping", BCON_INT32(1)), nullptr, nullptr, &error);
    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushUserType(db, DatabaseMetaTableId);

    return 1;
}

/**
 * Retrieve a MongoDB collection and pass it to Lua
 * @param LUA
 * @return
 */
LUA_FUNCTION(client_collection) {
    CHECK_CLIENT()

    auto database = LUA->CheckString(2);
    auto name = LUA->CheckString(3);

    auto collection = mongoc_client_get_collection(client, database, name);

    LUA->PushUserType(collection, CollectionMetaTableId);

    return 1;
}
