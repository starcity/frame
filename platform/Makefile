#!make


TARGET=Server

CURRENTPWD=$(shell pwd)
BASELIB=$(CURRENTPWD)
TOOLSPWD=$(CURRENTPWD)/../tools

CXXFLAGS += -DDEBUG
CXXFLAGS += -O0 -g -Wall
CXXFLAGS += -std=c++11
CXXFLAGS +=-I$(TOOLSPWD)

CC=g++
SOURCES= $(wildcard *.cpp ../tools/json/*.cpp ../tools/log/*.cpp)
OBJS = $(patsubst %cpp, %o, $(SOURCES))
#SOURCES=$(CURRENTPWD)/main.o                      \
#      	$(TOOLSPWD)/log/Log.o                   \
#        $(CURRENTPWD)/httpserver.o                \
#        $(CURRENTPWD)/businessManager.o        \
#	     $(CURRENTPWD)/cencode.o                      \
#        $(TOOLSPWD)/json/json_reader.o          \
#        $(TOOLSPWD)/json/json_value.o           \
#        $(TOOLSPWD)/json/json_writer.o
#	$(TOOLSPWD)/tinyxml/CTcxml.o			\
#       $(TOOLSPWD)/tinyxml/tinyxmlerror.o				\
#       $(TOOLSPWD)/tinyxml/tinyxmlparser.o				\
#       $(TOOLSPWD)/tinyxml/tinyxml.o					\
#       $(TOOLSPWD)/tinyxml/tinystr.o				\

LIBDIR=-L.
LIB=-lpthread -levent -lyaml -Wl,-rpath,./ 


%.o:%.cpp %.h %.hpp
	$(CC) $(CXXFLAGS) -c $< -o $@


$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(CXXFLAGS) $(LIB) $(LIBDIR)

clean:
	rm $(OBJS) $(TARGET) -f

