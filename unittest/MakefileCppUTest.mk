#This makefile makes all the main book code with CppUTest test harness

#Set this to @ to keep the makefile quiet
SILENCE = @

ifndef TARGET
  TARGET=sample
endif


#---- Outputs ----#
COMPONENT_NAME = $(TARGET)_CppUTest

#--- Inputs ----#
CPPUTEST_HOME = cpputest-3.6

CPP_PLATFORM = Gcc


PROJECT_HOME_DIR = ..

SRC_DIRS = \
	$(PROJECT_HOME_DIR)/src \
	$(PROJECT_HOME_DIR)/src/taskFw \
#	src/ \

TEST_SRC_DIRS = \
	tests\
	tests/taskFw\
	stub\

	
INCLUDE_DIRS =\
  $(CPPUTEST_HOME)/include\
  $(PROJECT_HOME_DIR)/include\

MOCKS_SRC_DIRS = \
	mocks\
	
CPPUTEST_WARNINGFLAGS = -Wall -Wswitch-default -Werror 
CPPUTEST_CFLAGS = -std=c99 
CPPUTEST_CFLAGS += -Wall -Wstrict-prototypes -pedantic
LD_LIBRARIES = -lpthread
	
  
include $(CPPUTEST_HOME)/build/MakefileWorker.mk

