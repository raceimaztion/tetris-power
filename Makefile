OPTIONS = -Wall -c -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
HEADERS = common.h grid.h shapes.h mesh.h fonts.h widgets.h play.h texture.h screen.h score.h powerups.h
MAIN_ONLY_HEADERS = splash.h mainMenu.h
MODULES = main.o common.o grid.o shapes.o mesh.o fonts.o splash.o mainMenu.o widgets.o play.o texture.o screen.o score.o powerups.o

GPP = g++

all: tetris

linecount:
	wc -l *.h *.cpp

tetris: $(MODULES) $(HEADERS)
	$(GPP) -o tetris $(MODULES) -L/usr/lib -lm -lSDL -lGL -lSDL_mixer -lSDL_image -lGLU -lsqlite3

main.o: main.cpp $(HEADERS) $(MAIN_ONLY_HEADERS)
	$(GPP) $(OPTIONS) main.cpp

common.o: common.cpp $(HEADERS)
	$(GPP) $(OPTIONS) common.cpp

grid.o: grid.cpp $(HEADERS)
	$(GPP) $(OPTIONS) grid.cpp

shapes.o: shapes.cpp $(HEADERS)
	$(GPP) $(OPTIONS) shapes.cpp

mesh.o: mesh.cpp $(HEADERS)
	$(GPP) $(OPTIONS) mesh.cpp

fonts.o: fonts.cpp $(HEADERS)
	$(GPP) $(OPTIONS) fonts.cpp

splash.o: splash.cpp splash.h $(HEADERS)
	$(GPP) $(OPTIONS) splash.cpp

mainMenu.o: mainMenu.cpp mainMenu.h $(HEADERS)
	$(GPP) $(OPTIONS) mainMenu.cpp

widgets.o: widgets.cpp $(HEADERS)
	$(GPP) $(OPTIONS) widgets.cpp

play.o: play.cpp $(HEADERS)
	$(GPP) $(OPTIONS) play.cpp

texture.o: texture.cpp $(HEADERS)
	$(GPP) $(OPTIONS) texture.cpp

screen.o: screen.cpp $(HEADERS)
	$(GPP) $(OPTIONS) screen.cpp

score.o: score.cpp $(HEADERS)
	$(GPP) $(OPTIONS) score.cpp

powerups.o: powerups.cpp $(HEADERS)
	$(GPP) $(OPTIONS) powerups.cpp

run: tetris
	./tetris

clean:
	rm *.o

clean-all: clean
	rm tetris

tests: tests.cpp all
	$(GPP) -DDEBUG -o tests tests.cpp common.o texture.o mesh.o -Wall -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -L/usr/lib -lm -lSDL -lGL -lSDL_mixer -lSDL_image -lGLU
	./tests

