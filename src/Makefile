CC = g++
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
CFLAGS = -Wall -std=c++11 -Wno-missing-braces -s -O1 -I../include/ -L../lib/ $(LIBS)

game: main.cpp
	$(CC) -o $@ $^ $(CFLAGS)

%: %.cc
	$(CC) -o $@ $^ $(CFLAGS)
clean:
	rm -f game