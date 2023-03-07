SRCDIR=src
OBJDIR=obj

CPP=g++
CPPFLAGS=-Wall -pedantic -ggdb -I$(SRCDIR) -std=c++20 -IC:/msys64/mingw64/include/SDL2 -Dmain=SDL_main
LDFLAGS=-LC:/msys64/mingw64/lib -lopengl32 -lglu32 -lglew32 -DGLEW_STATIC -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_image

_DEPS=clkgl.h clkinputman.h clkinputtrigger.h clkkeybind.h clkmbuttonbind.h clkrand.h clkterminator.h clktextureman.h clktiming.h clkviewport.h clkwin.h
DEPS=$(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ= main.o clkinputman.o clkinputtrigger.o clkkeybind.o clkmbuttonbind.o clkrand.o clkterminator.o clktextureman.o clktiming.o clkviewport.o clkwin.o
OBJ =$(patsubst %,$(OBJDIR)/%,$(_OBJ))

04.exe: $(OBJ)
	$(CPP) -o $@ $^ $(LDFLAGS)

$(OBJ): $(OBJDIR)/%.o: src/%.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*\.o 04.exe