CPPC = g++
CC = gcc
INCLUDES = includes

CPPFLAGS = -I$(INCLUDES) -Werror -std=c++23 -MMD -MP
CFLAGS = -I$(INCLUDES) -MMD -MP
LDFLAGS = -lglfw -ldl -lGL -lnoise -lm

CPP_SOURCES := $(shell find src -name '*.cpp')
C_SOURCES   := $(shell find includes -name 'glad.c')

OBJS := $(CPP_SOURCES:%.cpp=build/%.o) $(C_SOURCES:%.c=build/%.o)
DEPS := $(OBJS:.o=.d)

all: bin/executable

bin/executable: $(OBJS)
	@mkdir -p bin
	$(CPPC) $^ -o $@ $(LDFLAGS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPPC) $(CPPFLAGS) -c $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin

-include $(DEPS)

.PHONY: all clean