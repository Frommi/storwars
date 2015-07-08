srcdir = src
bindir = bin
objdir = obj
exec = main

VPATH = $(srcdir)

CXX = g++
CXXFLAGS = -O2 -Wall -pedantic -Wextra -std=c++11 -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread

sources = main.cpp Application.cpp InputHandler.cpp
objects = $(sources:.cpp=.o)

all: $(bindir) $(objdir) $(exec)

$(bindir):
	mkdir -p $(bindir)

$(objdir):
	mkdir -p $(objdir)

$(exec): $(objects)
	$(CXX) $(CXXFLAGS) $(addprefix $(objdir)/, $(objects)) -o $(bindir)/$@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(objdir)/$@

run: all
	$(bindir)/$(exec)

clean:
	rm -rf bin/
	rm -rf obj/
	rm -rf *.o