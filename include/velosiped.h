#pragma once

#include <stdio.h>

extern int velo_errno;

int velo_puts(const char *s);

char *velo_strchr(const char *s, int c);

size_t velo_strlen(const char *s);

char *velo_strcpy(char *dst, const char *src);

char *velo_strncpy(char *dst, const char *src, size_t dsize);

char *velo_strcat(char *dst, const char *src);

char *velo_strncat(char *dst, const char *src, size_t ssize);

char *velo_fgets(char *s, int size, FILE *stream);

char *velo_strdup(const char *s);

ssize_t velo_getline(char **lineptr, size_t *n, FILE *stream);

char *velo_strstr(const char *haystack, const char *needle);

char *velo_strtok(char *str, const char *delim);
