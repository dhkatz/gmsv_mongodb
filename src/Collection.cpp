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

    CHECK_BSON(command, opts)

    SETUP_QUERY(error, reply)

    bool success = mongoc_collection_command_with_opts(collection, command, nullptr, opts, &reply, &error);

    CLEANUP_BSON(command, opts)

    CLEANUP_QUERY(error, reply, !success)

    LUA->ReferencePush(BSONToLua(LUA, &reply));

    return 1;
}

LUA_FUNCTION(collection_name) {
    CHECK_COLLECTION()

    LUA->PushString(mongoc_collection_get_name(collection));

    return 1;
}

LUA_FUNCTION(collection_count) {
    CHECK_COLLECTION()

    CHECK_BSON(filter, opts)

    SETUP_QUERY(error)

    int64_t count = mongoc_collection_count_documents(collection, filter, opts, nullptr, nullptr, &error);

    CLEANUP_BSON(filter, opts)

    CLEANUP_QUERY(error, count == -1)

    LUA->PushNumber((double)count);

    return 1;
}

LUA_FUNCTION(collection_find) {
    CHECK_COLLECTION()

    CHECK_BSON(filter, opts)

    auto cursor = mongoc_collection_find_with_opts(collection, filter, opts, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));

    CLEANUP_BSON(filter, opts)

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

    CHECK_BSON(filter, opts)

    bson_t options;
    bson_init(&options);
    if (opts) bson_copy_to_excluding_noinit(opts, &options, "limit", "singleBatch", (char *)nullptr);

    BSON_APPEND_INT32(&options, "limit", 1 );
    BSON_APPEND_BOOL(&options, "singleBatch", true);

    auto cursor = mongoc_collection_find_with_opts(collection, filter, &options, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));

    CLEANUP_BSON(filter, opts)

    LUA->CreateTable();

    const bson_t* bson;
    mongoc_cursor_next(cursor, &bson);
    mongoc_cursor_destroy(cursor);

    LUA->ReferencePush(BSONToLua(LUA, bson));

    return 1;
}

LUA_FUNCTION(collection_insert) {
    CHECK_COLLECTION()

    CHECK_BSON(document)

    SETUP_QUERY(error)

    bool success = mongoc_collection_insert(collection, MONGOC_INSERT_NONE, document, nullptr, &error);

    CLEANUP_BSON(document)

    CLEANUP_QUERY(error, !success)

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_remove) {
    CHECK_COLLECTION()

    CHECK_BSON(selector)

    SETUP_QUERY(error)

    bool success = mongoc_collection_remove(collection, MONGOC_REMOVE_NONE, selector, nullptr, &error);

    CLEANUP_BSON(selector)

    CLEANUP_QUERY(error, !success)

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_update) {
    CHECK_COLLECTION()

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);
    LUA->CheckType(3, GarrysMod::Lua::Type::Table);

    CHECK_BSON(selector, update)

    SETUP_QUERY(error)

    bool success = mongoc_collection_update(collection, MONGOC_UPDATE_MULTI_UPDATE, selector, update, nullptr, &error);

    CLEANUP_BSON(selector, update)

    CLEANUP_QUERY(error, !success)

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_bulk) {
    CHECK_COLLECTION()

    CHECK_BSON(opts)

    auto bulk = mongoc_collection_create_bulk_operation_with_opts(collection, opts);

    CLEANUP_BSON(opts)

    LUA->PushUserType(bulk, BulkMetaTableId);

    return 1;
}
