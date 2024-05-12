build:
	gcc -Wall mundo.c -o mundo.exe

run: build
	./mundo.exe