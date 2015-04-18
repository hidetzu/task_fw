CC     = gcc
AT     = @
MKDIR  = mkdir -p
RM     = rm -rf


##################
# configs
#
USE_DEBUG=yes
USE_COV=yes

TARGET=taskFw


INCLUDES  = -I./include
CFLAGS    = -Wall
LDFLAGS   = -lpthread

ifeq ($(USE_DEBUG), yes)
CFLAGS += -g
endif

ifeq ($(USE_COV), yes)
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS+= -fprofile-arcs
endif

#SRC_DIR=$(shell find src -name *.c)
SRC_DIR=src\
   src/taskFw\
#  src  \

############
# project dir settings
PROJECT_TOP=$(shell pwd)
MAKE_DIR=$(PROJECT_TOP)/build
OUTPUT_DIR=$(PROJECT_TOP)/work
BIN_DIR=$(OUTPUT_DIR)/bin

############
# helpers
include build/helpers.mk

SRCS += $(call get_src_from_dir_list, $(SRC_DIR))
SRCS += main.c

objects = $(call src_to_o, $(SRCS))


.PHONY: all clean
all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET):$(objects)
	@echo linking $(notdir $<)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OUTPUT_DIR)/%.o:%.c
	@echo compiling $(notdir $<)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

lcov:
	make --no-print-directory SRC_DIR="$(SRC_DIR)" OUTPUT_DIR="$(OUTPUT_DIR)" -f $(MAKE_DIR)/lcov.mk

clean:
	@rm -rf $(OUTPUT_DIR) $(TARGET)


###################
# for unittest
#
.PHONY: unittest unittest_clean

unittest:
	cd unittest; make TARGET=$(TARGET) -f MakefileCppUTest.mk

unittest_clean:
	cd unittest; make TARGET=$(TARGET) -f MakefileCppUTest.mk clean
