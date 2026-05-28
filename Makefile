all:
	gcc ./src/*.c main.c -I./include -o corrida

run:
	gcc ./src/*.c main.c -I./include -o corrida && ./corrida

clean:
	rm -f corrida
