#include "ObjectID.hpp"

#define CHECK_OBJECTID() auto oid = LUA->GetUserType<bson_oid_t>(1, ObjectIDMetaTableId); if (oid == nullptr) return 0;

LUA_FUNCTION(new_objectid) {
    if (LUA->IsType(1, GarrysMod::Lua::Type::String)) {
        unsigned int len;
        auto str = LUA->GetString(1, &len);

        if (!bson_oid_is_valid(str, len)) {
            LUA->ThrowError("Invalid type passed to ObjectID()");
            return 0;
        }

        bson_oid_t oid;

        bson_oid_init_from_string(&oid, str);

        LUA->PushUserType(&oid, ObjectIDMetaTableId);
    } else if (LUA->IsType(1, GarrysMod::Lua::Type::Nil) || LUA->IsType(1, GarrysMod::Lua::Type::None)) {
        bson_oid_t oid;

        bson_oid_init(&oid, nullptr);

        LUA->PushUserType(&oid, ObjectIDMetaTableId);
    } else {
        LUA->ThrowError("Invalid type passed to ObjectID");
        return 0;
    }

    return 1;
}

LUA_FUNCTION(objectid_tostring) {
    CHECK_OBJECTID()

    char buf[25];
    bson_oid_to_string(oid, buf);

    LUA->PushString(buf);

    return 1;
}

LUA_FUNCTION(objectid_eq) {
    CHECK_OBJECTID()

    auto oid2 = LUA->GetUserType<bson_oid_t>(1, ObjectIDMetaTableId);

    if (oid2 == nullptr) return false;

    LUA->PushBool(bson_oid_equal(oid, oid2));

    return 1;
}

LUA_FUNCTION(objectid_data) {
    CHECK_OBJECTID()

    LUA->PushString(reinterpret_cast<const char *>(oid->bytes), sizeof(oid->bytes));

    return 1;
}

LUA_FUNCTION(objectid_hash) {
    CHECK_OBJECTID()

    LUA->PushNumber(bson_oid_hash(oid));

    return 1;
}
