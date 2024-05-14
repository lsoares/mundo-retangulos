.SILENT:

GCC_FLAGS = -Werror -Wall -Wextra -Wenum-conversion -Wenum-compare -Wswitch -Wshadow

build:
	gcc ${GCC_FLAGS} cli.c retangulos.c ver_mundo.c -o cli.exe

test: build
	gcc ${GCC_FLAGS} test_retangulos.c retangulos.c -o test_retangulos.exe
	./test_retangulos.exe
	echo "✅ Testes ao módulo retângulos."

	gcc ${GCC_FLAGS} test_cli.c testing.c -o test_cli.exe
	./test_cli.exe
	echo "✅ Testes funcionais."

run: build
	./cli.exe