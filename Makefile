srcdir = src
bindir = bin
objdir = obj
exec = main

VPATH = $(srcdir)

CXX = g++

ifeq ($(OS),Windows_NT)
	#Windows stuff
	CXXFLAGS = -O2 -Wall -pedantic -Wextra -std=c++11 -lglew32 -DGLEW_STATIC -lglfw3 -lgdi32 -lSOIL -lopengl32 -lassimp -lnoisepp
else
	#Linux stuff
	CXXFLAGS = -O2 -Wall -pedantic -Wextra -std=c++11 -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lassimp
endif

sources = main.cpp application.cpp voxel_generator.cpp voxel_shader_program_generate.cpp voxel_shader_program_render.cpp static_mesh.cpp str_event.cpp static_shader_program.cpp shader_program.cpp world_pipeline.cpp input_handler.cpp texture.cpp
objects = $(sources:.cpp=.o)

all: $(bindir) $(objdir) $(exec)

$(bindir):
	mkdir -p $(bindir)

$(objdir):
	mkdir -p $(objdir)

$(exec): $(objects)
	$(CXX) $(addprefix $(objdir)/, $(objects)) -o $(bindir)/$@ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(objdir)/$@ 2>> log.txt

run: all
	$(bindir)/$(exec)

clean:
	rm -rf log.txt
	rm -rf bin/
	rm -rf obj/
	rm -rf *.o
