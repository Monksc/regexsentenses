//
//  vector.h
//
//
//  Created by Cameron Monks on 06/09/2021
//  Copyright © 2021 cameronmonks. All rights reserved.
//

#ifndef vector_h
#define vector_h

#include <stdlib.h>
#include <string.h>

struct vector {
    void * ptr;
    size_t size;
    size_t len;
    size_t cap;
};

void vector_init(struct vector * self, size_t size);
#define vector_deinit(self) free((self)->ptr)
void vector_deinit_deep(struct vector * self, void (* deinit)(void *));

void vector_deep_copy(struct vector * dst, const struct vector * src,
        void (*copyfunc)(void * dst, const void * src));

#define vector_get(self, index) ((void *)(((char *)(self)->ptr) + (self)->size * index))

void vector_set(struct vector * self, size_t index, void * ptr);
void vector_push(struct vector * self, void * ptr);

#endif /* vector_h */


