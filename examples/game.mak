CXXFLAGS=-Wall -Wextra -pedantic -O2 -ggdb -std=c++98
LFLAGS=-lm

SRCDIR=src
OBJDIR=bin/build
BINDIR=bin
RESDIR=res

SRCS=$(shell find $(SRCDIR) -type f -name *.cpp)
OBJS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCS:.cpp=.o))

TARGET=$(BINDIR)/stacker

CXXFLAGS+=$(shell pkg-config --cflags sdl3)
LFLAGS+=$(shell pkg-config --libs sdl3) -lGL -lGLU

all: $(TARGET)

$(OBJDIR): ; @mkdir -p $@
$(BINDIR): ; @mkdir -p $@

$(TARGET): $(OBJS) | $(BINDIR)
	$(CXX) $^ -o $@ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.rc | $(OBJDIR)
	$(WINDRES) $< -o $@

clean:
	rm -dr $(OBJDIR) $(BINDIR)

.PHONY: all clean
