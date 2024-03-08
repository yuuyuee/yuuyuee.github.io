# LuaJit 2.1

## About LuaJIT

* Use `luaL_newstate` instead of `lua_newstate`.
* Use `luaL_openlibs` instead of `lua_openbase`.
* To change or extend the lib of standard libraries
  to load, copy `src/lib_init.c` and modify it accordingly.

## Lua API

### Pseudo-indicess

```C
#define LUA_REGISTRYINDEX   (-10000)
#define LUA_ENVIRONINDEX    (-10001)
#define LUA_GLOBALSINDEX    (-10002)
#define lua_upvalueindex(i) (LUA_GLOBALSINDEX-(i))
```

### C functions interface

```C
typedef int (*lua_CFunction)(lua_State *L);
```

### State manipulation

```C
LUA_API lua_State* lua_newstate(lua_Alloc f, void *ud);
LUA_API void lua_close(lua_State *L);
LUA_API lua_State* lua_newthread(lua_State *L);
LUA_API lua_CFunction lua_atpanic(lua_State *L, lua_CFunction panicf);
```

### Basic stack manipulation

```C
LUA_API int lua_gettop(lua_State *L);
LUA_API void lua_settop(lua_State *L, int idx);
LUA_API void lua_pushvalue(lua_State *L, int idx);
LUA_API void lua_remove(lua_State *L, int idx);
LUA_API void lua_insert(lua_State *L, int idx);
LUA_API void lua_replace(lua_State *L, int idx);
LUA_API int lua_checkstack(lua_State *L, int sz);
LUA_API void lua_xmove(lua_State *from, lua_State *to, int n);
```

### Access functions (stack -> C)

```C
LUA_API int lua_isnumber(lua_State *L, int idx);
LUA_API int lua_isstring(lua_State *L, int idx);
LUA_API int lua_iscfunction(lua_State *L, int idx);
LUA_API int lua_isuserdata(lua_State *L, int idx);
LUA_API int lua_type(lua_State *L, int idx);
LUA_API const char* lua_typename(lua_State *L, int tp);

LUA_API int lua_equal(lua_State *L, int idx1, int idx2);
LUA_API int lua_rawequal(lua_State *L, int idx1, int idx2);
LUA_API int lua_lessthan(lua_State *L, int idx1, int idx2);

LUA_API lua_Number lua_tonumber(lua_State *L, int idx);
LUA_API lua_Integer lua_tointeger(lua_State *L, int idx);
LUA_API int lua_toboolean(lua_State *L, int idx);
LUA_API const char* lua_tolstring(lua_State *L, int idx, size_t *len);
LUA_API size_t lua_objlen(lua_State *L, int idx);
LUA_API lua_CFunction lua_tocfunction(lua_State *L, int idx);
LUA_API void* lua_touserdata(lua_State *L, int idx);
LUA_API lua_State* lua_tothread(lua_State *L, int idx);
LUA_API const void* lua_topointer(lua_State *L, int idx);
```

### Push functions (C -> stack)

```C
LUA_API void lua_pushnil(lua_State *L);
LUA_API void lua_pushnumber(lua_State *L, lua_Number n);
LUA_API void lua_pushinteger(lua_State *L, lua_Integer n);
LUA_API void lua_pushlstring(lua_State *L, const char *s, size_t l);
LUA_API void lua_pushstring(lua_State *L, const char *s);
LUA_API const char* lua_pushvfstring(lua_State *L, const char *fmt,
                                                      va_list argp);
LUA_API const char* lua_pushfstring(lua_State *L, const char *fmt, ...);
LUA_API void lua_pushcclosure(lua_State *L, lua_CFunction fn, int n);
LUA_API void lua_pushboolean(lua_State *L, int b);
LUA_API void lua_pushlightuserdata(lua_State *L, void *p);
LUA_API int lua_pushthread(lua_State *L);
```

### Get functions (Lua -> stack)

```C
LUA_API void lua_gettable(lua_State *L, int idx);
LUA_API void lua_getfield(lua_State *L, int idx, const char *k);
LUA_API void lua_rawget(lua_State *L, int idx);
LUA_API void lua_rawgeti(lua_State *L, int idx, int n);
LUA_API void lua_createtable(lua_State *L, int narr, int nrec);
LUA_API void* lua_newuserdata(lua_State *L, size_t sz);
LUA_API int lua_getmetatable(lua_State *L, int objindex);
LUA_API void lua_getfenv(lua_State *L, int idx);
```

