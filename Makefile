# compiler
CXX = g++

# name of output executable
BIN = NuMC

# directory to store binary
BIN_DIR = build

# and the name of the test executable
TESTBIN = build/Test_NuMC

# and a directory to store the output files from the test
TEST_OUTPUT_DIR = $(shell pwd)/test/output

# set source and object directories
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = test
DATA_DIR = $(shell pwd)/data

# a hash of the current src/include *.cpp files
# this is saved into all output data files to verify the code
# that produced any particular data file
HASH = $(shell find ${SRC_DIR} ${OBJ_DIR} -type f -name "*.cpp" -exec md5sum {} \; | sort -k 2 | md5sum | tr -d '-' | sed -e's/[[:space:]]*$$//')

# the first 8 characters of the current git commit
COMMIT = $(shell git rev-parse HEAD | head -c 8)

# find all the source files
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/readers/*.cpp)
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp) $(wildcard $(TEST_DIR)/readers/*.cpp)

# and make the appropriate object files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.d)
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

# output options for compilation
# -MMD and -MP produce header file dependency maps for each source file
OUTPUT_OPTS = -MMD -MP -o

# linker flags
LDFLAGS = -Llib -L/usr/lib/root

# libs for ROOT
ROOTLIBS = -lHist -lCore -lTree -lRIO -lTreePlayer -lMathCore -lMathMore -lGpad

# libs for BOOST
BOOSTLIBS = -lboost_program_options

# third party libraries
LDLIBS += $(BOOSTLIBS) $(ROOTLIBS)

# other dependencies for executable
BINDEPS = data/bedmap2_bin

# name the phony's just to be safe
.PHONY: all clean test

# set the primary target
all: $(BIN_DIR)/$(BIN) $(DEPS) $(BINDEPS)

# and the test target
test: $(TESTBIN) $(DEPS) $(BINDEPS)

# dependencies for bin - everything comes from here
# LDFLAGS must come BEFORE LDLIBS - this is linking ONLY
$(BIN_DIR)/$(BIN): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(TESTBIN): $(TEST_OBJ) $(filter-out $(OBJ_DIR)/$(BIN).o, $(OBJ))
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# include the generated dependency files
-include $(DEPS)

# we provide a rule to compile the objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< $(OUTPUT_OPTS) $@

# we provide a rule to compile the objects
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) -DOUTPUT_DIR=\"$(TEST_OUTPUT_DIR)\" -c $< -o $@

# if bedmap hasn't been unzipped, unzip it and store it in the data directory
data/bedmap2_bin: data/bedmap2_bin.zip
	unzip data/bedmap2_bin.zip -d data

# if bedmap data hasn't been downloaded, download it
data/bedmap2_bin.zip:
	wget -c https://secure.antarctica.ac.uk/data/bedmap2/bedmap2_bin.zip --directory-prefix=data

# delete all objects, binaries, and test results
clean:
	rm -rf $(OBJ) $(DEPS) $(TEST_OBJ) $(BIN) $(TESTBIN) obj/lib/*.o test/output/*

