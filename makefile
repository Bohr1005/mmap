 
 
#COMPLILER USED
CC = g++
AR = ar cru

#VARIABLR
INCLUDE=
LIB=
 
#COMPILE FLAGS
ADD_INCL = -I$(INCLUDE)
ADD_LIB = -L$(LIB) -pthread  -lstdc++
 
#LAST FLAG
CFLAGS = -Wall -g -DLINUX  -funsigned-char -fno-stack-protector
CFLAGS  += $(ADD_INCL)
LDFLAGS += $(ADD_LIB)

TARGET = test
   
#EXE PGM  AND LIB
all: ${TARGET}

#MAKE RULE
${TARGET}:
	$(CC) $@.cpp $(CFLAGS) $(LDFLAGS) $^ -o $@

clean: 
	rm -f ${TARGET}
	
  	
		
