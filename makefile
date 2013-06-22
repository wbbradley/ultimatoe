UNAME := $(shell uname)
DEBUG_FLAGS := -DDEBUG -g -O0
NDEBUG_FLAGS := -g -O3

CPP = clang++ -std=c++11 -stdlib=libc++ -DMACOS
LINKER = clang++ -stdlib=libc++
LINKER_OPTS := $(NDEBUG_FLAGS)
LINKER_DEBUG_OPTS := $(DEBUG_FLAGS)

BUILD_DIR = build

CFLAGS := \
	-c \
	-Wall \
	-pthread \
	-DDEBUG \
	-g \

STR_REPL_SOURCES = \
				 ultimatoe.cpp \
				 disk.cpp \
				 mmap_file.cpp \
				 cmd_options.cpp \
				 logger.cpp \
				 utils.cpp \

STR_REPL_OBJECTS = $(addprefix $(BUILD_DIR)/,$(STR_REPL_SOURCES:.cpp=.o))
STR_REPL_TARGET = ultimatoe

TARGETS = $(STR_REPL_TARGET)

all: build-dir $(TARGETS)

build-dir:
	@if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi

.PHONY: test

test: $(STR_REPL_TARGET)
	@echo Test passed.

$(STR_REPL_TARGET): $(STR_REPL_OBJECTS)
	$(LINKER) $(LINKER_OPTS) $(STR_REPL_OBJECTS) -o $(STR_REPL_TARGET)

$(BUILD_DIR)/%.o: %.cpp
	$(CPP) $(CFLAGS) $< -o $@

CLEAN = rm -rf $(BUILD_DIR)/*.o $(TARGETS)

clean:
	$(CLEAN)

