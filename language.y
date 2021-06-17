%code requires {
    int yylex();
    #include <stdio.h>
    #include <stdlib.h>
    #include <strings.h>
    #include <ctype.h>
    #include <unistd.h>
    #include "regexexpression.h"
    #include "vector.h"
    typedef struct regexexpression returnstruct;
    void yyerror(returnstruct * r, char *s);
    int maxcharactercount;
}

%union { int num; int range[2]; char c; struct regexexpression rexpression; struct regexrange rrange; struct regexoptions roptions; struct regexcharactergroup rcgroup; struct regexcharacter rcharacter; struct vector list; }
%start begin
%token leftbox
%token rightbox
%token leftparanthesis
%token rightparanthesis
%token dot
%token <range> rangetoken
%token <c> character
%type  <rexpression> parseexpresssion
%type  <list> parseexpresssionmid
%type  <rrange> parserange
%type  <roptions> parseoption
%type  <list> parseoptionmid
%type  <rcgroup> parsecharactergroup
%type  <list> parsecharactergroupmid
%type  <rcharacter> parsecharacter

%parse-param {returnstruct *r}

%%

begin           : parseexpresssion { *r = $1; return 0; }
                ;

parseexpresssion    : parseexpresssionmid { struct regexexpression g; g.ranges = $1; $$ = g; }
                    ;

parseexpresssionmid : parseexpresssionmid parserange { vector_push(&$1, &$2); $$ = $1; }
                    | parserange                     {
                                                        struct vector v;
                                                        vector_init(&v, sizeof(struct regexrange));
                                                        vector_push(&v, &$1);
                                                        $$ = v;
                                                     }
                    ;

parserange  : parseoption rangetoken    {
                                            struct regexrange range;
                                            range.baseoption = $1;
                                            vector_init(&range.options, sizeof(struct regexoptions));
                                            range.minlen = $2[0];
                                            range.maxlen = $2[1];
                                            $$ = range;
                                        }
            | parseoption               {
                                            struct regexrange range;
                                            range.baseoption = $1;
                                            vector_init(&range.options, sizeof(struct regexoptions));
                                            range.minlen = 1;
                                            range.maxlen = 1;
                                            $$ = range;
                                        }
            ;


parseoption     : leftbox parseoptionmid rightbox   {
                                                        struct regexoptions g;
                                                        g.index = 0;
                                                        g.groups = $2;
                                                        $$ = g;
                                                    }
                | dot                               {
                                                        struct regexoptions ro;
                                                        regexoptions_init(&ro);
                                                        struct regexcharactergroup g;
                                                        regexcharactergroup_init(&g);
                                                        struct regexcharacter c;
                                                        c.minc = 0;
                                                        c.maxc = 126;
                                                        c.currentc = 0;
                                                        regexcharactergroup_push(&g, c);
                                                        regexoptions_push(&ro, g);
                                                        $$ = ro;
                                                    }
                | parsecharactergroup               {
                                                        struct regexoptions g;
                                                        g.index = 0;
                                                        vector_init(&g.groups, sizeof(struct regexcharactergroup));
                                                        regexoptions_push(&g, $1);
                                                        $$ = g;
                                                    }
                ;
parseoptionmid  : parseoptionmid parsecharactergroup    { vector_push(&$1, &$2); $$ = $1; }
                | parsecharactergroup                   {
                                                            struct vector v;
                                                            vector_init(&v, sizeof(struct regexcharactergroup));
                                                            vector_push(&v, &$1);
                                                            $$ = v;
                                                        }
                ;

parsecharactergroup : leftparanthesis  parsecharactergroupmid rightparanthesis {
                                                struct regexcharactergroup g;
                                                g.characters = $2; $$ = g;
                                            }
                    | parsecharacter {
                                        struct regexcharactergroup g;
                                        regexcharactergroup_init(&g);
                                        regexcharactergroup_push(&g, $1);
                                        $$ = g;
                                    }
                    ;

parsecharactergroupmid  : parsecharactergroupmid parsecharacter  { vector_push(&$1, &$2); $$ = $1; }
                        | parsecharacter                         {
                                                                    struct vector v;
                                                                    vector_init(&v, sizeof(struct regexcharacter));
                                                                    vector_push(&v, &$1);
                                                                    $$ = v;
                                                                 }
                        ;



parsecharacter  : character '-' character     { struct regexcharacter foo; foo.minc = $1; foo.maxc = $3; $$ = foo; }
                | character                   { struct regexcharacter foo; foo.minc = $1; foo.maxc = $1; $$ = foo; }
                ;

%%

void yyerror(returnstruct * r, char* s) { fprintf(stderr, "%s\n", s); }

void printhelppage() {
    printf(
        "HELP PAGE:\n"
        "\t-c <int> forces * and + to only go to int specified. Default is 255.\n"
        "\t-d --debug shows how the regex is parsed\n"
        "\t-h --help shows this page\n"
    );
}

int main(int argc, char * argv[]) {

    int validtoprint[argc];
    int atleastonearg = 0;

    int debug = 0;
    int help = 0;

    int parsemaxcharactercount = 0;

    maxcharactercount = 255;
    for (int i = 1; i < argc; i++) {
        validtoprint[i] = 0;

        if (parsemaxcharactercount) {
            maxcharactercount = atoi(argv[i]);
            parsemaxcharactercount = 0;
            continue;
        }

        if (strcmp(argv[i], "-debug") == 0) {
            debug = 1;
            continue;
        }
        if (strcmp(argv[i], "--debug") == 0) {
            debug = 1;
            continue;
        }
        if (strcmp(argv[i], "-help") == 0) {
            help = 1;
            continue;
        }
        if (strcmp(argv[i], "--help") == 0) {
            help = 1;
            continue;
        }
        if (argv[i][0] == '-') {
            char * itr = argv[i]+1;
            if (*itr == '-') { itr++; }
            while (*itr) {
                if (*itr == 'd') {
                    debug = 1;
                } else if (*itr == 'h') {
                    help = 1;
                } else if (*itr == 'c') {
                    parsemaxcharactercount = 1;
                } else {
                    fprintf(stderr, "Unknown character %c and argument %s\n",
                        *itr, argv[i]);
                    printhelppage();
                    return 1;
                }
                itr++;
            }
            continue;
        }
        atleastonearg = 1;
        validtoprint[i] = 1;
    }

    if (atleastonearg) {
        int fd[2];
        if (pipe(fd) != 0) {
            fprintf(stderr, "Failed to pipe\n");
            return 2;
        }
        dup2(fd[0], 0);
        for (int i = 1; i < argc; i++) {
            if (!validtoprint[i]) {
                continue;
            }
            dprintf(fd[1], "%s", argv[i]);
        }
        close(fd[1]);
    }
    
    if (help) {
        printhelppage();
        return 0;
    }

    returnstruct c;
    int r = yyparse(&c);
    if (r == 0) {
        regexexpression_reset(&c);
        if (debug) {
            regexexpression_debug(&c);
        }

        regexexpression_print(&c);
        while (!regexexpression_inc(&c)) {
            regexexpression_print(&c);
        }
    }

    return r;
}


