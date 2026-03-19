OBJ=./obj/
SRC=./src/
HEADER=./include/
API=$(HEADER)/api/
INTERNAL=$(HEADER)/internal/
TOOL=$(HEADER)/tool/
LIBPATH=./lib/
NAMELIB=Simple_Request_Protocol
LIB=$(LIBPATH)lib$(NAMELIB).a
FLAG=-Werror -Wextra  -I $(HEADER)
TEST=./test/
BIN=./bin/

all: $(LIB) 

$(LIB): $(OBJ)server.o $(OBJ)client.o $(OBJ)parser.o  $(OBJ)net.o $(OBJ)log.o $(OBJ)protocol.o $(OBJ)alloc.o
	ar rcs $(LIB)  $(OBJ)server.o $(OBJ)client.o $(OBJ)parser.o $(OBJ)net.o $(OBJ)log.o $(OBJ)protocol.o $(OBJ)alloc.o

$(OBJ)server.o : $(SRC)server.c $(API)server.h 
	gcc $(FLAG) -c $(SRC)server.c -o $(OBJ)server.o

$(OBJ)client.o : $(SRC)client.c $(API)client.h 
	gcc $(FLAG) -c $(SRC)client.c -o $(OBJ)client.o

$(OBJ)parser.o : $(SRC)parser.c $(INTERNAL)parser.h  
	gcc $(FLAG) -c $(SRC)parser.c -o $(OBJ)parser.o 

$(OBJ)protocol.o : $(SRC)protocol.c $(INTERNAL)protocol.h
	gcc $(FLAG) -c $(SRC)protocol.c -o $(OBJ)protocol.o  

$(OBJ)net.o : $(SRC)net.c $(INTERNAL)net.h
	gcc $(FLAG) -c $(SRC)net.c -o $(OBJ)net.o 

$(OBJ)log.o : $(SRC)log.c $(TOOL)log.h 
	gcc $(FLAG) -c $(SRC)log.c -o $(OBJ)log.o

$(OBJ)alloc.o : $(SRC)alloc.c $(TOOL)alloc.h 
	gcc $(FLAG) -c $(SRC)alloc.c -o $(OBJ)alloc.o

test: $(BIN)client.exe $(BIN)server.exe $(BIN)parser.exe

$(BIN)client.exe : $(LIB) $(TEST)client.c 
	gcc $(FLAG) $(TEST)client.c  -L $(LIBPATH) -l$(NAMELIB) -o $(BIN)client.exe

$(BIN)server.exe : $(LIB) $(TEST)server.c 
	gcc $(FLAG) $(TEST)server.c  -L $(LIBPATH) -l$(NAMELIB) -o $(BIN)server.exe 
$(BIN)parser.exe : $(LIB) $(TEST)parser.c
	gcc $(FLAG) $(TEST)parser.c  -L $(LIBPATH) -l$(NAMELIB) -o $(BIN)parser.exe

clean :
	rm -f $(OBJ)*.o
	rm -f $(BIN)*.exe
	rm -f $(LIB)


