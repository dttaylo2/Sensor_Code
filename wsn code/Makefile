prefix := /usr/local
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -l json -l rf24-bcm

# define all programs
PROGRAMS = sender
SOURCES = ${PROGRAMS:=.cpp}



all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	g++ -o $@ -Wall -I../ $@.cpp ${CCFLAGS}

clean:
	rm -rf $(PROGRAMS)

