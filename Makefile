CXX = clang++
VERSION = 0.1.0

SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BUILD_DIR = build
TEST_DIR = tests
INSTALL_DIR = /usr/local

# Build type (Debug/Release)
BUILD_TYPE ?= Debug
ifeq ($(BUILD_TYPE),Debug)
    CXXFLAGS = -std=c++20 -g -Wall -Wextra -Werror -fsanitize=address,undefined
else
    CXXFLAGS = -std=c++20 -O3 -DNDEBUG
endif

# External dependencies
LDFLAGS =

INCLUDES = -I$(INCLUDE_DIR) -I$(LIB_DIR)

SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/**/*.cpp) \
       $(wildcard $(LIB_DIR)/*.cpp)
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean test format lint run help earth dry-run earth-run

all: $(BUILD_DIR)/program

$(BUILD_DIR)/program: $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Install and uninstall targets

# install: all
#     install -d $(INSTALL_DIR)/bin
#     install -m 755 $(BUILD_DIR)/program $(INSTALL_DIR)/bin

# uninstall:
#     rm -f $(INSTALL_DIR)/bin/program

run: $(BUILD_DIR)/program
	./$(BUILD_DIR)/program

format:
	clang-format -i ./**/*.cpp
	clang-format -i ./**/*.h 

lint:
	clang-tidy $(SRC_DIR)/**/*.cpp $(SRC_DIR)/**/*.h $(TEST_DIR)/*.cpp -- $(INCLUDES)

clean:
	rm -rf $(BUILD_DIR)

earth:
	earthly +build

earth-run:
	earthly +run

dry-run:
	./build/**

help:
	@echo "This help message made by me to show me of the future how to start and build this project. First of all, it contains earthly so project can be built within docker. Secondly, all project can be built and started using only this makefile. Use make all and make run to made it on your local machine. Or use make earth to build it in docker container so it will not affect local system."
	@echo ""
	@echo "`make help-target`"

help-target:
	@echo "all: Build the project"
	@echo "run: Run the project and build it before"
	@echo "format: Format all files in the project"
	@echo "lint: Lint all files in the project"
	@echo "clean: Clean the project"
	@echo "earth: Build the project in docker container and move it to local machine"
	@echo "earth-run: Run the project in docker container"
	@echo "dry-run: Run the project in docker container without building it"
	@echo "help: Show this help message"