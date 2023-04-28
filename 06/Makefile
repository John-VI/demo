SRCDIR=src
OBJDIR=obj

CPP=g++
CPPFLAGS=-Wall -pedantic -ggdb -I$(SRCDIR) -std=c++20 -D_REENTRANT -fext-numeric-literals
LDFLAGS=-lGL -lGLU -lGLEW -DGLEW_STATIC -lSDL2main -lSDL2 -lSDL2_image

_DEPS=clkgl.h clkinputman.h clkinputtrigger.h clkkeybind.h clkmbuttonbind.h clkrand.h clkterminator.h clktextureman.h clktiming.h clkviewport.h clkwin.h clktextgrid.h
DEPS=$(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ= main.o clkinputman.o clkinputtrigger.o clkkeybind.o clkmbuttonbind.o clkrand.o clkterminator.o clktextureman.o clktiming.o clkviewport.o clkwin.o clktextgrid.o
OBJ =$(patsubst %,$(OBJDIR)/%,$(_OBJ))

06: $(OBJ)
	$(CPP) -o $@ $^ $(LDFLAGS)

$(OBJ): $(OBJDIR)/%.o: src/%.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*\.o 06
