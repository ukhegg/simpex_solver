#pragma once


// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define SIMPLEX_SOLVER_HELPER_DLL_IMPORT __declspec(dllimport)
#define SIMPLEX_SOLVER_HELPER_DLL_EXPORT __declspec(dllexport)
#define SIMPLEX_SOLVER_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define SIMPLEX_SOLVER_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define SIMPLEX_SOLVER_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define SIMPLEX_SOLVER_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define SIMPLEX_SOLVER_HELPER_DLL_IMPORT
#define SIMPLEX_SOLVER_HELPER_DLL_EXPORT
#define SIMPLEX_SOLVER_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define SIMPLEX_SOLVER_API and SIMPLEX_SOLVER_LOCAL.
// SIMPLEX_SOLVER_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// SIMPLEX_SOLVER_LOCAL is used for non-api symbols.

#ifdef SIMPLEX_SOLVER_DLL // defined if SIMPLEX_SOLVER is compiled as a DLL
#ifdef SIMPLEX_SOLVER_DLL_EXPORTS // defined if we are building the SIMPLEX_SOLVER DLL (instead of using it)
#define SIMPLEX_SOLVER_API SIMPLEX_SOLVER_HELPER_DLL_EXPORT
#else
#define SIMPLEX_SOLVER_API SIMPLEX_SOLVER_HELPER_DLL_IMPORT
#endif // SIMPLEX_SOLVER_DLL_EXPORTS
#define SIMPLEX_SOLVER_LOCAL SIMPLEX_SOLVER_HELPER_DLL_LOCAL
#else // SIMPLEX_SOLVER_DLL is not defined: this means SIMPLEX_SOLVER is a static lib.
#define SIMPLEX_SOLVER_API
#define SIMPLEX_SOLVER_LOCAL
#endif // SIMPLEX_SOLVER_DLL
