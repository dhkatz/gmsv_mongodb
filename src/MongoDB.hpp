#ifndef GMSV_MONGODB_MONGODB_HPP
#define GMSV_MONGODB_MONGODB_HPP

#include <stdexcept>

#include <mongoc/mongoc.h>

#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/Types.h>

/**
 * Setup the query and options BSON data with error handling.
 * WARNING: Always call CLEANUP_BSON() before throwing other errors or returning!
 */

#define EXPAND( x ) x

#define GET_MACRO(_1, _2, _3, NAME, ...) NAME

#define CHECK_BSON(...) EXPAND(GET_MACRO(__VA_ARGS__, CHECK_BSON3, CHECK_BSON2, CHECK_BSON1)(__VA_ARGS__))

#define CHECK_BSON1(VAR1) \
    int ref_##VAR1 = INT_MIN; \
    if (LUA->Top() == 2) { \
        LUA->CheckType(2, GarrysMod::Lua::Type::Table); \
        ref_##VAR1 = LUA->ReferenceCreate(); \
    } else { \
        LUA->ThrowError("Received incorrect number of arguments!"); \
        return 0; \
    } \
    bson_t* VAR1; \
    try { \
        VAR1 = LuaToBSON(LUA, ref_##VAR1); \
    } catch (std::runtime_error& e) { \
        LUA->ReferenceFree(ref_##VAR1); \
        LUA->ThrowError(e.what()); \
    }

#define CHECK_BSON2(VAR1, VAR2) \
    int ref_##VAR1 = INT_MIN, ref_##VAR2 = INT_MIN; \
    if (LUA->Top() == 3) { \
        LUA->CheckType(2, GarrysMod::Lua::Type::Table); \
        LUA->CheckType(3, GarrysMod::Lua::Type::Table); \
        ref_##VAR2 = LUA->ReferenceCreate(); \
        ref_##VAR1 = LUA->ReferenceCreate(); \
    } else if (LUA->Top() == 2) { \
        LUA->CheckType(2, GarrysMod::Lua::Type::Table); \
        ref_##VAR1 = LUA->ReferenceCreate(); \
    } else { \
        LUA->ThrowError("Received incorrect number of arguments!"); \
        return 0; \
    } \
    bson_t* VAR1; \
    try { \
        VAR1 = LuaToBSON(LUA, ref_##VAR1); \
    } catch (std::runtime_error& e) { \
        if (ref_##VAR2 != INT_MIN) LUA->ReferenceFree(ref_##VAR2); \
        LUA->ReferenceFree(ref_##VAR1); \
        LUA->ThrowError(e.what()); \
    } \
    bson_t* VAR2 = nullptr; \
    if (ref_##VAR2 != INT_MIN) { \
        try { \
            VAR2 = LuaToBSON(LUA, ref_##VAR2); \
        } catch (std::runtime_error& e) { \
            LUA->ReferenceFree(ref_##VAR2); \
            LUA->ThrowError(e.what()); \
        } \
    }

#define CHECK_BSON3(VAR1, VAR2, VAR3) \
    int ref_##VAR1 = INT_MIN, ref_##VAR2 = INT_MIN, ref_##VAR3 = INT_MIN; \
    if (LUA->Top() == 4) { \
        LUA->CheckType(2, GarrysMod::Lua::Type::Table); \
        LUA->CheckType(3, GarrysMod::Lua::Type::Table); \
        LUA->CheckType(4, GarrysMod::Lua::Type::Table); \
        ref_##VAR3 = LUA->ReferenceCreate(); \
        ref_##VAR2 = LUA->ReferenceCreate(); \
        ref_##VAR1 = LUA->ReferenceCreate(); \
    } else if (LUA->Top() == 3) { \
        LUA->CheckType(2, GarrysMod::Lua::Type::Table); \
        LUA->CheckType(3, GarrysMod::Lua::Type::Table); \
        ref_##VAR2 = LUA->ReferenceCreate(); \
        ref_##VAR1 = LUA->ReferenceCreate(); \
    } else { \
        LUA->ThrowError("Received incorrect number of arguments!"); \
        return 0; \
    } \
    bson_t* (VAR1); \
    try { \
        (VAR1) = LuaToBSON(LUA, ref_##VAR1); \
    } catch (std::runtime_error& e) { \
        if (ref_##VAR2 != INT_MIN) LUA->ReferenceFree(ref_##VAR2); \
        if (ref_##VAR3 != INT_MIN) LUA->ReferenceFree(ref_##VAR3); \
        LUA->ReferenceFree(ref_##VAR1); \
        LUA->ThrowError(e.what()); \
    } \
    bson_t * (VAR2) = nullptr; \
    if (ref_##VAR2 != INT_MIN) { \
        try { \
            VAR2 = LuaToBSON(LUA, ref_##VAR2); \
        } catch (std::runtime_error& e) { \
            if (ref_##VAR3 != INT_MIN) LUA->ReferenceFree(ref_##VAR3); \
            LUA->ReferenceFree(ref_##VAR2); \
            LUA->ThrowError(e.what()); \
        } \
    } \
    bson_t * (VAR3) = nullptr; \
    if (ref_##VAR3 != INT_MIN) { \
        try { \
            VAR3 = LuaToBSON(LUA, ref_##VAR3); \
        } catch (std::runtime_error& e) { \
            LUA->ReferenceFree(ref_##VAR3); \
            LUA->ThrowError(e.what()); \
        } \
    }

/**
 * Cleanup the BSON data.
 */
#define CLEANUP_BSON(...) EXPAND(GET_MACRO(__VA_ARGS__, CLEANUP_BSON3, CLEANUP_BSON2, CLEANUP_BSON1)(__VA_ARGS__))

#define CLEANUP_BSON1(VAR1) bson_destroy(VAR1);

#define CLEANUP_BSON2(VAR1, VAR2) bson_destroy(VAR1); if (ref_##VAR2 != INT_MIN) { bson_destroy(VAR2); }

#define CLEANUP_BSON3(VAR1, VAR2, VAR3) bson_destroy(VAR1); if (ref_##VAR2 != INT_MIN) bson_destroy(VAR3); if (ref_##VAR3 != INT_MIN) bson_destroy(VAR3);

#include "Util.hpp"
#include "Client.hpp"
#include "Database.hpp"
#include "Collection.hpp"
#include "Bulk.hpp"

#endif //GMSV_MONGODB_MONGODB_HPP
