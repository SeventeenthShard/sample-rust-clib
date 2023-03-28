#pragma once

#include <stdlib.h>
#include <stdint.h>

#ifdef __GNUC__
#define MYLIB_API 
#else
#ifdef MYLIB_LIBRARY_EXPORT
#define MYLIB_API __declspec(dllexport)
#else
#define MYLIB_API __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define MYLIB_API_VERSION_MAJOR 0
#define MYLIB_API_VERSION_MINOR 1
#define MYLIB_API_VERSION_POINT 1

typedef void * MyLibHandle;
typedef void * MyLibBuffer;
typedef uint8_t MyLibBool;


//
// Enums
//
// NOTE: For interop reasons, _NEVER_ reassign an enum value, only add new ones.


typedef enum MyLibArrayDatatype {
	// Floating point types
	MYLIB_ADT_FLOAT8  = 1,
	MYLIB_ADT_FLOAT16 = 2,
	MYLIB_ADT_FLOAT32 = 3,
	MYLIB_ADT_FLOAT64 = 4,
	// Integer types
	MYLIB_ADT_INT8    = 9,
	MYLIB_ADT_UINT8   = 10,
	MYLIB_ADT_INT16   = 11,
	MYLIB_ADT_UINT16  = 12,
	MYLIB_ADT_INT32   = 13,
	MYLIB_ADT_UINT32  = 14,
	MYLIB_ADT_INT64   = 15,
	MYLIB_ADT_UINT64  = 16,
	// Misc types
	MYLIB_ADT_BOOL    = 23,
	MYLIB_ADT_STRING  = 24,
	MYLIB_ADT_OPAQUE  = 25
} MyLibArrayDatatype;

//
// Errors
//

typedef int64_t MyLibStatus;

#define MYLIB_OK 0
#define MYLIB_ERR_MISC               1
#define MYLIB_ERR_INVALID_ARGUMENT   2
#define MYLIB_ERR_INVALID_CALL_ORDER 3



//
// Types and constants
//
typedef int64_t MYLIB_INT;

#define MYLIB_NIL ((void *)0)

//
// Structs
//

typedef struct
{
	size_t entries_num;
	const char * * keys;
	const char * * values;
	// Reserving space for future minor versions
	uint8_t reserved[64];
} MyLibSettings;


// MyLib follows semver 2.0, it is suggested libraries implementing MyLib do likewise
typedef struct
{
	MYLIB_INT api_version_major;
	MYLIB_INT api_version_minor;
	MYLIB_INT api_version_point;
	const char * library_name;
	const char * library_version;
	const char * library_description;
	// Reserving space for future minor versions
	uint8_t reserved[64];
} MyLibLibraryInfo;


typedef struct
{
	MyLibHandle opaque_handle;

	// Reserving space for future minor versions
	uint8_t reserved[64];
} MyLibSession;


typedef struct
{
	MyLibHandle opaque_handle;

	const char *        name;
	MyLibArrayDatatype datatype;
	size_t              shape[8];
	size_t              stride[8];
	MyLibBuffer         buffer;

	// Reserving space for future minor versions
	uint8_t reserved[64];
} MyLibArray;



//
// Lifecycle-related
//


// No other library function (except `MyLibGetLibraryInfo`) may be called until this is called and returns MYLIB_OK. 
// If it does not return MYLIB_OK the library is in an indeterminent state and must not be used.
MYLIB_API MyLibStatus MyLibInitialize(const MyLibSettings * library_settings);


// No other library function can be called after this (even if this fails).
MYLIB_API MyLibStatus MyLibFinalize();


//
// Introspection-related
//


// Call this to get library version information, etc. Semver 2.0 semantics should be used by the caller to determine
// if the callee library is compatible with the caller prior to initialization.
MYLIB_API MyLibStatus MyLibGetLibraryInfo(MyLibLibraryInfo * out_library_info);




// Get a description of the last error, if any.
MYLIB_API const char * MyLibGetLastErrorStr();



//
// Session-related
//


// Call this before performing any inference operations
MYLIB_API MyLibStatus MyLibAcquireSession(const MyLibSettings * session_settings, MyLibSession * out_session);


// Call this after all inference operations in a session are complete
MYLIB_API MyLibStatus MyLibReleaseSession(MyLibSession * out_session);




#ifdef __cplusplus
}
#endif
