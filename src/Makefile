CC = g++
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
CFLAGS = -Wall -std=c++11 -Wno-missing-braces -s -O1 -I../include/ -L../lib/ $(LIBS)
SCENE_H = ../include/scene.h
OBJ_SCENES = scenes/scene_main.o
OBJS = $(OBJ_SCENES)

scenes/scene_main.o: scenes/scene_main.cpp $(SCENE_H)
	$(CC) -c $< $(CFLAGS) -o $@

scenes/scene_play.o: scenes/scene_play.cpp $(SCENE_H)
	$(CC) -c $< $(CFLAGS) -o $@

game: main.cpp $(SCENE_H) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%: %.cpp
	$(CC) -o $@ $^ $(CFLAGS)
clean:
	rm -f game