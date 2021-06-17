//
//  regexexpression.c
//
//
//  Created by Cameron Monks on 06/09/2021
//  Copyright © 2021 cameronmonks. All rights reserved.
//

#include <stdio.h>
#include "regexexpression.h"

void regexexpression_init(struct regexexpression * self) {
    vector_init(&self->ranges, sizeof(struct regexrange));
}
void regexrange_init(struct regexrange * self, struct regexrange baseoption,
        unsigned minlen, unsigned maxlen) {
    vector_init(&self->options, sizeof(struct regexoptions));
    self->minlen = minlen;
    self->maxlen = maxlen;
}
void regexoptions_init(struct regexoptions * self) {
    vector_init(&self->groups, sizeof(struct regexcharactergroup));
    self->index = 0;
}
void regexcharactergroup_init(struct regexcharactergroup * self) {
    vector_init(&self->characters, sizeof(struct regexcharacter));
}

void regexexpression_deinit(struct regexexpression * self) {
    vector_deinit_deep(&self->ranges, (void (*)(void *)) regexrange_deinit);
}
void regexrange_deinit(struct regexrange * self) {
    vector_deinit_deep(&self->options, (void (*)(void *)) regexoptions_deinit);
    regexoptions_deinit(&self->baseoption);
}
void regexoptions_deinit(struct regexoptions * self) {
    vector_deinit_deep(&self->groups, (void (*)(void *)) regexcharactergroup_deinit);
}
void regexcharactergroup_deinit(struct regexcharactergroup * self) {
    vector_deinit(&self->characters);
}

void regexexpression_copy(struct regexexpression * dst, const struct regexexpression * src) {
    vector_deep_copy(&dst->ranges, &src->ranges,
            (void (*) (void*, const void*)) regexrange_copy);
}
void regexrange_copy(struct regexrange * dst, const struct regexrange * src) {
    regexoptions_copy(&dst->baseoption, &src->baseoption);
    vector_deep_copy(&dst->options, &src->options,
            (void (*) (void*, const void*)) regexoptions_copy);
    dst->minlen = src->minlen;
    dst->maxlen = src->maxlen;
}
void regexoptions_copy(struct regexoptions * dst, const struct regexoptions * src) {
    vector_deep_copy(&dst->groups, &src->groups,
            (void (*) (void*, const void*)) regexcharactergroup_copy);
    dst->index = src->index;
}
void regexcharactergroup_copy(struct regexcharactergroup * dst,
        const struct regexcharactergroup * src) {
    vector_deep_copy(&dst->characters, &src->characters,
            (void (*) (void*, const void*)) regexcharacter_copy);
}
void regexcharacter_copy(struct regexcharacter * dst,
        const struct regexcharacter * src) {
    dst->minc = src->minc;
    dst->maxc = src->maxc;
    dst->currentc = src->currentc;
}


void regexexpression_push(struct regexexpression * self, struct regexrange item) {
    vector_push(&self->ranges, &item);
}
void regexrange_push(struct regexrange * self, struct regexoptions item) {
    vector_push(&self->options, &item);
}
void regexoptions_push(struct regexoptions * self, struct regexcharactergroup item) {
    vector_push(&self->groups, &item);
}
void regexcharactergroup_push(struct regexcharactergroup * self, struct regexcharacter item) {
    vector_push(&self->characters, &item);
}

void regexexpression_debug(struct regexexpression * self) {
    printf("START EXPRESSION\n");
    for (size_t i = 0; i < self->ranges.len; i++) {
        regexrange_debug(vector_get(&self->ranges, i));
    }
    printf("END   EXPRESSION\n");
}
void regexrange_debug(struct regexrange * self) {
    printf("\tLEN: MIN: %d MAX: %d\n", self->minlen, self->maxlen);
    regexoptions_debug(&self->baseoption);
}
void regexoptions_debug(struct regexoptions * self) {
    printf("\tSTART OPTIONS\n");
    for (size_t i = 0; i < self->groups.len; i++) {
        regexcharactergroup_debug(vector_get(&self->groups, i));
    }
    printf("\tEND   OPTIONS\n");
}
void regexcharactergroup_debug(struct regexcharactergroup * self) {
    printf("\t\tSTART CHARACTER GROUP\n");
    for (size_t i = 0; i < self->characters.len; i++) {
        regexcharacter_debug(vector_get(&self->characters, i));
    }
    printf("\t\tEND   CHARACTER GROUP\n");
}
void regexcharacter_debug(struct regexcharacter * self) {
    printf("\t\t\tMINC: %c, MAXC: %c\n", self->minc, self->maxc);
}


