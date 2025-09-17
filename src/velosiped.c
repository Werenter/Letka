#include "sq_typedefs.h"
#include "velosiped.h"

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

int velo_errno = 0;

int velo_puts(const char *s) {
	assert(s != NULL);

	while(*s != '\0') {
		if(putchar(*s++) == EOF) return EOF;
	}
	return 0;
}

// Костыль с кастами для обхода ворнинга
char *velo_strchr(const char *s, int c) {
	assert(s != NULL);

	do {
		if(*s == c) return (char *)(size_t)s;
		s++;
		if(*s == c) return (char *)(size_t)s;
	} while(*s != '\0');
	return NULL;
}

size_t velo_strlen(const char *s) {
	assert(s != NULL);

	size_t i = 0;
	while(s[i] != '\0') i++;
	return i;
}

char *velo_strcpy(char *dst, const char *src) {
	assert(dst != NULL);
	assert(src != NULL);

	size_t i = 0;
	do {
		dst[i] = src[i];
	} while(dst[i++] != '\0');
	return dst;
}

char *velo_strncpy(char *dst, const char *src, size_t dsize) {
	assert(dst != NULL);
	assert(src != NULL);

	if(dsize == 0) return dst;

	size_t i = 0;
	while(1) {
		dst[i] = src[i];
		if(src[i] == '\0' || i >= dsize) break;
		i++;
	}

	for(; i <= dsize; i++) {
		dst[i] = '\0';
	}
	return dst;
}

char *velo_strcat(char *dst, const char *src) {
	size_t n = velo_strlen(src);
	size_t movement = velo_strlen(dst);
	for(size_t i = 0; i <= n; i++) {
		dst[i+movement] = src[i];
	}
	return dst;
}

char *velo_strncat(char *dst, const char *src, size_t ssize) {
	size_t movement = velo_strlen(dst);
	size_t i = 0;
	for(; i < ssize; i++) {
		dst[i+movement] = src[i];
		if(src[i] == '\0') break;
	}
	dst[i+movement] = '\0';
	return dst;
}

char *velo_fgets(char *s, int size, FILE *stream) {
	assert(s != NULL);
	assert(stream != NULL);

	if(size == 0) return NULL;

	bool is_empty = true;
	int i = 0;
	while(i < size-1) {
		int next = getc(stream);

		if(next == EOF) break;
		else is_empty = false;

		s[i++] = (char)next;

		if(next == '\n') break;
	}
	s[i] = '\0';
	if(!is_empty) return s;
	else return NULL;
}

char *velo_strdup(const char *s) {
	assert(s!= NULL);

	char *out = malloc(velo_strlen(s)+1);
	if(out == NULL) {
		velo_errno = ENOMEM;
		return NULL;
	}

	velo_strcpy(out, s);
	return out;
}

#define START_BUFF_SIZE 128

ssize_t velo_getline(char **lineptr, size_t *n, FILE *stream) {
	if(lineptr == NULL || n == NULL || stream == NULL) {
		velo_errno = EINVAL;
		return -1;
	}

	if(*lineptr == NULL) {
		*lineptr = calloc(START_BUFF_SIZE, sizeof(char));
		if(*lineptr == NULL) {
			UNUSED(errno);
			velo_errno = ENOMEM;
			return -1;
		}

		*n = START_BUFF_SIZE;
	}

	size_t readed_characters = 0;
	while(1) {
		// 2 - place for next symbol and null character
		if(readed_characters+2 >= *n) {
			*lineptr = realloc(*lineptr, *n*2);
			*n *= 2; // TODO: fix memleak
			if(*lineptr == NULL) {
				velo_errno = ENOMEM;
				return -1;
			}
		}

		int next = getc(stream);
		if(next == EOF) return -1;

		*lineptr[readed_characters++] = (char)next;
		if(next == '\n') break;
	}
	*lineptr[readed_characters] = '\0';
	return (ssize_t)(readed_characters+1);
}


// Реализация полиномиального хеша
static const uint64_t q = 0x7fffffff;
static uint64_t x = 0;
static uint64_t precomputedX = 1;

static void polynomial_hash_init(size_t elements_count) {
	srand((unsigned int)time(NULL));
	x = ((uint64_t)rand())%q;
	precomputedX = 1;

	for(size_t i = 0; i < elements_count-1; i++) {
		precomputedX *= x;
		precomputedX = (precomputedX)%q;
	}
	UNUSED(errno);
}


