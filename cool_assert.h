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
 * This macros works as assert, but it allows print
 * custom message. (soft version)
 *
 * Soft assert finish program only with flag -DHARD_ASSERTS
 * and asserts enabled (-DDEBUG)
 * 
 * @param [in] COND Conditition for checking
 * @param [in] MESSAGE_FORMAT Format for error message
 * @param [in] VA_ARGS - arguments for error message format
 */
#define soft_assert(COND, MESSAGE_FORMAT, ...) (void)0

/**
 * This macros works as assert, but it allows print
 * custom message. (hard version)
 *
 * Hard assert finish program always when asserts enabled (-DDEBUG)
 * 
 * @param [in] COND Conditition for checking
 * @param [in] MESSAGE_FORMAT Format for error message
 * @param [in] VA_ARGS - arguments for error message format
 */
#define hard_assert(COND, MESSAGE_FORMAT, ...) (void)0
#endif // DEBUG || _DEBUG
