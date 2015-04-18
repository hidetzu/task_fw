#This makefile makes all the main book code with CppUTest test harness

#Set this to @ to keep the makefile quiet
SILENCE = @

ifndef TARGET
  TARGET=sample
endif

ifndef PROJECT_TOP
  PROJECT_TOP=..
endif

ifndef UNITTEST_TOP
  UNITTEST_TOP=.
endif

ifndef UNITTEST_WORK 
  CPPUTEST_OBJS_DIR=$(PROJECT_TOP)/work
else
  CPPUTEST_OBJS_DIR=$(UNITTEST_WORK)
endif

CPPUTEST_LIB_DIR=$(CPPUTEST_OBJS_DIR)/lib

#---- Outputs ----#
COMPONENT_NAME = $(TARGET)_CppUTest

#--- Inputs ----#
CPPUTEST_HOME=$(UNITTEST_TOP)/cpputest-3.6

CPP_PLATFORM = Gcc
PROJECT_HOME_DIR=$(PROJECT_TOP)

SRC_DIRS = \
	$(PROJECT_HOME_DIR)/src \
	$(PROJECT_HOME_DIR)/src/taskFw \
#	src/ \

TEST_SRC_DIRS = \
	$(UNITTEST_TOP)/tests \
	$(UNITTEST_TOP)/tests/taskFw \
	$(UNITTEST_TOP)/stub \

	
INCLUDE_DIRS =\
  $(CPPUTEST_HOME)/include\
  $(PROJECT_HOME_DIR)/include\

MOCKS_SRC_DIRS = \
	$(UNITTEST_TOP)/mocks
	
CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_USE_GCOV = Y
CPPUTEST_WARNINGFLAGS = -Wall -Wswitch-default -Werror 
CPPUTEST_CFLAGS = -std=c99 
CPPUTEST_CFLAGS += -DUNITTEST_ENABLE
CPPUTEST_CFLAGS += -Wall -Wstrict-prototypes -pedantic
LD_LIBRARIES = -lpthread
	
  
include $(CPPUTEST_HOME)/build/MakefileWorker.mk

