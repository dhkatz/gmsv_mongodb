#include "Bulk.hpp"

#define CHECK_BULK() \
    auto bulk = LUA->GetUserType<mongoc_bulk_operation_t>(1, BulkMetaTableId); \
    if (bulk == nullptr) return 0;

LUA_FUNCTION(destroy_bulk) {
    CHECK_BULK()

    mongoc_bulk_operation_destroy(bulk);

    return 0;
}

LUA_FUNCTION(bulk_execute) {
    CHECK_BULK()

    bson_t reply;
    bson_error_t error;
    if (!mongoc_bulk_operation_execute(bulk, &reply, &error)) {
        bson_destroy(&reply);
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->ReferencePush(BSONToLua(LUA, &reply));

    return 1;
}

LUA_FUNCTION(bulk_insert) {
    CHECK_BULK()

    BUILD_QUERY()

    bson_error_t error;
    bool success = mongoc_bulk_operation_insert_with_opts(bulk, query, opts, &error);

    CLEANUP_QUERY()

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    return 1;
}

LUA_FUNCTION(bulk_remove) {
    CHECK_BULK()

    BUILD_QUERY()

    bson_error_t error;
    bool success = mongoc_bulk_operation_remove_one_with_opts(bulk, query, opts, &error);

    CLEANUP_QUERY()

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    return 1;
}

LUA_FUNCTION(bulk_update) {
    CHECK_BULK()

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);
    LUA->CheckType(3, GarrysMod::Lua::Type::Table);

    BUILD_QUERY()

    bson_error_t error;
    bool success = mongoc_bulk_operation_update_one_with_opts(bulk, query, opts, nullptr, &error);

    CLEANUP_QUERY()

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    return 1;
}


LUA_FUNCTION(bulk_replace) {
    CHECK_BULK()

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);
    LUA->CheckType(3, GarrysMod::Lua::Type::Table);

    BUILD_QUERY()

    bson_error_t error;
    bool success = mongoc_bulk_operation_replace_one_with_opts(bulk, query, opts, nullptr, &error);

    CLEANUP_QUERY()

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    return 1;
}

