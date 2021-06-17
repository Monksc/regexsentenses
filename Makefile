build: lexbuild yaccbuild
	gcc -g -Wvla -fsanitize=address lex.yy.c y.tab.c regexexpression.c vector.c -o regexsentenses
lexbuild: yaccbuild
	lex language.l
yaccbuild:
	yacc -d language.y
run: build
	./regexsentenses
install: build
	mv ./regexsentenses /usr/local/bin/
uninstall:
	rm /usr/local/bin/regexsentenses
clean:
	rm ./regexsentenses y.tab.c y.tab.h lex.yy.c
	rm -r regexsentenses.dSYM
