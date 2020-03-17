#ifndef GMSV_MONGODB_MONGODB_HPP
#define GMSV_MONGODB_MONGODB_HPP

#include <stdexcept>

#include <mongoc/mongoc.h>

#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/Types.h>

/**
 * Setup the query and options BSON data with error handling.
 * WARNING: Always call CLEANUP_QUERY() before throwing other errors or returning!
 */
#define BUILD_QUERY() \
        int queryRef = INT_MIN, optsRef = INT_MIN; \
        if (LUA->Top() == 3) { \
            LUA->CheckType(2, GarrysMod::Lua::Type::Table); \
            LUA->CheckType(3, GarrysMod::Lua::Type::Table); \
            optsRef = LUA->ReferenceCreate(); \
            queryRef = LUA->ReferenceCreate(); \
        } else if (LUA->Top() == 2) { \
            LUA->CheckType(2, GarrysMod::Lua::Type::Table); \
            queryRef = LUA->ReferenceCreate(); \
        } else { \
            LUA->ThrowError("Received incorrect number of arguments!"); \
            return 0; \
        } \
        bson_t* query; \
        try { \
            query = LuaToBSON(LUA, queryRef); \
        } catch (std::runtime_error& e) { \
            if (optsRef != INT_MIN) LUA->ReferenceFree(optsRef); \
            LUA->ReferenceFree(queryRef); \
            LUA->ThrowError(e.what()); \
        } \
        bson_t * opts = nullptr; \
        if (optsRef != INT_MIN) { \
            try { \
                opts = LuaToBSON(LUA, optsRef); \
            } catch (std::runtime_error& e) { \
                LUA->ReferenceFree(optsRef); \
                LUA->ThrowError(e.what()); \
            } \
        }

/**
 * Cleanup the query and options data.
 */
#define CLEANUP_QUERY() \
        bson_destroy(query); \
        if (optsRef != INT_MIN) { \
            bson_destroy(opts); \
        }


#include "Util.hpp"
#include "Client.hpp"
#include "Database.hpp"
#include "Collection.hpp"
#include "Bulk.hpp"

#endif //GMSV_MONGODB_MONGODB_HPP
