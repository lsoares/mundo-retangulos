.SILENT:

GCC_FLAGS = -Werror -Wall -Wextra -Wenum-conversion -Wenum-compare -Wswitch -Wshadow

clean:
	find . -type f -name "*.exe" -exec rm -f {} \;
	rm nul || true

build: clean
	gcc ${GCC_FLAGS} cli.c retangulos.c ver_mundo.c -o cli.exe

test: build
	gcc ${GCC_FLAGS} tests/test_retangulos.c tests/testing.c retangulos.c -o tests/test_retangulos.exe
	tests/test_retangulos.exe
	echo "✅ Testes ao módulo retângulos."

	gcc ${GCC_FLAGS} tests/test_cli.c tests/testing.c -o tests/test_cli.exe
	tests/test_cli.exe
	echo "✅ Testes funcionais."

	make clean

run: build
	./cli.exe