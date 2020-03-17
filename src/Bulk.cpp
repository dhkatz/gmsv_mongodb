#include "Bulk.hpp"

LUA_FUNCTION(destroy_bulk) {
    auto bulk = LUA->GetUserType<mongoc_bulk_operation_t>(1, BulkMetaTableId);

    if (bulk == nullptr) return 0;

    mongoc_bulk_operation_destroy(bulk);

    return 0;
}

LUA_FUNCTION(bulk_execute) {
    auto bulk = LUA->GetUserType<mongoc_bulk_operation_t>(1, BulkMetaTableId);

    if (bulk == nullptr) return 0;

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
    auto bulk = LUA->GetUserType<mongoc_bulk_operation_t>(1, BulkMetaTableId);

    if (bulk == nullptr) return 0;

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

        return 0;
    }

    bson_t* query;
    try {
        query = LuaToBSON(LUA, queryRef);
    } catch (std::exception& e) {
        if (optsRef != INT_MIN) LUA->ReferenceFree(optsRef);
        LUA->ReferenceFree(queryRef);
        LUA->ThrowError(e.what());
    }

    bson_t * opts;
    bson_error_t error;
    bool success;

    if (optsRef != INT_MIN) {
        try {
            opts = LuaToBSON(LUA, optsRef);
        } catch (std::exception& e)  {
            LUA->ReferenceFree(optsRef);
            LUA->ThrowError(e.what());
        }
        success = mongoc_bulk_operation_insert_with_opts(bulk, query, opts, &error);
    } else {
        success = mongoc_bulk_operation_insert_with_opts(bulk, query, nullptr, &error);
    }

    bson_destroy(query);
    if (optsRef != INT_MIN) {
        bson_destroy(opts);
    }

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    return 1;
}

LUA_FUNCTION(bulk_remove) {

    return 1;
}

LUA_FUNCTION(bulk_update) {

    return 1;
}


LUA_FUNCTION(bulk_replace) {

    return 1;
}

