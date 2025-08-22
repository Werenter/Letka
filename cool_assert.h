#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef soft_assert
#undef soft_assert
#endif

#ifdef hard_assert
#undef hard_assert
#endif

#define stringify1(a) #a
#define stringify(a) stringify1(a)

#if defined(DEBUG) || defined(_DEBUG) 

#ifdef HARD_ASSERTS
#define soft_assert(COND, MESSAGE_FORMAT, ...) \
	do {\
		if(!(COND)) {\
			fputs("Soft assertion failed: " stringify(COND) ": " __FILE__ ":" stringify(__LINE__) " ", stderr); \
			fprintf(stderr, MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__); \
			exit(255); \
		} \
	}while(0)
#else
#define soft_assert(COND, MESSAGE_FORMAT, ...) \
	do {\
		if(!(COND)) {\
			fputs("Soft assertion failed: " stringify(COND) ": " __FILE__ ":" stringify(__LINE__) " ", stderr); \
			fprintf(stderr, MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__); \
		} \
	}while(0)
#endif

#define hard_assert(COND, MESSAGE_FORMAT, ...) \
	do {\
		if(!(COND)) {\
			fputs("Hard assertion failed: " stringify(COND) ": " __FILE__ ":" stringify(__LINE__) " ", stderr); \
			fprintf(stderr, MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__); \
			exit(255); \
		} \
	}while(0)

#else // DEBUG || _DEBUG

/**
 * This functions checks if value is equal or
 * very close to zero
 * 
 * @param [double] val Value for checking
 */
#define soft_assert(COND, MESSAGE_FORMAT, ...) (void)0

/**
 * This functions checks if value is equal or
 * very close to zero
 * 
 * @param [double] val Value for checking
 */
#define hard_assert(COND, MESSAGE_FORMAT, ...) (void)0
#endif // DEBUG || _DEBUG
