#include "Collection.hpp"

#define CHECK_COLLECTION() \
        auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId); \
        if (collection == nullptr) return 0; \

LUA_FUNCTION(destroy_collection) {
    CHECK_COLLECTION()

    mongoc_collection_destroy(collection);

    return 0;
}

LUA_FUNCTION(collection_command) {
    CHECK_COLLECTION()

    BUILD_QUERY()

    bson_t reply;
    bson_error_t error;
    bool success = mongoc_collection_command_with_opts(collection, query, nullptr, opts, &reply, &error);

    CLEANUP_QUERY()

    if (!success) {
        bson_destroy(&reply);
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->ReferencePush(BSONToLua(LUA, &reply));
    bson_destroy(&reply);

    return 1;
}

LUA_FUNCTION(collection_name) {
    CHECK_COLLECTION()

    LUA->PushString(mongoc_collection_get_name(collection));

    return 1;
}

LUA_FUNCTION(collection_count) {
    CHECK_COLLECTION()

    BUILD_QUERY()

    bson_error_t error;
    int64_t count = mongoc_collection_count_documents(collection, query, opts, nullptr, nullptr, &error);

    CLEANUP_QUERY()

    if (count == -1) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushNumber(count);

    return 1;
}

LUA_FUNCTION(collection_find) {
    CHECK_COLLECTION()

    BUILD_QUERY()

    auto cursor = mongoc_collection_find_with_opts(collection, query, opts, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));

    CLEANUP_QUERY()

    LUA->CreateTable();

    int table = LUA->ReferenceCreate();

    const bson_t * bson;
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

LUA_FUNCTION(collection_find_one) {
    CHECK_COLLECTION()

    BUILD_QUERY()

    bson_t options;
    bson_init(&options);
    if (opts) bson_copy_to_excluding_noinit(opts, &options, "limit", "singleBatch", (char *)nullptr);

    BSON_APPEND_INT32(&options, "limit", 1 );
    BSON_APPEND_BOOL(&options, "singleBatch", true);

    auto cursor = mongoc_collection_find_with_opts(collection, query, &options, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));

    CLEANUP_QUERY()

    LUA->CreateTable();

    const bson_t* bson;
    mongoc_cursor_next(cursor, &bson);
    mongoc_cursor_destroy(cursor);

    LUA->ReferencePush(BSONToLua(LUA, bson));

    return 1;
}

LUA_FUNCTION(collection_insert) {
    CHECK_COLLECTION()

    BUILD_QUERY()

    bool success = mongoc_collection_insert(collection, MONGOC_INSERT_NONE, query, nullptr, nullptr);

    CLEANUP_QUERY()

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_remove) {
    CHECK_COLLECTION()

    BUILD_QUERY()

    bool success = mongoc_collection_remove(collection, MONGOC_REMOVE_NONE, query, nullptr, nullptr);

    CLEANUP_QUERY()

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_update) {
    CHECK_COLLECTION()

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);
    LUA->CheckType(3, GarrysMod::Lua::Type::Table);

    BUILD_QUERY()

    bson_error_t error;
    bool success = mongoc_collection_update(collection, MONGOC_UPDATE_MULTI_UPDATE, query, opts, nullptr, &error);

    CLEANUP_QUERY()

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_bulk) {
    CHECK_COLLECTION()

    BUILD_QUERY()

    auto bulk = mongoc_collection_create_bulk_operation_with_opts(collection, query);

    CLEANUP_QUERY()

    LUA->PushUserType(bulk, BulkMetaTableId);

    return 1;
}
