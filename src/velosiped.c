#include "velosiped.h"

#include <stdlib.h>
#include <assert.h>
#include <errno.h>

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

	bool flag = false;
	int i = 0;
	while(i < size-1) {
		int next = getc(stream);

		if(next == EOF) break;
		else flag = true;

		s[i++] = (char)next;

		if(next == '\n') break;
	}
	s[i] = '\0';
	if(flag) return s;
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
			velo_errno = ENOMEM;
			return -1;
		}

		*n = START_BUFF_SIZE;
	}

	size_t readed_characters = 0;
	while(1) {
		// 2 - place for next symbol and null character
		if(readed_characters+2 >= *n) {
			*lineptr = realloc(*lineptr, *n);
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
