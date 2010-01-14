

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
OBJS   = ${SRCS:.c=.o}
HDRS2  = ${wildcard src/*.h src/*/*.h}


#%.o : %.c
#%src/*/*.o : %src/*/*.c
#	${CC} ${CFLAGS} –c ${.SOURCE}

all: ${PROG}

#%.o : %src/*. %src/*/*.c

${OBJS} : ${SRCS} ${HDRS2}
	${LD} -c ${CFLAGS} ${LDFLAGS} ${SRCS} ${HDRS2}

${PROG}: ${OBJS}
	${LD} ${wildcard *.o} -o smr ${LDFLAGS}  ${LIBS}


linker:
	echo "win"	

clean:
	rm -f ${wildcard *.o} ${PROG}


${OBJS}: ${HDRS} Makefile
