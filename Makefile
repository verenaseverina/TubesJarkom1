sendfile: client.o  
	@echo "Linking sendfile ..."
	@g++ client.o   -o sendfile

recvfile: server.o  
	@echo "Linking recvfile ..."
	@g++ server.o   -o recvfile

client.o: src/client.cpp
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

all : sendfile recvfile