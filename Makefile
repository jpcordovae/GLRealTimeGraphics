#makefile for 2d graphic

all:
	gcc -o GL2DGraphics main.cpp glApplication.cpp glutwindow.cpp glCartesianGraphic.cpp glWindow.cpp -I./nvMath -lglut -lboost_system -lboost_thread

clean:
	rm -rf *o GL2DGraphics
