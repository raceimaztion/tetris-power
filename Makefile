OPTIONS = -Wall -c -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT# -DDEBUG
HEADERS = common.h grid.h shapes.h mesh.h fonts.h widgets.h play.h texture.h shaders.h screen.h
MAIN_ONLY_HEADERS = splash.h mainMenu.h
MODULES = main.o common.o grid.o shapes.o mesh.o fonts.o splash.o mainMenu.o widgets.o play.o texture.o shaders.o screen.o

all: tetris

linecount:
	wc -l *.h *.cpp

tetris: $(MODULES) screen.o $(HEADERS)
	g++ -o tetris $(MODULES) -L/usr/lib -lm -lSDL -lGL -lSDL_mixer -lSDL_image -lGLU

main.o: main.cpp $(HEADERS) $(MAIN_ONLY_HEADERS)
	g++ $(OPTIONS) main.cpp

common.o: common.cpp $(HEADERS)
	g++ $(OPTIONS) common.cpp

grid.o: grid.cpp $(HEADERS)
	g++ $(OPTIONS) grid.cpp

shapes.o: shapes.cpp $(HEADERS)
	g++ $(OPTIONS) shapes.cpp

mesh.o: mesh.cpp $(HEADERS)
	g++ $(OPTIONS) mesh.cpp

fonts.o: fonts.cpp $(HEADERS)
	g++ $(OPTIONS) fonts.cpp

splash.o: splash.cpp splash.h $(HEADERS)
	g++ $(OPTIONS) splash.cpp

mainMenu.o: mainMenu.cpp mainMenu.h $(HEADERS)
	g++ $(OPTIONS) mainMenu.cpp

widgets.o: widgets.cpp $(HEADERS)
	g++ $(OPTIONS) widgets.cpp

play.o: play.cpp $(HEADERS)
	g++ $(OPTIONS) play.cpp

texture.o: texture.cpp $(HEADERS)
	g++ $(OPTIONS) texture.cpp

shaders.o: shaders.cpp common.h
	g++ $(OPTIONS) shaders.cpp

screen.o: screen.cpp $(HEADERS)
	g++ $(OPTIONS) screen.cpp

run: tetris
	./tetris

clean:
	rm *.o

clean-all: clean
	rm tetris

tests: tests.cpp all
	g++ -DDEBUG -o tests tests.cpp common.o texture.o mesh.o -Wall -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -L/usr/lib -lm -lSDL -lGL -lSDL_mixer -lSDL_image -lGLU
	./tests

