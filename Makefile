.SILENT:

build:
	gcc -Wall -Werror cli.c retangulos.c ver_mundo.c -o cli.exe

test: build
	gcc -Wall -Werror test_retangulos.c retangulos.c -o test_retangulos.exe
	./test_retangulos.exe
	echo "✅ Testes ao módulo de retângulos."

	gcc -Wall -Werror test_cli.c testing.c -o test_cli.exe
	./test_cli.exe
	echo "✅ Testes funcionais."

run: build
	./mundo.exe