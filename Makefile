all: sendfile recvfile

sendfile: sender.o Ack.o Packet.o
	@echo "Linking sendfile ..."
	@g++ sender.o Ack.o Packet.o -o sendfile -g

recvfile: receiver.o Ack.o Packet.o
	@echo "Linking recvfile ..."
	@g++ receiver.o Ack.o Packet.o -o recvfile -g

Ack.o: src/Ack.cpp src/Ack.h
	@echo "Compiling Ack.cpp ..."
	@g++ -c src/Ack.cpp -o Ack.o -g

Packet.o: src/Packet.cpp src/Packet.h
	@echo "Compiling Packet.cpp ..."
	@g++ -c src/Packet.cpp -o Packet.o -g

receiver.o: src/receiver.cpp
	@echo "Compiling receiver.cpp ..."
	@g++ -c src/receiver.cpp -o receiver.o -g

sender.o: src/sender.cpp src/Packet.h src/Ack.h
	@echo "Compiling sender.cpp ..."
	@g++ -c src/sender.cpp -o sender.o -g

.PHONY: clean all

clean:
	@echo "Removing object files ..."
	@rm -rf *.o

	@echo "Removing executable ..."
	@rm -rf sendfile recvfile