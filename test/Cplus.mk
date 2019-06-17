# testCplus.c
# testCplus.h
# testCplusMain.c

main:testCplus.c testCplusMain.c
	g++ -o $@ $^
# testCplus.o:testCplus.c testCplus.h
# 	g++ -c $^ -o testCplus.o
# testCplusMain.o:testCplusMain.c
# 	g++ -c testCplusMain.c -o testCplusMain.o

.PHONY:clean
clean:
	rm -rf main
