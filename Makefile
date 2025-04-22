# FIXME use cmake

all:

# NOTE: This one Makefile works with Microsoft nmake and GNU make.
# They use different conditional syntax, but each can be nested and inverted within the other.

ifdef MAKEDIR:
!ifdef MAKEDIR

# Microsoft nmake on Windows. Visual C++.
CFLAGS=-MD -Gy -Z7 -EHsc -std:c++20 -W4 -EHsc -WX
CXX=cl

RM = del 2>nul
RM_F = $(RM) /f
O=obj
EXE=.exe

CLINK_FLAGS=/link /out:$@ /incremental:no /opt:ref /pdb:$(@B).pdb

!else
else

# GNU/Posix make on Unix with gcc, clang, etc.
RM = rm 2>/dev/null
RM_F = rm -f 2>/dev/null
O=o
EXE=
CXX=g++

CFLAGS=-g
CLINK_FLAGS=-o $@

endif
!endif :

all: palindrome$(EXE) \
     local_minimum$(EXE) \

local_minimum$(EXE): local_minimum.cpp
	@-$(RM_F) $(@R).pdb $(@R).ilk
	$(CXX) $(CFLAGS) local_minimum.cpp $(CLINK_FLAGS)

palindrome$(EXE): palindrome.cpp
	@-$(RM_F) $(@R).pdb $(@R).ilk
	$(CXX) $(CFLAGS) palindrome.cpp $(CLINK_FLAGS)

clean:
	$(RM_F) local_minimum local_minimum.exe local_minimum.o local_minimum.obj local_minimum.pdb \
	palindrome palindrome.exe palindrome.o palindrome.obj palindrome.pdb
