UNAME := $(shell uname)
DEBUG_FLAGS := -DDEBUG -g -O0
NDEBUG_FLAGS := -O3 -g

CPP = clang -x c++ -arch x86_64 -std=gnu++11 -stdlib=libc++ -DMACOS
LINKER = clang -arch x86_64 -stdlib=libc++ -lc++
#-lstdc++
LINKER_OPTS := $(DEBUG_FLAGS)
LINKER_DEBUG_OPTS := $(DEBUG_FLAGS)
PREPP := python tools/prepp/prepp.py cpp

BUILD_DIR = build

CFLAGS := \
	-I. \
	-c \
	-Wall \
	-pthread \
	-DDEBUG \
	-g \

ULTIMATOE_PREPP_SOURCES = \
	ultimatoe.prepp \

ULTIMATOE_CPP_SOURCES = \

ULTIMATOE_OBJECTS = \
	$(addprefix $(BUILD_DIR)/,$(ULTIMATOE_PREPP_SOURCES:.prepp=.o)) \
	$(addprefix $(BUILD_DIR)/,$(ULTIMATOE_CPP_SOURCES:.cpp=.o))

ULTIMATOE_TARGET = ultimatoe

TARGETS = $(ULTIMATOE_TARGET)

all: build-dir $(TARGETS)

build-dir:
	@if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi

.PHONY: test

test: $(ULTIMATOE_TARGET)
	@echo Running tests...
	./ultimatoe -t
	@echo Test passed.

$(ULTIMATOE_TARGET): $(ULTIMATOE_OBJECTS)
	$(LINKER) $(LINKER_OPTS) $(ULTIMATOE_OBJECTS) -o $(ULTIMATOE_TARGET)

$(BUILD_DIR)/%.o: %.cpp
	$(CPP) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.ii: $(BUILD_DIR)/%.prepp.cpp
	$(CPP) $(CFLAGS) -E $< > $@

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.ii
	$(CPP) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.prepp.cpp: %.prepp
	$(PREPP) $< > $@

CLEAN = rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.cpp $(TARGETS)

clean:
	$(CLEAN)

