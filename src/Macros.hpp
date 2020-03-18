#ifndef GMSV_MONGODB_MACROS_HPP
#define GMSV_MONGODB_MACROS_HPP

#define EXPAND( x ) x
#define NUM_ARGS_(_1, _2, _3, _4, _5, TOTAL, ...) TOTAL
#define NUM_ARGS(...) EXPAND(NUM_ARGS_(__VA_ARGS__, 3, 2, 1, 0))
#define CAT_(X, Y) X##Y
#define CAT(MACRO, NUMBER) EXPAND(CAT_(MACRO, NUMBER))
#define VA_MACRO(MACRO, ...) CAT(MACRO, EXPAND(NUM_ARGS (__VA_ARGS__)))(__VA_ARGS__)

/**
 * Setup the BSON data with error handling.
 * WARNING: Always call CLEANUP_BSON() before throwing other errors or returning!
 */

#define CHECK_BSON( ... ) EXPAND(VA_MACRO(CHECK_BSON, void, void, __VA_ARGS__))

#define CHECK_BSON1(s, t, VAR1) \
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

#define CHECK_BSON2(s, t, VAR1, VAR2) \
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

#define CHECK_BSON3(s, t, VAR1, VAR2, VAR3) \
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
#define CLEANUP_BSON(...) EXPAND(VA_MACRO( CLEANUP_BSON, void, void, __VA_ARGS__ ))

#define CLEANUP_BSON1(s, t, VAR1) bson_destroy(VAR1);

#define CLEANUP_BSON2(s, t, VAR1, VAR2) bson_destroy(VAR1); if (ref_##VAR2 != INT_MIN) { bson_destroy(VAR2); }

#define CLEANUP_BSON3(s, t, VAR1, VAR2, VAR3) bson_destroy(VAR1); if (ref_##VAR2 != INT_MIN) bson_destroy(VAR3); if (ref_##VAR3 != INT_MIN) bson_destroy(VAR3);


#define SETUP_QUERY(...) EXPAND(VA_MACRO( SETUP_QUERY, void, void, __VA_ARGS__ ))

#define SETUP_QUERY1(s, t, VAR1) bson_error_t VAR1;

#define SETUP_QUERY2(s, t, VAR1, VAR2) bson_error_t VAR1; bson_t VAR2;

#define CLEANUP_QUERY(...) EXPAND(VA_MACRO( CLEANUP_QUERY, void, void, __VA_ARGS__ ))

#define CLEANUP_QUERY2(s, t, VAR1, CHECK) if (CHECK) { LUA->ThrowError(VAR1.message); return 0; }

#define CLEANUP_QUERY3(s, t, VAR1, VAR2, CHECK) if (CHECK) { bson_destroy(&VAR2); LUA->ThrowError(VAR1.message); return 0; }

#endif //GMSV_MONGODB_MACROS_HPP