void regexexpression_print(struct regexexpression * self) {
    for (size_t i = 0; i < self->ranges.len; i++) {
        regexrange_print(vector_get(&self->ranges, i));
    }
}
void regexrange_print(struct regexrange * self) {
    for (size_t i = 0; i < self->options.len; i++) {
        regexoptions_print(vector_get(&self->options, i));
    }
}
void regexoptions_print(struct regexoptions * self) {
    regexcharactergroup_print(vector_get(&self->groups, self->index));
}
void regexcharactergroup_print(struct regexcharactergroup * self) {
    for (size_t i = 0; i < self->characters.len; i++) {
        regexcharacter_print(vector_get(&self->characters, i));
    }
}
void regexcharacter_print(struct regexcharacter * self) {
    printf("%c", self->currentc);
}


void regexexpression_reset(struct regexexpression * self) {
    for (size_t i = 0; i < self->ranges.len; i++) {
        regexrange_reset(vector_get(&self->ranges, i));
    }
}
void regexrange_reset(struct regexrange * self) {
    vector_deinit_deep(&self->options, (void (*)(void *)) regexoptions_deinit);
    vector_init(&self->options, sizeof(struct regexoptions));
    regexoptions_reset(&self->baseoption);
    for (size_t i = 0; i < self->minlen; i++) {
        struct regexoptions copy;
        regexoptions_copy(&copy, &self->baseoption);
        vector_push(&self->options, &copy);
    }
}
void regexoptions_reset(struct regexoptions * self) {
    for (size_t i = 0; i < self->groups.len; i++) {
        regexcharactergroup_reset(vector_get(&self->groups, i));
    }
    self->index = 0;
}
void regexcharactergroup_reset(struct regexcharactergroup * self) {
    for (size_t i = 0; i < self->characters.len; i++) {
        regexcharacter_reset(vector_get(&self->characters, i));
    }
}
void regexcharacter_reset(struct regexcharacter * self) {
    self->currentc = self->minc;
}

int regexexpression_inc(struct regexexpression * self) {
    for (int i = self->ranges.len - 1; i >= 0; i--) {
        if (!regexrange_inc(vector_get(&self->ranges, i))) {
            return 0;
        }
    }
    return 1;
}
int regexrange_inc(struct regexrange * self) {
    for (int i = self->options.len - 1; i >= 0; i--) {
        if (!regexoptions_inc(vector_get(&self->options, i))) {
            return 0;
        }
    }

    if (self->options.len >= self->maxlen) {
        regexrange_reset(self);
        return 1;
    }

    struct regexoptions copy;
    regexoptions_copy(&copy, &self->baseoption);
    vector_push(&self->options, &copy);

    return 0;
}
int regexoptions_inc(struct regexoptions * self) {
    if (!regexcharactergroup_inc(vector_get(&self->groups, self->index))) {
        return 0;
    }

    self->index++;
    if (self->index >= self->groups.len) {
        self->index = 0;
        return 1;
    }
    return 0;
}
int regexcharactergroup_inc(struct regexcharactergroup * self) {
    for (int i = self->characters.len - 1; i >= 0; i--) {
        if (!regexcharacter_inc(vector_get(&self->characters, i))) {
            return 0;
        }
    }
    return 1;
}
int regexcharacter_inc(struct regexcharacter * self) {
    self->currentc++;
    int r = self->currentc > self->maxc;
    if (r) {
        self->currentc = self->minc;
    }
    return r;
}

