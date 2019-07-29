UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    COMPILER = g++
    FLAGS = -std=c++1y -pedantic -Wall
    GL_FLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
    FILES = attribs.cpp ../../glad.c
    APP_NAME = attribsBin
endif


all: main

main: $(FILES)
	    $(COMPILER) $(FLAGS) $(FILES) -o $(APP_NAME) $(GL_FLAGS) $(GLAD_FLAGS)

.PHONY: clean run
	clean:
	    rm opengl-app

run: $(APP_NAME)
	    ./$(APP_NAME)
