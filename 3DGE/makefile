
CC=g++
STD=c++11
LIBS=-lGLEW -lglfw -lGLU -lGL -lfreeimage
ERRORFLAGS=-Wall -Wextra -Wparentheses -Wfloat-equal -Wshadow -Wmissing-field-initializers -Wmissing-declarations -Winline
PEDANTIC=-pedantic

FLAGS=$(ERRORFLAGS) $(PEDANTIC) -std=$(STD) $(LIBS)

SRCDIR=src
SRCFILES=$(SRCDIR)/*.cc

OUTDIR=bin

default: setup debug

all: setup normal debug release release-debug

setup:
	mkdir -p $(OUTDIR)

normal:
	$(CC) $(FLAGS) $(SRCFILES) -o $(OUTDIR)/normal

debug:
	$(CC) -g3 $(FLAGS) $(SRCFILES) -o $(OUTDIR)/debug

release:
	$(CC) -O3 $(FLAGS) $(SRCFILES) -o $(OUTDIR)/release

release-debug:
	$(CC) -O3 -g3 $(FLAGS) $(SRCFILES) -o $(OUTDIR)/release-debug

