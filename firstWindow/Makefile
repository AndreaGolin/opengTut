UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    COMPILER = g++
    FLAGS = -std=c++1y -pedantic -Wall
    GL_FLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
    FILES = firstWindow.cpp
    APP_NAME = firstWindowBin
endif


all: main

main: $(FILES)
	    $(COMPILER) $(FLAGS) $(FILES) ../glad.c -o $(APP_NAME) $(GL_FLAGS) $(GLAD_FLAGS)

.PHONY: clean run
	clean:
	    rm opengl-app

run: $(APP_NAME)
	    ./$(APP_NAME)
