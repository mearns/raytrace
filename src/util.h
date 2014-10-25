/**
 * File: util.h
 *
 */
#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

void Util_outOfMemory(const char *message);

void* Util_allocOrDie(size_t size, const char *message);

void* Util_cloneOrDie(const void *pRhs, size_t size, const char *message);

#endif
//end inclusion filter

