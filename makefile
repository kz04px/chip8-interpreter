CC     := g++
TARGET := main

BINDIR := bin
OBJDIR := obj
SRCDIR := src

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))
CFLAGS  := -std=c++17 -Wall -Wextra -Wpedantic -Wshadow
LIB     := -lGL -lSDL2 -lSDL2_image -lSDL2_ttf
INC     := -I/usr/include/SDL2/

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(BINDIR)/$(TARGET) $(LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	$(RM) -r $(OBJDIR) $(BINDIR)/$(TARGET) $(BINDIR)/$(TARGET)-debug

release:
	$(MAKE) CFLAGS="$(CFLAGS) -O3 -DNDEBUG"

debug:
	$(MAKE) CFLAGS="$(CFLAGS) -g" TARGET="$(TARGET)-debug"

.PHONY: clean
