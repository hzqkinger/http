INCLUDE=../include
LIB_PATH=../lib
FLAG=-ljsoncpp
src=test_json.cpp

test:$(src)
	g++ -o $@ $^ -I$(INCLUDE) -L$(LIB_PATH) $(FLAG)
.PHONY:clean
clean:
	rm -rf test

# g++ test_json.cpp -o test -I../include -L../lib -ljsoncpp
