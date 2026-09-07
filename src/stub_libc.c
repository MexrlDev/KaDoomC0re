#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

// Define missing types
typedef long ssize_t;
typedef long off_t;

// ----- Memory / string functions -----
void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}
void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}
void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    if (d < s) {
        while (n--) *d++ = *s++;
    } else {
        d += n; s += n;
        while (n--) *--d = *--s;
    }
    return dest;
}
size_t strlen(const char *s) {
    size_t n = 0;
    while (*s++) n++;
    return n;
}
int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *(unsigned char*)a - *(unsigned char*)b;
}
int strncmp(const char *a, const char *b, size_t n) {
    while (n-- && *a && *a == *b) { a++; b++; }
    if (!n) return 0;
    return *(unsigned char*)a - *(unsigned char*)b;
}
char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}
char *strncpy(char *dest, const char *src, size_t n) {
    char *d = dest;
    while (n-- && (*d++ = *src++));
    return dest;
}
char *strchr(const char *s, int c) {
    while (*s) {
        if (*s == c) return (char*)s;
        s++;
    }
    return NULL;
}
char *strrchr(const char *s, int c) {
    const char *last = NULL;
    while (*s) {
        if (*s == c) last = s;
        s++;
    }
    return (char*)last;
}
char *strstr(const char *haystack, const char *needle) {
    for (; *haystack; haystack++) {
        const char *h = haystack, *n = needle;
        while (*h && *n && *h == *n) { h++; n++; }
        if (!*n) return (char*)haystack;
    }
    return NULL;
}
int strcasecmp(const char *a, const char *b) {
    while (*a && *b && (*a | 0x20) == (*b | 0x20)) { a++; b++; }
    return (*a | 0x20) - (*b | 0x20);
}
int strncasecmp(const char *a, const char *b, size_t n) {
    while (n-- && *a && *b && (*a | 0x20) == (*b | 0x20)) { a++; b++; }
    if (!n) return 0;
    return (*a | 0x20) - (*b | 0x20);
}
char *strdup(const char *s) { return NULL; }

// ----- stdio stubs (no-ops) -----
int printf(const char *fmt, ...) { return 0; }
int puts(const char *s) { return 0; }
void exit(int status) { while (1); }
void *malloc(size_t size) { return NULL; }
void free(void *ptr) {}
void *calloc(size_t nmemb, size_t size) { return NULL; }
void *realloc(void *ptr, size_t size) { return NULL; }
int getchar(void) { return EOF; }
int feof(FILE *stream) { return 0; }
int fgetc(FILE *stream) { return EOF; }
int ungetc(int c, FILE *stream) { return c; }
FILE *fopen(const char *path, const char *mode) { return NULL; }
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) { return 0; }
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) { return 0; }
int fclose(FILE *stream) { return 0; }
int fseek(FILE *stream, long offset, int whence) { return 0; }
long ftell(FILE *stream) { return 0; }
int remove(const char *path) { return 0; }
int rename(const char *old, const char *new) { return 0; }
int mkdir(const char *path, int mode) { return 0; }
void *opendir(const char *path) { return NULL; }
void *readdir(void *dir) { return NULL; }
int closedir(void *dir) { return 0; }
int stat(const char *path, void *buf) { return 0; }
char *getenv(const char *name) { return NULL; }
int __errno_location(void) { static int err; return err; }
void *localeconv(void) { static void *dummy; return dummy; }
double strtod(const char *nptr, char **endptr) { return 0.0; }
long strtol(const char *nptr, char **endptr, int base) { return 0; }
int __isoc99_sscanf(const char *str, const char *format, ...) { return 0; }
int __isoc99_fscanf(FILE *stream, const char *format, ...) { return 0; }
int __printf_chk(int flag, const char *format, ...) { return 0; }
int __fprintf_chk(FILE *stream, int flag, const char *format, ...) { return 0; }
int __vfprintf_chk(FILE *stream, int flag, const char *format, va_list ap) { return 0; }
int __vsnprintf_chk(char *str, size_t size, int flag, size_t slen, const char *format, va_list ap) { return 0; }
size_t __fread_chk(void *ptr, size_t ptrlen, size_t size, size_t nmemb, FILE *stream) { return 0; }
void *__memset_chk(void *dest, int c, size_t len, size_t destlen) { return memset(dest, c, len); }
char *__strncpy_chk(char *dest, const char *src, size_t n, size_t destlen) { return strncpy(dest, src, n); }
unsigned short *__ctype_b_loc(void) { static unsigned short ctype[256]; return ctype; }
unsigned short *__ctype_toupper_loc(void) { static unsigned short toupper[256]; return toupper; }
unsigned short *__ctype_tolower_loc(void) { static unsigned short tolower[256]; return tolower; }
int __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function) { while (1); }

// ----- POSIX-like stubs -----
int open(const char *path, int flags, ...) { return -1; }
int close(int fd) { return 0; }
ssize_t read(int fd, void *buf, size_t count) { return -1; }
ssize_t write(int fd, const void *buf, size_t count) { return count; }
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) { return NULL; }
int munmap(void *addr, size_t len) { return 0; }
off_t lseek(int fd, off_t offset, int whence) { return 0; }
char *strerror(int errnum) { return "Unknown error"; }
void perror(const char *s) {}
