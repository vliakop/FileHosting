SOURCE	= common/commons.cpp common/ClientList.cpp
OBJS	= $(SOURCE:.cpp=.o)
CC		= g++

all: client server

client: $(OBJS) client.o client_functions/client_fnc.o
	$(CC) $(OBJS) client.o client_functions/client_fnc.o -o $@

server: $(OBJS) server.o server_functions/server_fnc.o
	$(CC) $(OBJS) server.o server_functions/server_fnc.o -o $@


clean:
	rm -f *.o client_functions/*.o server_functions/*.o common/*.o
