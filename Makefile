OPTIONS = -Wall -c -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
HEADERS = common.h grid.h shapes.h mesh.h fonts.h

all: tetris

tetris: main.o common.o grid.o shapes.o mesh.o fonts.o $(HEADERS)
	g++ -o tetris main.o common.o grid.o shapes.o mesh.o -L/usr/lib -lSDL -lGL -lSDL_mixer

main.o: main.cpp $(HEADERS)
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

run: tetris
	./tetris

clean:
	rm *.o

clean-all: clean
	rm tetris

