all: myls.c
	gcc -g -Wall -o myls myls.c infodemo.c
clean: 
	$(RM) myls
