# Executable name
EXEC_NAME=clawfinder

# Compiler to use
CC=g++

# Default Options to use
CFLAGS= -Wno-write-strings -O3
CLIBS=

#NAUTYSRC= nauty.c nautil.c naugraph.c

# Source files
SRC =               \
	CmdLine.cpp     \
	GraphMatrix.cpp	\
	GraphUtils.cpp	\
	Esu.cpp		\
	CompressedGraph.cpp \
	GraphCompressor.cpp \
	GraphStats.cpp      \
	Conditions.cpp	\
	Isomorphism.cpp	\
	Error.cpp	\
	GTrie.cpp	\
	Timer.cpp	\
	Random.cpp	\
	GraphTree.cpp	\
	nauty/nauty.c	\
	nauty/nautil.c	\
	nauty/naugraph.c	\
	claw_finder.cpp 	\
	

OBJ =  ${SRC:.cpp=.o}

#------------------------------------------------------------

all: ${EXEC_NAME}

${EXEC_NAME}: ${OBJ}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_NAME} ${OBJ}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+

clean:
	rm ${EXEC_NAME} *.o *~ *# -rf
