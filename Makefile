OPTIONS = -Wall -c -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
HEADERS = common.h grid.h shapes.h mesh.h fonts.h widgets.h
MAIN_ONLY_HEADERS = splash.h mainMenu.h

all: tetris

tetris: main.o common.o grid.o shapes.o mesh.o fonts.o splash.o mainMenu.o widgets.o $(HEADERS)
	g++ -o tetris main.o common.o grid.o shapes.o mesh.o fonts.o splash.o mainMenu.o widgets.o -L/usr/lib -lSDL -lGL -lSDL_mixer -lSDL_image

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

run: tetris
	./tetris

clean:
	rm *.o

clean-all: clean
	rm tetris