### Set functions (stack -> Lua)

```C
LUA_API void lua_settable(lua_State *L, int idx);
//  [-1, +0, e]
// Does the equivalent to t[k] = v, where t is the value
// at the given index and v is the value at the top of the stack.
// This function pops the value from the stack. As in Lua,
// this function may trigger a metamethod for the "newindex" event.
LUA_API void lua_setfield(lua_State *L, int idx, const char *k);
LUA_API void lua_rawset(lua_State *L, int idx);
LUA_API void lua_rawseti(lua_State *L, int idx, int n);
LUA_API int lua_setmetatable(lua_State *L, int objindex);
LUA_API int lua_setfenv(lua_State *L, int idx);
```

### `load` and `call` functions (load and run Lua code)

```C
LUA_API void lua_call(lua_State *L, int nargs, int nresults);
LUA_API int lua_pcall(lua_State *L, int nargs, int nresults, int errfunc);
LUA_API int lua_cpcall(lua_State *L, lua_CFunction func, void *ud);
LUA_API int lua_load(lua_State *L, lua_Reader reader,
                     void *dt, const char *chunkname);
LUA_API int lua_dump(lua_State *L, lua_Writer writer, void *data);
```

### Coroutine functions

```C
LUA_API int lua_yield(lua_State *L, int nresults);
LUA_API int lua_resume(lua_State *L, int narg);
LUA_API int lua_status(lua_State *L);
```

### Garbage-collection function and options

```C
#define LUA_GCSTOP        0
#define LUA_GCRESTART     1
#define LUA_GCCOLLECT     2
#define LUA_GCCOUNT       3
#define LUA_GCCOUNTB      4
#define LUA_GCSTEP        5
#define LUA_GCSETPAUSE    6
#define LUA_GCSETSTEPMUL  7
#define LUA_GCISRUNNING   9

LUA_API int lua_gc(lua_State *L, int what, int data);
```

### Miscellaneous functions

```C
LUA_API int lua_error(lua_State *L);
LUA_API int lua_next(lua_State *L, int idx);
LUA_API void lua_concat(lua_State *L, int n);
LUA_API lua_Alloc lua_getallocf(lua_State *L, void **ud);
LUA_API void lua_setallocf(lua_State *L, lua_Alloc f, void *ud);
```

### some useful macros

```C
// [-n, +0, –]
// Pops n elements from the stack.
#define lua_pop(L,n) lua_settop(L, -(n)-1)
#define lua_newtable(L) lua_createtable(L, 0, 0)
#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))
#define lua_pushcfunction(L,f) lua_pushcclosure(L, (f), 0)
#define lua_strlen(L,i) lua_objlen(L, (i))
#define lua_isfunction(L,n) (lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n) (lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n) (lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n) (lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n) (lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n) (lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n) (lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n) (lua_type(L, (n)) <= 0)
#define lua_pushliteral(L, s) \
    lua_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)
#define lua_setglobal(L,s) lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s) lua_getfield(L, LUA_GLOBALSINDEX, (s))
#define lua_tostring(L,i) lua_tolstring(L, (i), NULL)

/////////////////////////////////////////////////////////////////////////
// The auxiliary macros
#define luaL_argcheck(L, cond,numarg,extramsg) \
    ((void)((cond) || luaL_argerror(L, (numarg), (extramsg))))
#define luaL_checkstring(L,n) (luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d) (luaL_optlstring(L, (n), (d), NULL))
#define luaL_checkint(L,n) ((int)luaL_checkinteger(L, (n)))
#define luaL_optint(L,n,d) ((int)luaL_optinteger(L, (n), (d)))
#define luaL_checklong(L,n) ((long)luaL_checkinteger(L, (n)))
#define luaL_optlong(L,n,d) ((long)luaL_optinteger(L, (n), (d)))

#define luaL_typename(L,i) lua_typename(L, lua_type(L,(i)))

#define luaL_dofile(L, fn) \
  (luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0))

//  [-0, +?, –]
// Loads and runs the given string.
// It returns false if there are no errors or true in case of errors.
#define luaL_dostring(L, s) \
  (luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define luaL_getmetatable(L,n) (lua_getfield(L, LUA_REGISTRYINDEX, (n)))

#define luaL_opt(L,f,n,d) (lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

/* From Lua 5.2. */
#define luaL_newlibtable(L, l) \
    lua_createtable(L, 0, sizeof(l)/sizeof((l)[0]) - 1)
#define luaL_newlib(L, l) (luaL_newlibtable(L, l), luaL_setfuncs(L, l, 0))
```

