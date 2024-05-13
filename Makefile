build:
	gcc -Wall mundo.c retangulos.c ver_mundo.c -o mundo.exe

run: build
	./mundo.exe