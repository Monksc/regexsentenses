//
//  regexexpression.h
//
//
//  Created by Cameron Monks on 06/09/2021
//  Copyright © 2021 cameronmonks. All rights reserved.
//

#ifndef regexexpression_h
#define regexexpression_h

#include "vector.h"

struct regexcharacter {
    char minc;
    char maxc;
    char currentc;
};

struct regexcharactergroup {
    struct vector characters;
};

struct regexoptions {
    struct vector groups;
    size_t index;
};

struct regexrange {
    struct regexoptions baseoption;
    struct vector options;
    unsigned minlen;
    unsigned maxlen;
};

struct regexexpression {
    struct vector ranges;
};


void regexexpression_init(struct regexexpression * self);
void regexrange_init(struct regexrange * self,
        struct regexrange baseoption, unsigned minlen, unsigned maxlen);
void regexoptions_init(struct regexoptions * self);
void regexcharactergroup_init(struct regexcharactergroup * self);

void regexexpression_deinit(struct regexexpression * self);
void regexrange_deinit(struct regexrange * self);
void regexoptions_deinit(struct regexoptions * self);
void regexcharactergroup_deinit(struct regexcharactergroup * self);

void regexexpression_copy(struct regexexpression * dst, const struct regexexpression * src);
void regexrange_copy(struct regexrange * dst, const struct regexrange * src);
void regexoptions_copy(struct regexoptions * dst, const struct regexoptions * src);
void regexcharactergroup_copy(struct regexcharactergroup * dst,
        const struct regexcharactergroup * src);
void regexcharacter_copy(struct regexcharacter * dst,
        const struct regexcharacter * src);


void regexexpression_push(struct regexexpression * self, struct regexrange item);
void regexrange_push(struct regexrange * self, struct regexoptions item);
void regexoptions_push(struct regexoptions * self, struct regexcharactergroup item);
void regexcharactergroup_push(struct regexcharactergroup * self, struct regexcharacter item);

void regexexpression_debug(struct regexexpression * self);
void regexrange_debug(struct regexrange * self);
void regexoptions_debug(struct regexoptions * self);
void regexcharactergroup_debug(struct regexcharactergroup * self);
void regexcharacter_debug(struct regexcharacter * self);

void regexexpression_print(struct regexexpression * self);
void regexrange_print(struct regexrange * self);
void regexoptions_print(struct regexoptions * self);
void regexcharactergroup_print(struct regexcharactergroup * self);
void regexcharacter_print(struct regexcharacter * self);

void regexexpression_reset(struct regexexpression * self);
void regexrange_reset(struct regexrange * self);
void regexoptions_reset(struct regexoptions * self);
void regexcharactergroup_reset(struct regexcharactergroup * self);
void regexcharacter_reset(struct regexcharacter * self);

int regexexpression_inc(struct regexexpression * self);
int regexrange_inc(struct regexrange * self);
int regexoptions_inc(struct regexoptions * self);
int regexcharactergroup_inc(struct regexcharactergroup * self);
int regexcharacter_inc(struct regexcharacter * self);


#endif /* regexexpression_h */


