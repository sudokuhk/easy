cxx=g++
SERVER = main
#SOURCES = $(wildcard *.cpp)

BASE_PATH	= $(shell pwd)
COMMON_PATH = $(BASE_PATH)/common

INC_PATH 	= $(BASE_PATH)

COMMON_SOURCES = $(COMMON_PATH)/thread/ea_condition.cpp		\
				 $(COMMON_PATH)/thread/ea_condition_posix.cpp	\
				 $(COMMON_PATH)/thread/ea_criticalsection.cpp	\
				 $(COMMON_PATH)/thread/ea_criticalsection_guard.cpp	\
				 $(COMMON_PATH)/thread/ea_criticalsection_posix.cpp	\
				 $(COMMON_PATH)/thread/ea_thread.cpp	\
				 $(COMMON_PATH)/thread/ea_thread_posix.cpp	\
				 $(COMMON_PATH)/log/ea_log.cpp	\
				 $(COMMON_PATH)/net/ea_basesocket.cpp	\
				 $(COMMON_PATH)/net/ea_tcpsocket.cpp	\
				 $(COMMON_PATH)/net/ea_nethelper.cpp	\
				 $(COMMON_PATH)/net/ea_buffer.cpp	\
				 $(COMMON_PATH)/net/ea_simplebuffer.cpp	\
				 $(COMMON_PATH)/net/ea_epoll.cpp	\
				 $(COMMON_PATH)/net/ea_timer.cpp	\

INC_PATH	+= $(COMMON_PATH)/ 	\

SERVER_SOURCES = main.cpp 						\

SOURCES = $(SERVER_SOURCES) $(COMMON_SOURCES)
DEP_SOURCES = $(notdir $(SOURCES))

SERVER_OBJECTS = $(patsubst %.cpp,%.o, $(DEP_SOURCES))

LIBRARYS= -pthread 
#LIB_PATH = /home/sudoku/boost/sdk/lib
MACROS = #-DC_STRING_FUNC -DLOG_PRINTF -DFT_LOG_DEBUG
SHARED = #-shared -fPIC
CPPFLAGS += -g -O0 -Wno-deprecated -Wnonnull 

INC_PATH += 

#####################################################
INC_DIR = $(patsubst %, -I%, $(INC_PATH))
LIB_DIR = $(patsubst %, -L%, $(LIB_PATH))

all : $(SERVER)

$(SERVER) : $(SERVER_OBJECTS)
	$(cxx) $(MACROS) $(CPPFLAGS) $(SHARED) -o $@ $^ $(INC_DIR) $(LIB_DIR) $(LIBRARYS)

.PHONY : clean
clean:
	-rm -f $(SERVER)
	-rm -f *.o
	-rm -f *.d

ifneq "$(MAKECMDGOALS)" "clean"
    include $(DEP_SOURCES:.cpp=.d)  
endif

%.d : 
	@t1=$*; t2=$${t1##*/}; \
	for file in $(SOURCES); \
        do \
                find=$${file%%$$t2.cpp};        \
                        find=$$find$$t2.cpp;            \
                        split=$${file##*/};     \
                        cmpfile=$$t2.cpp;       \
                        if [ $$split = $$cmpfile ] && [ $${find} = $${file} ]; then     \
                rely_file=$$file;       \
                fi;     \
        done;   \
    set -e; rm -f $@; \
	$(cxx) -MM $(CPPFLAGS) $(INC_DIR) $$rely_file > $@.;	\
    cat $@. > $@;   \
	echo "	$(cxx) $(CPPFLAGS) $(MACROS) $(SHARED) -c -o $$t2.o $(INC_DIR) $$rely_file" >> $@; \
    sed 's/\.o/\.d/g' < $@. >> $@; \
    rm -f $@.       ;
