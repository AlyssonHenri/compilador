all : lexico.l sintatico.y
	clear
	flex -i lexico.l
	bison sintatico.y
	gcc sintatico.tab.c -o analizador -lfl
	./analizador