static void hash_add_to_end_and_delete_from_begin(uint64_t *current_hash, char new_char, char old_char) {
	uint64_t deleting_val = ((unsigned char)old_char*precomputedX)%q;
	if(deleting_val <= *current_hash) *current_hash -= deleting_val;
	else                              *current_hash = *current_hash+q-deleting_val;

	*current_hash = (*current_hash * x                      )%q;
	*current_hash = (*current_hash + (unsigned char)new_char)%q;
}

static void hash_add(uint64_t *current_hash, char new_char) {
	*current_hash = (*current_hash * x)%q;
	*current_hash = (*current_hash + (unsigned char)new_char)%q;
}

char *velo_strstr(const char *haystack, const char *needle) {
	size_t hsize = strlen(haystack);
	size_t nsize = strlen(needle);

	if(nsize > hsize) return 0;
	if(nsize == 0) return (char*)(size_t)haystack;
	if(nsize == hsize) {
		if(strcmp(haystack, needle) == 0) return (char*)(size_t)haystack;
		else return NULL;
	}

	polynomial_hash_init(nsize);

	uint64_t target_hash = 0;
	for(size_t i = 0; i < nsize; i++) hash_add(&target_hash, needle[i]);

	uint64_t current_hash = 0;
	for(size_t i = 0; i < nsize; i++) hash_add(&current_hash, haystack[i]);

	for(size_t i = 0; i+nsize <= hsize; i++) {
		if(target_hash == current_hash) {
			bool is_good = true;
//			for(size_t j = 0; j < nsize && i+j < hsize; j++) {
//				if(haystack[i+j] != needle[j]) {
//					is_good = false;
//					break;
//				}
//			}
			if(is_good) return (char*)(size_t)(haystack+i);
		}
		hash_add_to_end_and_delete_from_begin(&current_hash, haystack[i+nsize], haystack[i]);
	}
	return NULL;
}

/*
static void z_function(const char *str, size_t *z_array, size_t str_size);

// TODO: КОПИПАСТА
static void z_function(const char *str, size_t *z_array, size_t str_size) {
//	memset(z_array, 0, str_size);

	for (size_t i = 1, l = 0, r = 0; i < str_size; i++) {
		if (i <= r) {                            //если i входит в уже обработанный отрезок
			if(z_array[i-l] < r-i+1) z_array[i] = z_array[i-l];
			else z_array[i] = r-i+1;
		}

		//иначе начальным значением z[i] остаётся 0

		//пытаемся увеличить z[i] наивным алгоритмом
		while (i + z_array[i] < str_size) {
			if (str[i + z_array[i]] == str[z_array[i]]) {
				z_array[i]++;
			} else {
				break;
			}
		}

		//если мы можем увеличить r, делаем это
		if (z_array[i] > 0 && i + z_array[i] - 1 > r) {
			l = i;
			r = i + z_array[i] - 1;
		}
	}
}

char *velo_strstr(const char *haystack, const char *needle) {
	size_t hsize = strlen(haystack);
	size_t nsize = strlen(needle);

	if(nsize > hsize) return 0;
	if(nsize == 0) return (char*)(size_t)haystack;
	if(nsize == hsize) {
		if(strcmp(haystack, needle) == 0) return (char*)(size_t)haystack;
		else return NULL;
	}

	char *zstring = calloc(hsize+nsize+2, sizeof(char));
	size_t *z_array = calloc(hsize+nsize+2, sizeof(size_t));
	strcpy(zstring, needle);
	strcat(zstring, "\x31");
	strcat(zstring, haystack);

	z_function(zstring, z_array, hsize+nsize+2);

	char *result = NULL;
	for(size_t i = 0; i+nsize <= hsize; i++) {
		if(z_array[nsize+i+1] == nsize) {
			result = (char*)(size_t)haystack+i;
		}
	}
	free(zstring);
	free(z_array);
	return result;
}
*/


char *velo_strtok(char *str, const char *delim) {
	assert(delim != NULL);

	static char *last_pos = NULL;

	if(last_pos == NULL && str == NULL) return NULL;

	if(str != NULL) {
		last_pos = str;

		while(*last_pos != '\0' && strchr(delim, *last_pos) != NULL) {
			last_pos++;
		}

		if(*last_pos == '\0') {
			last_pos = NULL;
			return NULL;
		}
	}

	char *begin_pos = last_pos;
	bool delim_found = false;
	for(size_t i = 0;; i++) {
		if(begin_pos[i] == '\0') {
			last_pos = NULL;
			return begin_pos;
		} else if(strchr(delim, begin_pos[i]) != NULL) {
			begin_pos[i] = '\0';
			delim_found = true;
		} else {
			if(delim_found) {
				last_pos = begin_pos+i;
				return begin_pos;
			}
		}
	}
}
