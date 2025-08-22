#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef soft_assert
#undef soft_assert
#endif

#ifdef hard_assert
#undef hard_assert
#endif

#if defined(DEBUG) || defined(_DEBUG) 

#ifdef HARD_ASSERTS
#define soft_assert(COND, MESSAGE) {if(!(COND)) {fputs(MESSAGE, stderr); fputc('\n', stderr); exit(255);}}
#else
#define soft_assert(COND, MESSAGE) {if(!(COND)) {fputs(MESSAGE, stderr); fputc('\n', stderr);}}
#endif

#define hard_assert(COND, MESSAGE) {if(!(COND)) {fputs(MESSAGE, stderr); fputc('\n', stderr); exit(255);}}

#else // DEBUG || _DEBUG

#define soft_assert(COND, MESSAGE) {if(!(COND)) {fputs(MESSAGE, stderr); fputc('\n', stderr);}}
#define hard_assert(COND, MESSAGE) {if(!(COND)) {fputs(MESSAGE, stderr); fputc('\n', stderr);}}

#endif // DEBUG || _DEBUG
