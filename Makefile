src=HttpServer.c main.c
main:$(src)
	g++ -o $@ $^ -lpthread
.PHONY:clean
clean:
	rm -rf main
