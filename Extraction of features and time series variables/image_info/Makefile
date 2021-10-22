SHELL=/bin/sh

INST_PATH=$$PWD

LIBS=-L/usr/local/lib -ljpeg -lungif -ltiff 
DLDEF=-I/usr/local/include

# These are correct for Linux
CC=gcc -c $(PYTHON_INC) $(DLDEF) -DUSE_LIBTIFF
CXX=g++ -c $(PYTHON_INC) $(DLDEF) -DUSE_LIBTIFF

LDXX=g++
BUILD_PY_INT=$(LDXX) -shared  -Wl,-rpath,$$PWD -Wl,-rpath,$(INST_PATH)
BUILD_SOLIBRARY=$(LDXX) -shared -Wl,-rpath,$$PWD -Wl,-rpath,$(INST_PATH)
BUILD_TCL_INT=$(LDXX) -shared  -Wl,-rpath,$$PWD -Wl,-rpath,$(INST_PATH)

LIBOBJECTS=ppmutil.o ppmdl.o

# Default target, the static library
libimage_info.a: $(LIBOBJECTS)
	ar rv $@ $(LIBOBJECTS)
	ranlib $@

%.o: %.cxx
	$(CXX) $<
