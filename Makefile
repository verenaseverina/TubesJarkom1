all: sendfile recvfile

sendfile: bin/sender.o bin/ack.o bin/file.o bin/packet.o bin/receiver_socket.o bin/receiver_window.o bin/sender_socket.o bin/sender_window.o
	@echo "Linking sendfile ..."
	@g++ bin/sender.o bin/ack.o bin/file.o bin/packet.o bin/receiver_socket.o bin/receiver_window.o bin/sender_socket.o bin/sender_window.o -o sendfile -g

recvfile: bin/receiver.o bin/ack.o bin/file.o bin/packet.o bin/receiver_socket.o bin/receiver_window.o bin/sender_socket.o bin/sender_window.o
	@echo "Linking recvfile ..."
	@g++ bin/receiver.o bin/ack.o bin/file.o bin/packet.o bin/receiver_socket.o bin/receiver_window.o bin/sender_socket.o bin/sender_window.o -o recvfile -g

bin/ack.o: src/ack.cpp src/ack.h
	@echo "Compiling bin/ack.cpp ..."
	@g++ -c src/ack.cpp -o bin/ack.o -g

bin/file.o: src/file.cpp src/file.h
	@echo "Compiling bin/file.cpp ..."
	@g++ -c src/file.cpp -o bin/file.o -g

bin/packet.o: src/packet.cpp src/packet.h
	@echo "Compiling bin/packet.cpp ..."
	@g++ -c src/packet.cpp -o bin/packet.o -g

bin/receiver.o: src/receiver.cpp
	@echo "Compiling bin/receiver.cpp ..."
	@g++ -c src/receiver.cpp -o bin/receiver.o -g

bin/receiver_socket.o: src/receiver_socket.cpp src/receiver_socket.h
	@echo "Compiling bin/receiver_socket.cpp ..."
	@g++ -c src/receiver_socket.cpp -o bin/receiver_socket.o -g

bin/receiver_window.o: src/receiver_window.cpp
	@echo "Compiling bin/receiver_window.cpp ..."
	@g++ -c src/receiver_window.cpp -o bin/receiver_window.o -g

bin/sender.o: src/sender.cpp
	@echo "Compiling bin/sender.cpp ..."
	@g++ -c src/sender.cpp -o bin/sender.o -g

bin/sender_socket.o: src/sender_socket.cpp src/sender_socket.h
	@echo "Compiling bin/sender_socket.cpp ..."
	@g++ -c src/sender_socket.cpp -o bin/sender_socket.o -g

bin/sender_window.o: src/sender_window.cpp
	@echo "Compiling bin/sender_window.cpp ..."
	@g++ -c src/sender_window.cpp -o bin/sender_window.o -g

.PHONY: clean all

clean:
	@echo "Removing object files ..."
	@rm -rf bin/*.o

	@echo "Removing executable ..."
	@rm -rf sendfile recvfile