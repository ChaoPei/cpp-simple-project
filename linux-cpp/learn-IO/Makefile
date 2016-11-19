#
# Makefile 
# 

CC = gcc

EXEC_CLIENT = client
EXEC_SERVER = server

OBJ_CLIENT = client.o
OBJ_SERVER = server.o

SRC_CLIENT = io_client.c
SRC_SERVER = io_server.c
SRC_PROCESS = process.c

LD_FLAGES_COMMON = -O2 

all:
	make server
	make client


server:
	$(CC) $(SRC_PROCESS) $(SRC_SERVER) $(LD_FLAGES_COMMON) -o $(EXEC_SERVER)

client:
	$(CC) $(SRC_PROCESS) $(SRC_CLIENT) $(LD_FLAGES_COMMON) -o $(EXEC_CLIENT)

clean:
	rm -rf $(EXEC_SERVER) $(EXEC_CLIENT) *.o