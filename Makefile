INCLUDE=./lib/jsoncpp/include
LIB_PATH=./lib/jsoncpp/lib
FLAG=-ljsoncpp -g# -lpthread

src=HttpServer.c main.c
main:$(src)
	g++ -o $@ $^ -I$(INCLUDE) -L$(LIB_PATH) $(FLAG)
.PHONY:clean
clean:
	rm -rf main
