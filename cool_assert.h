#pragma once

#include <stdio.h>
#include <stdlib.h>

#define stringify1(a) #a
#define stringify(a) stringify1(a)

#ifndef NDEBUG

#ifdef HARD_ASSERTS
// TODO: #define soft_assert(...) hard_assert(__VA_ARGS__)
// TODO: maybe print current location in separate macro?
// TODO: it's better to call abort?
#define soft_assert(COND, MESSAGE_FORMAT, ...) \
	do {\
		if(!(COND)) {\
			fputs("Soft assertion failed: " stringify(COND) ": " __FILE__ ":" stringify(__LINE__) " ", stderr); \
			fprintf(stderr, MESSAGE_FORMAT __VA_OPT__(,) __VA_ARGS__); \
			abort(); \
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
			abort(); \
		} \
	}while(0)

#else // NDEBUG

/**
 * This macros works as assert, but it allows print
 * custom message. (soft version)
 *
 * Soft assert finish program only with flag -DHARD_ASSERTS
 * and asserts enabled (No NDEBUG flag)
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
 * Hard assert finish program always when asserts enabled (No NDEBUG flag)
 * 
 * @param [in] COND Conditition for checking
 * @param [in] MESSAGE_FORMAT Format for error message
 * @param [in] VA_ARGS - arguments for error message format
 */
#define hard_assert(COND, MESSAGE_FORMAT, ...) (void)0
#endif // NDEBUG
