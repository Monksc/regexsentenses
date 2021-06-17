//
//  vector.c
//
//
//  Created by Cameron Monks on 06/09/2021
//  Copyright © 2021 cameronmonks. All rights reserved.
//

#include "vector.h"

void vector_init(struct vector * self, size_t size) {
    self->size = size;
    self->len = 0;
    self->cap = 8;
    self->ptr = malloc(self->size * self->cap);
}

void vector_deinit_deep(struct vector * self, void (* deinit)(void *)) {
    for (size_t i = 0; i < self->len; i++) {
        deinit(vector_get(self, i));
    }

    vector_deinit(self);
}

void vector_deep_copy(struct vector * dst, const struct vector * src,
        void (*copyfunc)(void * dst, const void * src)) {
    dst->size = src->size;
    dst->len = src->len;
    dst->cap = src->cap;
    dst->ptr = malloc(src->cap * src->size);
    for (size_t i = 0; i < src->len; i++) {
        copyfunc(vector_get(dst, i), vector_get(src, i));
    }
}

void vector_set(struct vector * self, size_t index, void * ptr) {
    memcpy(vector_get(self, index), ptr, self->size);
}

void vector_push(struct vector * self, void * ptr) {
    if (self->len >= self->cap) {
        self->cap <<= 1;
        self->ptr = realloc(self->ptr, self->cap * self->size);
    }

    vector_set(self, self->len, ptr);

    self->len++;
}


