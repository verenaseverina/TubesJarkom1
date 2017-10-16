all: sendfile recvfile

sendfile: client.o Packet.o Ack.o
	@echo "Linking sendfile ..."
	@g++ client.o Packet.o Ack.o -o sendfile -g

recvfile: server.o Packet.o Ack.o
	@echo "Linking recvfile ..."
	@g++ server.o Packet.o Ack.o -o recvfile -g

Packet.o: src/Packet.cpp src/Packet.h
	@echo "Compiling Packet.cpp ..."
	@g++ -c src/Packet.cpp -o Packet.o 

Ack.o: src/Ack.cpp src/Ack.h
	@echo "Compiling Ack.cpp ..."
	@g++ -c src/Ack.cpp -o Ack.o 

client.o: src/client.cpp src/Packet.h src/Ack.h
	@echo "Compiling client.cpp ..."
	@g++ -c src/client.cpp -o client.o 

server.o: src/server.cpp
	@echo "Compiling server.cpp ..."
	@g++ -c src/server.cpp -o server.o 

.PHONY: clean all

clean:
	@echo "Removing object files ..."
	@rm -rf *.o

	@echo "Removing executable ..."
	@rm -rf sendfile recvfile