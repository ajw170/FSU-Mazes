#
# makefile for mazes-on-the-web project
#

LIB     = /home/courses/cop4531p/LIB
#LIB     = /home/faculty/lacher/cpp.80
CPP     = $(LIB)/cpp
TCPP    = $(LIB)/tcpp
GRAPH   = $(LIB)/graph
AREA51  = $(LIB)/area51
#AREA51  = ../area51
PROJ    = .
INCPATH = -I$(PROJ) -I$(CPP) -I$(TCPP) -I$(GRAPH)

CC   = g++ -std=c++11 -Wall -Wextra

all:       required extra
required:  solvemaze.x ranmaze2t.x ranmaze2w.x printmaze.x
extra:     agraph.x fpath.x fsymm.x maze2graph.x mazemaster.x rangraph.x

# required:

solvemaze.x: $(GRAPH)/graph.h $(GRAPH)/graph_util.h $(GRAPH)/bfsurvey.h $(GRAPH)/dfsurvey.h $(GRAPH)/survey_util.h solvemaze.cpp
	$(CC) $(INCPATH) -osolvemaze.x solvemaze.cpp

ranmaze2t.x: $(PROJ)/ranmaze2t.o $(PROJ)/xran.o
	$(CC) $(INCPATH) -o ranmaze2t.x $(PROJ)/xran.o $(PROJ)/ranmaze2t.o

ranmaze2w.x: $(PROJ)/ranmaze2w.o $(PROJ)/xran.o
	$(CC) $(INCPATH) -o ranmaze2w.x $(PROJ)/xran.o $(PROJ)/ranmaze2w.o

ranmaze2t.o: $(CPP)/xran.h $(GRAPH)/partition2.h ranmaze2t.cpp
	$(CC) $(INCPATH) -c ranmaze2t.cpp

ranmaze2w.o: $(CPP)/xran.h $(GRAPH)/partition2.h ranmaze2w.cpp
	$(CC) $(INCPATH) -c ranmaze2w.cpp

xran.o: $(CPP)/xran.h $(CPP)/xran.cpp
	$(CC) $(INCPATH) -c $(CPP)/xran.cpp

printmaze.x: mazeprint.c
	cc -O2 -g mazeprint.c -o printmaze.x -lm

# extra:

fpath.x: $(GRAPH)/graph.h $(GRAPH)/graph_util.h $(GRAPH)/bfsurvey.h $(GRAPH)/dfsurvey.h $(GRAPH)/survey_util.h $(GRAPH)/fpath.cpp
	$(CC) $(INCPATH) -ofpath.x $(GRAPH)/fpath.cpp

fsymm.x: $(GRAPH)/graph.h $(GRAPH)/graph_util.h $(GRAPH)/bfsurvey.h $(GRAPH)/dfsurvey.h $(GRAPH)/survey_util.h $(GRAPH)/fsymm.cpp
	$(CC) $(INCPATH) -ofsymm.x $(GRAPH)/fsymm.cpp

maze2graph.x: $(GRAPH)/maze_util.h $(GRAPH)/maze2graph.cpp
	$(CC) $(INCPATH) -omaze2graph.x $(GRAPH)/maze2graph.cpp

mazemaster.x: $(AREA51)/mazemaster_i.x
	cp $(AREA51)/mazemaster_i.x $(PROJ)/mazemaster.x
	chmod 700 mazemaster.x

rangraph.x: $(AREA51)/rangraph_i.x
	cp $(AREA51)/rangraph_i.x $(PROJ)/rangraph.x
	chmod 700 rangraph.x

agraph.x: $(AREA51)/agraph_i.x
	cp $(AREA51)/agraph_i.x $(PROJ)/agraph.x
	chmod 700 agraph.x

