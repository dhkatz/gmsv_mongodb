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

    bson_t* command;
    try {
        command = LuaToBSON(LUA, commandRef);
    } catch (std::runtime_error& e) {
        LUA->ReferenceFree(commandRef);
        LUA->ThrowError(e.what());
        return 0;
    }

    bson_t reply;
    bson_error_t error;
    bool success = mongoc_collection_command_simple(collection, command, nullptr, &reply, &error);

    bson_destroy(command);

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
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    LUA->PushString(mongoc_collection_get_name(collection));

    return 1;
}

LUA_FUNCTION(collection_count) {
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
        return 0;
    }

    bson_t* query;
    try {
        query = LuaToBSON(LUA, queryRef);
    } catch (std::runtime_error& e) {
        if (optsRef != INT_MIN) LUA->ReferenceFree(optsRef);
        LUA->ReferenceFree(queryRef);
        LUA->ThrowError(e.what());
    }

    bson_t * opts;
    bson_error_t error;
    int64_t count;

    if (optsRef != INT_MIN) {
        try {
            opts = LuaToBSON(LUA, optsRef);
        } catch (std::runtime_error& e)  {
            LUA->ReferenceFree(optsRef);
            LUA->ThrowError(e.what());
        }
        count = mongoc_collection_count_documents(collection, query, opts, nullptr, nullptr, &error);
    } else {
        count = mongoc_collection_count_documents(collection, query, nullptr, nullptr, nullptr, &error);
    }

    bson_destroy(query);
    if (optsRef != INT_MIN) {
        bson_destroy(opts);
    }

    if (count == -1) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushNumber(count);

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

        return 0;
    }

    bson_t* query;
    try {
        query = LuaToBSON(LUA, queryRef);
    } catch (std::runtime_error& e) {
        if (optsRef != INT_MIN) LUA->ReferenceFree(optsRef);
        LUA->ReferenceFree(queryRef);
        LUA->ThrowError(e.what());
    }

    bson_t * opts;
    mongoc_cursor_t * cursor;

    if (optsRef != INT_MIN) {
        try {
            opts = LuaToBSON(LUA, optsRef);
        } catch (std::runtime_error& e)  {
            LUA->ReferenceFree(optsRef);
            LUA->ThrowError(e.what());
        }
        cursor = mongoc_collection_find_with_opts(collection, query, opts, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));
    } else {
        cursor = mongoc_collection_find_with_opts(collection, query, nullptr, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));
    }

    bson_destroy(query);
    if (optsRef != INT_MIN) {
        bson_destroy(opts);
    }

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

        return 0;
    }

    bson_t* query;
    try {
        query = LuaToBSON(LUA, queryRef);
    } catch (std::runtime_error& e) {
        if (optsRef != INT_MIN) LUA->ReferenceFree(optsRef);
        LUA->ReferenceFree(queryRef);
        LUA->ThrowError(e.what());
    }

    bson_t *options = nullptr;

    if (optsRef != INT_MIN) {
        try {
            options = LuaToBSON(LUA, optsRef);
        } catch (std::runtime_error& e)  {
            LUA->ReferenceFree(optsRef);
            LUA->ThrowError(e.what());
        }
    }

    bson_t opts;
    bson_init(&opts);
    if (options) bson_copy_to_excluding_noinit(options, &opts, "limit", "singleBatch", (char *)nullptr);
    BSON_APPEND_INT32(&opts, "limit", 1 );
    BSON_APPEND_BOOL(&opts, "singleBatch", true);

    auto cursor = mongoc_collection_find_with_opts(collection, query, &opts, mongoc_read_prefs_new(MONGOC_READ_PRIMARY));

    bson_destroy(query);
    bson_destroy(&opts);
    if (optsRef != INT_MIN) {
        bson_destroy(options);
    }

    LUA->CreateTable();

    const bson_t* bson;
    mongoc_cursor_next(cursor, &bson);
    mongoc_cursor_destroy(cursor);

    LUA->ReferencePush(BSONToLua(LUA, bson));

    return 1;
}

LUA_FUNCTION(collection_insert) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);

    int docRef = LUA->ReferenceCreate();

    bson_t* doc;
    try {
        doc = LuaToBSON(LUA, docRef);
    } catch (std::runtime_error& e) {
        LUA->ReferenceFree(docRef);
        LUA->ThrowError(e.what());
        return 0;
    }

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

    bson_t* selector;
    bson_t* update;

    try {
        selector = LuaToBSON(LUA, selRef);
        update = LuaToBSON(LUA, updRef);
    } catch (std::runtime_error& e) {
        LUA->ReferenceFree(selRef);
        LUA->ReferenceFree(updRef);
        LUA->ThrowError(e.what());
        return 0;
    }

    bson_error_t error;
    bool success = mongoc_collection_update (collection, MONGOC_UPDATE_MULTI_UPDATE, selector, update, nullptr, &error);

    bson_destroy(selector);
    bson_destroy(update);

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(collection_bulk) {
    auto collection = LUA->GetUserType<mongoc_collection_t>(1, CollectionMetaTableId);

    if (collection == nullptr) return 0;

    bson_t* opts = nullptr;

    if (LUA->IsType(2, GarrysMod::Lua::Type::Table)) {
        auto optsRef = LUA->ReferenceCreate();
        try {
            opts = LuaToBSON(LUA, optsRef);
        } catch (std::runtime_error& e) {
            LUA->ReferenceFree(optsRef);
            LUA->ThrowError(e.what());
            return 0;
        }
    }

    auto bulk = mongoc_collection_create_bulk_operation_with_opts(collection, opts);

    bson_destroy(opts);

    LUA->PushUserType(bulk, BulkMetaTableId);

    return 1;
}
