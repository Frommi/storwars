srcdir = src
bindir = bin
objdir = obj
exec = main

#test_srcdir = test

#VPATH = $(srcdir) $(srcdir)/Archiver/Huffman $(srcdir)/Archiver/LZ77 $(srcdir)/Archiver/LZW $(srcdir)/Archiver/FASTQ $(srcdir)/InputOutput/Buffered $(srcdir)/InputOutput/StreamReader $(srcdir)/InputOutput/ArrayReaderWriter $(srcdir)/InputOutput/Bit $(srcdir)/Generator $(bindir) $(objdir)
CXX = g++
CXXFLAGS = -O2 -Wall -pedantic -Wextra -std=c++11 -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread

#sources = main.cpp HuffmanArchiver.cpp LZ77Archiver.cpp LZWArchiver.cpp Trie.cpp TrieNode.cpp FASTQArchiver.cpp BufferedReader.cpp BufferedWriter.cpp StreamReader.cpp ArrayReaderWriter.cpp Generator.cpp BitReader.cpp BitWriter.cpp
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
