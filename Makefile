# Makefile template for shared library

CC = gcc --std=c99
CXX = g++ --std=c++11
CFLAGS = -fPIC -Wall -Wextra -O2 -g
CFLAGS += $(shell pkg-config --cflags json-c)
CFLAGS += $(shell pkg-config --cflags libcurl)
CFLAGS += -I$(SOAR_HOME)/include
LDFLAGS += $(shell pkg-config --libs json-c)
LDFLAGS += $(shell pkg-config --libs libcurl)
LDFLAGS += -L$(SOAR_HOME) -lSoar -lpthread -ldl
RM = rm -f
TARGET = ttt-agent
HDRS = tic_tac_toe.h soarTTT.h
OBJS = tic_tac_toe.o ttt_agent.o soarTTT.o

.PHONY: all
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

tic_tac_toe.o: tic_tac_toe.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

ttt_agent.o: ttt_agent.cpp $(HDRS)
	$(CXX) $(CFLAGS) -c $< -o $@

soarTTT.o: soarTTT.cpp $(HDRS)
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-${RM} ${TARGET} ${OBJS}
