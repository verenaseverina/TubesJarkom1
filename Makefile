all: sendfile recvfile

sendfile: client.o Ack.o Packet.o
	@echo "Linking sendfile ..."
	@g++ client.o Ack.o Packet.o -o sendfile -g

recvfile: server.o Ack.o Packet.o
	@echo "Linking recvfile ..."
	@g++ server.o Ack.o Packet.o -o recvfile -g

Ack.o: src/Ack.cpp src/Ack.h
	@echo "Compiling Ack.cpp ..."
	@g++ -c src/Ack.cpp -o Ack.o -g

client.o: src/client.cpp src/Packet.h src/Ack.h
	@echo "Compiling client.cpp ..."
	@g++ -c src/client.cpp -o client.o -g

Packet.o: src/Packet.cpp src/Packet.h
	@echo "Compiling Packet.cpp ..."
	@g++ -c src/Packet.cpp -o Packet.o -g

server.o: src/server.cpp
	@echo "Compiling server.cpp ..."
	@g++ -c src/server.cpp -o server.o -g

.PHONY: clean all

clean:
	@echo "Removing object files ..."
	@rm -rf *.o

	@echo "Removing executable ..."
	@rm -rf sendfile recvfile