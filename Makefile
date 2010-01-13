

#OBJS = main.obj io.obj
#CC = bcc
#MODEL = s
#CFLAGS = –m$(MODEL) 

#project.exe : $(OBJS)
#	tlink c0$(MODEL) $(OBJS), $(.TARGET),, c$(MODEL) /Lf:\bc\lib 

#$(OBJS) : incl.h 



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
SRCS   = ${wildcard src/*.c src/*/*.c}
OBJS   = ${wildcard src/*.o src/*/*.o}
HDRS2  = ${wildcard src/*.h src/*/*.h}


%.o : %.c
%src/*/*.o : %src/*/*.c
#	${CC} ${CFLAGS} –c ${.SOURCE}

all: ${PROG}

${PROG}: ${OBJS}
	echo ${HDRS2}
	echo ${OBJS}
	echo ${@}
	${LD} -c ${CFLAGS} ${LDFLAGS} ${SRCS} ${HDRS2}
	${LD} ${wildcard src/*.o src/*/*.o} -o smr ${LDFLAGS}  ${LIBS}
	

linker:
	echo "win"	

clean:
	rm -f ${OBJS} ${PROG}


${OBJS}: ${HDRS} Makefile
