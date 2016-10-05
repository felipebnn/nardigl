##################
# Make for C/C++ #
# v1.6           #
##################

OUTPUT_FILE := main
ATTR_ALL := -MMD -Wall
ATTR_GPP := -O2 -std=c++14
ATTR_OUT := -lglfw3 -lglmf32 -lopengl32
ATTR_GCC :=

LIB_FILES := $(shell find lib/ -name '*.a')
LIB_FOLDER := -Llib
INCLUDE_FOLDER := -Iinclude -Isrc

SRCS_GPP := $(shell find src/ -name '*.cpp')
SRCS_GCC := $(shell find src/ -name '*.c')
DEPS := $(SRCS_GPP:src/%.cpp=obj/%.d) $(SRCS_GCC:src/%.c=obj/%.d)
OBJS := $(SRCS_GPP:src/%.cpp=obj/%.o) $(SRCS_GCC:src/%.c=obj/%.o)

all: build
default: build
build: create_obj_folders $(OUTPUT_FILE)
rebuild: clean build

RUN_MSG := Running...
run: build
	@echo $(RUN_MSG)
	@./$(OUTPUT_FILE)

clean:
	@echo Cleaning objects...
	@rm -rf obj

create_project:
	@mkdir -p src;
	@mkdir -p lib;
	@mkdir -p include;

create_obj_folders: #cria pastas necessarias
	@mkdir -p $(OBJS:%.o=%);rm -rf $(OBJS:%.o=%)

##################################################################

$(OUTPUT_FILE): $(OBJS)
	@echo Building $@
	@g++ -o $@ $(OBJS) $(LIB_FOLDER) $(ATTR_OUT) $(LIB_FILES)

obj/%.o: src/%.cpp
	@echo Building $@
	@g++ $(ATTR_ALL) $(ATTR_GPP) $(INCLUDE_FOLDER) -c $< -o $@

obj/%.o: src/%.c
	@echo Building $@
	@gcc $(ATTR_ALL) $(ATTR_GCC) $(INCLUDE_FOLDER) -c $< -o $@

-include $(DEPS)
