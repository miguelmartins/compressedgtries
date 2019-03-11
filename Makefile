# Executable name
EXEC_NAME=clawfinder

# Compiler to use
CC=g++

# Default Options to use
CFLAGS= -Wall -O3
CLIBS=

# Source files
SRC =               \
	claw_finder.cpp \
	GraphMatrix.cpp	\
	GraphUtils.cpp	\
	CompressedGraph.cpp \
	GraphCompressor.cpp \
	GraphStats.cpp      \
	Error.cpp	

OBJ =  ${SRC:.cpp=.o}

#------------------------------------------------------------

all: ${EXEC_NAME}

${EXEC_NAME}: ${OBJ}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_NAME} ${OBJ}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+

clean:
	rm ${EXEC_NAME} *.o *~ *# -rf
