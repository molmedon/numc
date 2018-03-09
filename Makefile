# compiler
CXX = g++

# name of output executable
BIN = NuMC

# directory to store binary
BIN_DIR = build

# and the name of the test executable
TESTBIN = build/Test_NuMC

# set source and object directories
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = test
DATA_DIR = $(shell pwd)/data

# a hash of the current src/include *.cpp files
# this is saved into all output data files to verify the code
# that produced any particular data file
HASH = $(shell find ${SRC_DIR} ${OBJ_DIR} -type f -name "*.cpp" -exec md5sum {} \; | sort -k 2 | md5sum | tr -d '-' | sed -e's/[[:space:]]*$$//')
# COMMIT = $(shell git rev-parse HEAD)

# find all the source files
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/readers/*.cpp)
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp) $(wildcard $(TEST_DIR)/readers/*.cpp)

# and make the appropriate object files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# preprocessor flags - include boost and root
CPPFLAGS = -Iinclude -isystem/usr/include/boost -isystem/usr/include/root # include the system libraries without warnings
CPPFLAGS += -isystemlib/include/ # and 3rd party libs without warnings too

# compiler falgs
CFLAGS += -std=c++14 -Wall -Wextra -Wdisabled-optimization -pedantic -Wshadow
CFLAGS += -Wstrict-overflow=5 -Wsign-conversion -Wold-style-cast -Wcast-align
CFLAGS += -Wundef -Wno-unused -Wlong-long -Wconversion -Wstack-protector
CFLAGS += -Wpointer-arith -Wpacked -Wformat-y2k -Warray-bounds -Wreorder
CFLAGS += -mtune=native
CFLAGS += -DDATA_DIR=\"$(DATA_DIR)\"

# linker flags
LDFLAGS = -Llib -L/usr/lib/root

# libs for ROOT
ROOTLIBS = -lHist -lCore -lTree -lRIO -lTreePlayer -lMathCore -lMathMore

# libs for BOOST
BOOSTLIBS = -lboost_program_options

# third party libraries
LDLIBS += $(BOOSTLIBS) $(ROOTLIBS)

# dependencies for executable
DEPS = data/bedmap2_bin

# name the phony's just to be safe
.PHONY: all clean test

# set the primary target
all: $(BIN_DIR)/$(BIN) $(DEPS)

# and the test target
test: $(TESTBIN) $(DEPS)

# dependencies for bin - everything comes from here
# LDFLAGS must come BEFORE LDLIBS - this is linking ONLY
$(BIN_DIR)/$(BIN): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(TESTBIN): $(TEST_OBJ) $(filter-out $(OBJ_DIR)/$(BIN).o, $(OBJ))
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# we provide a rule to compile the objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# we provide a rule to compile the objects
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# if bedmap hasn't been unzipped, unzip it and store it in the data directory
data/bedmap2_bin: data/bedmap2_bin.zip
	unzip data/bedmap2_bin.zip -d data

# if bedmap data hasn't been downloaded, download it
data/bedmap2_bin.zip:
	wget -c https://secure.antarctica.ac.uk/data/bedmap2/bedmap2_bin.zip --directory-prefix=data

# delete the object and binary for clean
clean:
	rm -rf $(OBJ) $(TEST_OBJ) $(BIN) obj/lib/*.o
