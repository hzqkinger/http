src=sock.c
obj_sock:$(src)
	g++ -o $@ $^ -lpthread
.PHONY:clean
clean:
	rm -rf obj_sock
