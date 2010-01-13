#
# The Compiler
#
CC      = gcc
LD      = ${CC}
SMR     = /usr/local/smr
CFLAGS  = -Wall -O2 -I${SMR}/include
LDFLAGS = -L${SMR}/lib


LIBS   = -lm /usr/local/smr/lib/librhd.a

#
# Our program files
#
PROG   = smr

HDRS   =
FILE   = main.c
OBJS   = main.o

all: ${PROG}

${PROG}: ${OBJS}
	${LD} -o ${@} ${LDFLAGS} src/${FILE} ${LIBS}

clean:
	rm -f ${OBJS} ${PROG} 


${OBJS}: ${HDRS} Makefile