### Compatibility macros and functions

```C
#define lua_open() luaL_newstate()
#define lua_getregistry(L) lua_pushvalue(L, LUA_REGISTRYINDEX)
#define lua_getgccount(L) lua_gc(L, LUA_GCCOUNT, 0)
#define lua_Chunkreader lua_Reader
#define lua_Chunkwriter lua_Writer
```

### Hack

```C
LUA_API void lua_setlevel(lua_State *from, lua_State *to);
```

### Debug API

```C
// Event codes
#define LUA_HOOKCALL    0
#define LUA_HOOKRET     1
#define LUA_HOOKLINE    2
#define LUA_HOOKCOUNT   3
#define LUA_HOOKTAILRET 4

// Event masks
#define LUA_MASKCALL    (1 << LUA_HOOKCALL)
#define LUA_MASKRET     (1 << LUA_HOOKRET)
#define LUA_MASKLINE    (1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT   (1 << LUA_HOOKCOUNT)

typedef struct lua_Debug lua_Debug;  /* activation record */

/* Functions to be called by the debuger in specific events */
typedef void (*lua_Hook)(lua_State *L, lua_Debug *ar);

LUA_API int lua_getstack(lua_State *L, int level, lua_Debug *ar);
LUA_API int lua_getinfo(lua_State *L, const char *what, lua_Debug *ar);
LUA_API const char *lua_getlocal(lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *lua_setlocal(lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *lua_getupvalue(lua_State *L, int funcindex, int n);
LUA_API const char *lua_setupvalue(lua_State *L, int funcindex, int n);
LUA_API int lua_sethook(lua_State *L, lua_Hook func, int mask, int count);
LUA_API lua_Hook lua_gethook(lua_State *L);
LUA_API int lua_gethookmask(lua_State *L);
LUA_API int lua_gethookcount(lua_State *L);

/* From Lua 5.2. */
LUA_API void *lua_upvalueid(lua_State *L, int idx, int n);
LUA_API void lua_upvaluejoin(lua_State *L, int idx1, int n1, int idx2, int n2);
LUA_API int lua_loadx(lua_State *L, lua_Reader reader, void *dt,
                       const char *chunkname, const char *mode);
LUA_API const lua_Number *lua_version(lua_State *L);
LUA_API void lua_copy(lua_State *L, int fromidx, int toidx);
LUA_API lua_Number lua_tonumberx(lua_State *L, int idx, int *isnum);
LUA_API lua_Integer lua_tointegerx(lua_State *L, int idx, int *isnum);

/* From Lua 5.3. */
LUA_API int lua_isyieldable(lua_State *L);


struct lua_Debug {
  int event;
  const char *name;             /* (n) */
  const char *namewhat;         /* (n) `global', `local', `field', `method' */
  const char *what;             /* (S) `Lua', `C', `main', `tail' */
  const char *source;           /* (S) */
  int currentline;              /* (l) */
  int nups;                     /* (u) number of upvalues */
  int linedefined;              /* (S) */
  int lastlinedefined;          /* (S) */
  char short_src[LUA_IDSIZE];   /* (S) */
  /* private part */
  int i_ci;                     /* active function */
};
```

### Auxiliary function

```C
/* extra error code for `luaL_load' */
#define LUA_ERRFILE     (LUA_ERRERR+1)

typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;

LUALIB_API void luaL_openlib(lua_State *L, const char *libname,
                                const luaL_Reg *l, int nup);
LUALIB_API void luaL_register(lua_State *L, const char *libname,
                                const luaL_Reg *l);
LUALIB_API int luaL_getmetafield(lua_State *L, int obj, const char *e);
LUALIB_API int luaL_callmeta(lua_State *L, int obj, const char *e);
LUALIB_API int luaL_typerror(lua_State *L, int narg, const char *tname);
LUALIB_API int luaL_argerror(lua_State *L, int numarg, const char *extramsg);
LUALIB_API const char* luaL_checklstring(lua_State *L, int numArg,
                                                          size_t *l);
