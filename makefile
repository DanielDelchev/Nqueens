CC=g++;
CFLAGS=-Wall -std=c++11
all:main.cpp
	g++	main.cpp -o Nqueens -Wall -O3 -std=c++11 -I.