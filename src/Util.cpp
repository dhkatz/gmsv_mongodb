#include "Util.hpp"

const char* LuaToJSON(GarrysMod::Lua::ILuaBase* LUA, int ref) {
    LUA->ReferencePush(ref);

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-1, "util");
    LUA->GetField(-1, "TableToJSON");
    LUA->ReferencePush(ref);

    if (LUA->PCall(1, 1, 0) != 0) {
        throw std::runtime_error(LUA->GetString(-1));
    }

    if (!LUA->IsType(-1, GarrysMod::Lua::Type::String)) {
        throw std::runtime_error("Invalid table passed to MongoDB!");
    }

    auto json = LUA->GetString(-1);

    LUA->Pop(2);
    LUA->ReferenceFree(ref);

    return json;
}

bson_t* LuaToBSON(GarrysMod::Lua::ILuaBase* LUA, int ref) {
    auto json = LuaToJSON(LUA, ref);

    bson_error_t error;
    auto bson = bson_new_from_json((const uint8_t*)json, -1,  &error);
    if (error.code != 0) {
        throw std::runtime_error(error.message);
    }

    return bson;
}

int BSONToLua(GarrysMod::Lua::ILuaBase* LUA, const bson_t* bson) {
    bson_iter_t iter;

    LUA->CreateTable();

    if (bson_iter_init(&iter, bson)) {
        while (bson_iter_next(&iter)) {
            auto type = bson_iter_type(&iter);

            switch (type) {
                case BSON_TYPE_DOUBLE:
                    LUA->PushNumber(bson_iter_as_double(&iter));
                    break;
                case BSON_TYPE_INT32:
                case BSON_TYPE_INT64:
                    LUA->PushNumber((double) bson_iter_as_int64(&iter));
                    break;
                case BSON_TYPE_BOOL:
                    LUA->PushBool(bson_iter_as_bool(&iter));
                    break;
                case BSON_TYPE_UTF8:
                    LUA->PushString(bson_iter_utf8(&iter, nullptr));
                    break;
                case BSON_TYPE_DATE_TIME:
                    LUA->PushNumber((double) bson_iter_date_time(&iter));
                    break;
                case BSON_TYPE_REGEX:
                    LUA->PushString(bson_iter_regex(&iter, nullptr));
                    break;
                case BSON_TYPE_CODE:
                    LUA->PushString(bson_iter_code(&iter, nullptr));
                    break;
                case BSON_TYPE_TIMESTAMP: {
                    uint32_t t;
                    bson_iter_timestamp(&iter, &t, nullptr);
                    LUA->PushNumber(t);
                    break;
                }
                case BSON_TYPE_OID: {
                    const bson_oid_t *oid = bson_iter_oid(&iter);

                    LUA->PushUserType((void *) oid, ObjectIDMetaTableId);
                    break;
                }
                case BSON_TYPE_DOCUMENT: {
                    uint32_t len;
                    const uint8_t *data;
                    bson_t b;
                    bson_iter_document(&iter, &len, &data);
                    bson_init_static(&b, data, (size_t) len);
                    LUA->ReferencePush(BSONToLua(LUA, &b));
                    bson_destroy(&b);
                    break;
                }
                case BSON_TYPE_ARRAY: {
                    uint32_t len;
                    const uint8_t *data;
                    bson_t b;
                    bson_iter_array(&iter, &len, &data);
                    bson_init_static(&b, data, (size_t) len);
                    LUA->ReferencePush(BSONToLua(LUA, &b));
                    bson_destroy(&b);
                    break;
                }
                case BSON_TYPE_NULL:
                    LUA->PushNil();
                    break;
                default:
                    continue;
            }

            LUA->SetField(-2, bson_iter_key(&iter));
        }
    }

    return LUA->ReferenceCreate();
}
