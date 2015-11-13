srcdir = src
bindir = bin
objdir = obj
exec = main

VPATH = $(srcdir)

CXX = g++

ifeq ($(OS),Windows_NT)
	#Windows stuff
	CXXFLAGS = -O2 -Wall -pedantic -Wextra -std=c++11 -lglew32 -lglfw3 -lgdi32 -lSOIL -lopengl32 -lassimp
else
	#Linux stuff
	CXXFLAGS = -O2 -Wall -pedantic -Wextra -std=c++11 -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lassimp
endif

sources = main.cpp application.cpp input_handler.cpp shader_program.cpp static_mesh.cpp static_shader_program.cpp world_pipeline.cpp game_state.cpp str_event.cpp str_engine_cpu.cpp texture.cpp
objects = $(sources:.cpp=.o)

all: $(bindir) $(objdir) $(exec)

$(bindir):
	mkdir -p $(bindir)

$(objdir):
	mkdir -p $(objdir)

$(exec): $(objects)
	$(CXX) $(addprefix $(objdir)/, $(objects)) -o $(bindir)/$@ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(objdir)/$@ 2> log.txt

run: all
	$(bindir)/$(exec)

clean:
	rm -rf bin/
	rm -rf obj/
	rm -rf *.o
