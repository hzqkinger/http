#! /bin/bash

#if [ $# -ge 1 -a X$1 == "Xclean" ]
if [ X$1 == "Xclean" ]
then
	make -f exec_hello.mk clean
else
	make -f exec_hello.mk
fi


