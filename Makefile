#
# Makefile for CuraEngine
#

BUILD_DIR = build
SRC_DIR = src
LIBS_DIR = libs

BUILD_TYPE = DEBUG

VERSION ?= DEV
CXX ?= g++
CFLAGS += -c -Wall -Wextra -Woverloaded-virtual -std=c++11 -DVERSION=\"$(VERSION)\" -isystem libs

ifeq ($(BUILD_TYPE),DEBUG)
	CFLAGS+=-ggdb -Og -g -Wno-sign-compare -Wno-old-style-cast
endif
ifeq ($(BUILD_TYPE),PROFILE)
	CFLAGS+= -pg
endif
ifeq ($(BUILD_TYPE),RELEASE)
	CFLAGS+= -O3 -fomit-frame-pointer
endif

LDFLAGS += -L$(BUILD_DIR)/ #-lcgal

SOURCES_RAW = halfEdgeMesh.cpp main.cpp optimizedModel.cpp settings.cpp commandSocket.cpp mesh.cpp supportClassification.cpp supportGeneration.cpp 
SOURCES_RAW += modelFile/modelFile.cpp utils/gettime.cpp utils/logoutput.cpp utils/socket.cpp
SOURCES = $(addprefix $(SRC_DIR)/,$(SOURCES_RAW))

OBJECTS_RAW = $(SOURCES_RAW:.cpp=.o)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(OBJECTS_RAW))

DIRS = $(sort $(dir $(OBJECTS)))

EXECUTABLE = $(BUILD_DIR)/Atlas

TARGET_OS ?= Unknown

ifeq ($(OS),Windows_NT)
	#On windows, we target windows.
	TARGET_OS := WIN32
else
	UNAME := $(shell uname)
	ifeq ($UNAME, Darwin)
		#On MacOS, we target MacOS.
		TARGET_OS := MACOS
	else
		#On linux, check our compiler, we might be compiling for Windows with mingw
		ifneq (,$(findstring mingw,$(CXX)))
			TARGET_OS := WIN32
		else
			TARGET_OS := LINUX
		endif
	endif
endif

ifeq ($(TARGET_OS),WIN32)
	#For windows make it large address aware, which allows the process to use more then 2GB of memory.
	EXECUTABLE := $(EXECUTABLE).exe
	CFLAGS += -march=pentium4 -flto
	LDFLAGS += -Wl,--large-address-aware -lm -lwsock32 -flto
endif
ifeq ($(TARGET_OS),LINUX)
	CFLAGS += -flto
	LDFLAGS += --static -flto
endif
ifeq ($(TARGET_OS), MACOS)
	CFLAGS += -force_cpusubtype_ALL -mmacosx-version-min=10.6 -arch x86_64 -arch i386
	LDFLAGS += -force_cpusubtype_ALL -mmacosx-version-min=10.6 -arch x86_64 -arch i386
endif

all: $(DIRS) $(SOURCES) $(EXECUTABLE)

#$(BUILD_DIR)/libclipper.a: $(LIBS_DIR)/clipper/clipper.cpp
#	$(CXX) $(CFLAGS) -o $(BUILD_DIR)/libclipper.a $(LIBS_DIR)/clipper/clipper.cpp

$(EXECUTABLE): $(OBJECTS) # $(BUILD_DIR)/libclipper.a
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(DIRS):
	-@mkdir -p $(DIRS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $< -o $@

test: $(EXECUTABLE)
	python tests/runtest.py $(abspath $(EXECUTABLE))

## clean stuff
clean:
	rm -f $(EXECUTABLE) $(OBJECTS) #$(BUILD_DIR)/libclipper.a

help:
	@cat Makefile |grep \#\#| grep \: |cut -d\# -f3
