#include "Collection.hpp"

LUA_FUNCTION(destroy_collection) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    mongoc_collection_destroy(collection);

    return 0;
}

LUA_FUNCTION(collection_command) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);
    int commandRef = LUA->ReferenceCreate();

    auto command = LuaToBSON(LUA, commandRef);

    bson_t reply;
    bson_error_t error;
    bool success = mongoc_collection_command_simple(collection, command, nullptr, &reply, &error);

    bson_destroy(command);

    if (success) {
        LUA->ReferencePush(BSONToLua(LUA, &reply));
    } else {
        LUA->PushBool(success);
    }

    bson_destroy(&reply);

    return 1;
}

LUA_FUNCTION(collection_find) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    int queryRef = INT_MIN, optsRef = INT_MIN;

    if (LUA->Top() == 3) {
        LUA->CheckType(2, GarrysMod::Lua::Type::Table);
        LUA->CheckType(3, GarrysMod::Lua::Type::Table);
        optsRef = LUA->ReferenceCreate();
        queryRef = LUA->ReferenceCreate();
    } else if (LUA->Top() == 2) {
        LUA->CheckType(2, GarrysMod::Lua::Type::Table);
        queryRef = LUA->ReferenceCreate();
    } else {
        LUA->ThrowError("Received incorrect number of arguments!");
    }

    auto query = LuaToBSON(LUA, queryRef);
    if (query == nullptr) {
        LUA->ThrowError("Failed to build query!");
        return 0;
    }

    bson_t * opts;
    mongoc_cursor_t * cursor;

    if (optsRef != INT_MIN) {
        opts = LuaToBSON(LUA, optsRef);
        cursor = mongoc_collection_find_with_opts(collection, query, opts, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));
    } else {
        cursor = mongoc_collection_find_with_opts(collection, query, nullptr, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));
    }

    bson_destroy(query);
    if (optsRef != INT_MIN) {
        bson_destroy(opts);
    }

    const bson_t * bson;

    LUA->CreateTable();

    int table = LUA->ReferenceCreate();

    for (int i = 0; mongoc_cursor_next(cursor, &bson); ++i) {
        LUA->ReferencePush(table);
        LUA->PushNumber(i);
        LUA->ReferencePush(BSONToLua(LUA, bson));
        LUA->SetTable(-3);
    }

    mongoc_cursor_destroy(cursor);
    LUA->ReferencePush(table);

    return 1;
}

LUA_FUNCTION(collection_insert) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);

    int docRef = LUA->ReferenceCreate();
    auto doc = LuaToBSON(LUA, docRef);

    bool success = mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, nullptr, nullptr);

    bson_destroy(doc);

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_remove) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);

    int selRef = LUA->ReferenceCreate();
    auto selector = LuaToBSON(LUA, selRef);

    bool success = mongoc_collection_remove (collection, MONGOC_REMOVE_NONE, selector, nullptr, nullptr);

    bson_destroy(selector);

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_update) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    LUA->CheckType(3, GarrysMod::Lua::Type::Table);
    LUA->CheckType(2, GarrysMod::Lua::Type::Table);

    int updRef = LUA->ReferenceCreate();
    int selRef = LUA->ReferenceCreate();

    auto selector = LuaToBSON(LUA, selRef);
    auto update = LuaToBSON(LUA, updRef);

    bool success = mongoc_collection_update (collection, MONGOC_UPDATE_MULTI_UPDATE, selector, update, nullptr, nullptr);

    bson_destroy(selector);
    bson_destroy(update);

    LUA->PushBool(success);

    return 1;
}
