/**
 * File: util.c
 *
 */
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void Util_outOfMemory(const char *message)
{
    if(message != NULL) {
        fprintf(stderr, "Out of memory:\n%s\n", message);
    }
    else {
        fputs("Out of memory.\n", stderr);
    }
    abort();
}

void * Util_allocOrDie(size_t size, const char *message)
{
    void * res = malloc(size);
    if(res == NULL) {
        Util_outOfMemory(message);

        //Just in case.
        abort();
        return NULL;
    }
    return res;
}

void* Util_cloneOrDie(const void *pRhs, size_t size, const char *message)
{
    void * pLhs = Util_allocOrDie(size, message);
    //Just in case.
    if (pLhs == NULL) {
        abort();
        return NULL;
    }

    memcpy(pLhs, pRhs, size);
    return pLhs;
}




