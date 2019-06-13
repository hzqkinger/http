
exec_hello:exec_hello.c
	g++ -o $@ $^ 
#	export AAA=hzq  在这里设置环境变量对其它程序没有意义

.PHONY:clean
clean:
	rm -rf exec_hello
