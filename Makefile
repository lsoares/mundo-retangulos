build:
	gcc -Wall mundo.c retangulos.c -o mundo.exe

run: build
	./mundo.exe