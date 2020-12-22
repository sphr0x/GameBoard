CC=g++
CFLAGS= -Wall -lm -pthread

all: main

main: *.cpp
	$(CC) $(CFLAGS) *.cpp -o main

upload: main
	sudo ./main

server:
	g++ test_server.c pong.cpp -o test_server

run-server: test_server
	./test_server

client:
	g++ test_client.c -o test_client

run-client: test_client
	./test_client

clean:
	rm -rf main
	rm -rf test_client
	rm -rf test_server