LUALIB_API const char* luaL_optlstring(lua_State *L, int numArg,
                                          const char *def, size_t *l);
LUALIB_API lua_Number luaL_checknumber(lua_State *L, int numArg);
LUALIB_API lua_Number luaL_optnumber(lua_State *L, int nArg, lua_Number def);
// [-0, +0, v]
// Checks whether the function argument arg is an integer
// (or can be converted to an integer) and returns this integer
// cast to a lua_Integer.
LUALIB_API lua_Integer luaL_checkinteger(lua_State *L, int numArg);
LUALIB_API lua_Integer luaL_optinteger(lua_State *L, int nArg,
                                      lua_Integer def);

LUALIB_API void luaL_checkstack(lua_State *L, int sz, const char *msg);
LUALIB_API void luaL_checktype(lua_State *L, int narg, int t);
LUALIB_API void luaL_checkany(lua_State *L, int narg);

LUALIB_API int luaL_newmetatable(lua_State *L, const char *tname);
LUALIB_API void* luaL_checkudata(lua_State *L, int ud, const char *tname);

LUALIB_API void luaL_where(lua_State *L, int lvl);
LUALIB_API int luaL_error(lua_State *L, const char *fmt, ...);

LUALIB_API int luaL_checkoption(lua_State *L, int narg, const char *def,
                                const char *const lst[]);

/* pre-defined references */
#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)

LUALIB_API int luaL_ref(lua_State *L, int t);
LUALIB_API void luaL_unref(lua_State *L, int t, int ref);

LUALIB_API int luaL_loadfile(lua_State *L, const char *filename);
LUALIB_API int luaL_loadbuffer(lua_State *L, const char *buff, size_t sz,
                              const char *name);
LUALIB_API int luaL_loadstring(lua_State *L, const char *s);

LUALIB_API lua_State* luaL_newstate(void);
LUALIB_API const char* luaL_gsub(lua_State *L, const char *s,
                                  const char *p, const char *r);

LUALIB_API const char* luaL_findtable(lua_State *L, int idx,
                                      const char *fname, int szhint);

/* From Lua 5.2. */
LUALIB_API int luaL_fileresult(lua_State *L, int stat, const char *fname);
LUALIB_API int luaL_execresult(lua_State *L, int stat);
LUALIB_API int luaL_loadfilex(lua_State *L, const char *filename,
                              const char *mode);
LUALIB_API int luaL_loadbufferx(lua_State *L, const char *buff, size_t sz,
                                const char *name, const char *mode);
LUALIB_API void luaL_traceback(lua_State *L, lua_State *L1, const char *msg,
                                int level);
LUALIB_API void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup);
LUALIB_API void luaL_pushmodule(lua_State *L, const char *modname,
                                int sizehint);
LUALIB_API void* luaL_testudata(lua_State *L, int ud, const char *tname);
LUALIB_API void luaL_setmetatable(lua_State *L, const char *tname);
```

### Generic Buffer manipulation

```C
typedef struct luaL_Buffer {
  char *p;          /* current position in buffer */
  int lvl;          /* number of strings in the stack (level) */
  lua_State *L;
  char buffer[LUAL_BUFFERSIZE];
} luaL_Buffer;

#define luaL_addchar(B,c) \
  ((void)((B)->p < ((B)->buffer+LUAL_BUFFERSIZE) || \
  luaL_prepbuffer(B)), (*(B)->p++ = (char)(c)))

/* compatibility only */
#define luaL_putchar(B,c) luaL_addchar(B,c)
#define luaL_addsize(B,n) ((B)->p += (n))

LUALIB_API void luaL_buffinit(lua_State *L, luaL_Buffer *B);
LUALIB_API char* luaL_prepbuffer(luaL_Buffer *B);
LUALIB_API void luaL_addlstring(luaL_Buffer *B, const char *s, size_t l);
LUALIB_API void luaL_addstring(luaL_Buffer *B, const char *s);
LUALIB_API void luaL_addvalue(luaL_Buffer *B);
LUALIB_API void luaL_pushresult(luaL_Buffer *B);
```
