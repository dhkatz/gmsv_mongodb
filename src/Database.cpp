#include "Database.hpp"

LUA_FUNCTION(destroy_database) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    mongoc_database_destroy(database);

    return 0;
}

LUA_FUNCTION(database_name) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    auto name = mongoc_database_get_name(database);

    LUA->PushString(name);

    return 1;
}

LUA_FUNCTION(database_copy) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    auto copy = mongoc_database_copy(database);

    LUA->PushUserType(copy, DatabaseMetaTableId);

    return 1;
}

LUA_FUNCTION(database_drop) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    bson_error_t error;
    bool success = mongoc_database_drop(database, &error);
    if (!success) {
        LUA->ThrowError(error.message);
    }

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(database_command) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    LUA->CheckType(2, GarrysMod::Lua::Type::Table);

    auto commandRef = LUA->ReferenceCreate();
    auto command = LuaToBSON(LUA, commandRef);
    LUA->ReferenceFree(commandRef);

    bson_t reply;
    bson_error_t error;
    bool success = mongoc_database_command_simple(database, command, nullptr, &reply, &error);

    if (!success) {
        LUA->ThrowError(error.message);
        return 0;
    }

    bson_destroy(command);

    auto resultRef = BSONToLua(LUA, &reply);

    LUA->ReferencePush(resultRef);

    return 1;
}

LUA_FUNCTION(database_user_add) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    auto username = LUA->CheckString(2);
    auto password = LUA->CheckString(3);

    int rolesRef = INT_MIN, dataRef = INT_MIN;

    if (LUA->Top() == 5) {
        if (LUA->IsType(4, GarrysMod::Lua::Type::Table) && LUA->IsType(5, GarrysMod::Lua::Type::Table)) {
            dataRef = LUA->ReferenceCreate();
            rolesRef = LUA->ReferenceCreate();
        } else if (LUA->IsType(5, GarrysMod::Lua::Type::Table)) {
            dataRef = LUA->ReferenceCreate();
        } else {
            LUA->ThrowError("Incorrect number of arguments passed!");
        }
    } else if (LUA->Top() == 4 && LUA->IsType(4, GarrysMod::Lua::Type::Table)) {
        rolesRef = LUA->ReferenceCreate();
    }

    bson_t* roles;
    if (rolesRef != INT_MIN) {
        roles = LuaToBSON(LUA, rolesRef);
        LUA->ReferenceFree(rolesRef);
    }

    bson_t* data;
    if (dataRef != INT_MIN) {
        data = LuaToBSON(LUA, dataRef);
    }

    bson_error_t error;
    bool success = mongoc_database_add_user(database, username, password, roles, data, &error);

    if (rolesRef != INT_MIN) {
        LUA->ReferenceFree(rolesRef);
        bson_destroy(roles);
    }

    if (dataRef != INT_MIN) {
        LUA->ReferenceFree(dataRef);
        bson_destroy(data);
    }

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(database_user_remove) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    auto username = LUA->CheckString(2);

    bson_error_t error;
    bool success = mongoc_database_remove_user(database, username, &error);
    if (error.code != 0) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushBool(success);

    return 1;
}

LUA_FUNCTION(database_collection_exists) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    auto name = LUA->CheckString(2);

    bson_error_t error;
    bool exists = mongoc_database_has_collection(database, name, &error);
    if (error.code != 0) {
        LUA->ThrowError(error.message);
        return 0;
    }

    LUA->PushBool(exists);

    return 1;
}

LUA_FUNCTION(database_collection_get) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    auto name = LUA->CheckString(2);

    auto collection = mongoc_database_get_collection(database, name);

    LUA->PushUserType(collection, CollectionMetaTableId);

    return 1;
}

LUA_FUNCTION(database_collection_create) {
    auto database = LUA->GetUserType<mongoc_database_t>(1, DatabaseMetaTableId);

    if (database == nullptr) return 0;

    auto name = LUA->CheckString(2);

    bson_t* options;
    if (LUA->IsType(3, GarrysMod::Lua::Type::Table)) {
        int optionsRef = LUA->ReferenceCreate();
        options = LuaToBSON(LUA, optionsRef);
    }

    bson_error_t error;
    auto collection = mongoc_database_create_collection(database, name, options, &error);

    LUA->PushUserType(collection, CollectionMetaTableId);

    return 1;
}
