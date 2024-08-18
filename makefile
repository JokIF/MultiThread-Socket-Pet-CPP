RECEIVER = receiver
SENDER = sender
OBJ = obj
BIN = bin
MAIN = main
LIB = lib

CC = g++
RM_RF = rm -rf
CFLAGS = -Wall -Wextra -Werror
RECEIVER_FLAG = -I $(RECEIVER)
SENDER_FLAG = -I $(SENDER)
CPPFLAGS = -MP -MMD
LDLIBS =
DFLAGS = -g


RECEIVER_DIR_OBJ = $(RECEIVER)/$(OBJ)
RECEIVER_MAIN = $(RECEIVER)/$(MAIN)/$(MAIN).cpp
RECEIVER_MAIN_OBJ = $(RECEIVER_DIR_OBJ)/$(MAIN)/$(MAIN).o

RECEIVER_DIRS = $(shell find receiver -type d)
RECEIVER_DIRS_OBJ = $(shell echo $(RECEIVER_DIRS:$(RECEIVER)/%=$(RECEIVER_DIR_OBJ)/%) | sed "0,/$(RECEIVER)/{s/$(RECEIVER)//}")
# RECEIVER_DIRS_OBJ = $(shell echo $(RECEIVER_DIRS) | sed "s/$(RECEIVER)/$(RECEIVER)\/$(OBJ)/")

RECEIVER_LIB = $(shell find $(RECEIVER)/$(LIB)/ -name "*.cpp")
RECEIVER_LIB_OBJ = $(RECEIVER_LIB:$(RECEIVER)/%.cpp=$(RECEIVER)/$(OBJ)/%.o)

RECEIVER_LIB_A = $(RECEIVER)/$(OBJ)/$(LIB)/lib.a
RECEIVER_BIN = $(BIN)/$(RECEIVER)


SENDER_DIR_OBJ = $(SENDER)/$(OBJ)
SENDER_MAIN = $(SENDER)/$(MAIN)/$(MAIN).cpp
SENDER_MAIN_OBJ = $(SENDER_DIR_OBJ)/$(MAIN)/$(MAIN).o

SENDER_DIRS = $(shell find $(SENDER) -type d)
SENDER_DIRS_OBJ = $(shell echo $(SENDER_DIRS:$(SENDER)/%=$(SENDER_DIR_OBJ)/%) | sed "0,/$(SENDER)/{s/$(SENDER)//}")

SENDER_LIB = $(shell find $(SENDER)/$(LIB)/ -name "*.cpp")
SENDER_LIB_OBJ = $(SENDER_LIB:$(SENDER)/%.cpp=$(SENDER_DIR_OBJ)/%.o)

SENDER_LIB_A = $(SENDER)/$(OBJ)/$(LIB)/lib.a
SENDER_BIN = $(BIN)/$(SENDER)

DEPS = $(RECEIVER_MAIN_OBJ:.o=.d) $(SENDER_MAIN_OBJ:.o=.d) $(RECEIVER_LIB_OBJ:.o=.d) $(SENDER_LIB_OBJ:.o=.d)

# check: 
# 	@echo $(SENDER_LIB_OBJ)

.PHONY: all
all: $(BIN) $(RECEIVER_DIR_OBJ) $(SENDER_DIR_OBJ) $(RECEIVER_BIN) $(SENDER_BIN)

-include $(DEPS)

$(BIN):
	mkdir $@

$(RECEIVER_DIR_OBJ):
	mkdir $@ $(RECEIVER_DIRS_OBJ)

$(SENDER_DIR_OBJ):
	mkdir $@ $(SENDER_DIRS_OBJ)

$(RECEIVER_BIN): $(RECEIVER_MAIN_OBJ) $(RECEIVER_LIB_A)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(RECEIVER_FLAG) -o $@ $^ $(LDLIBS) $(DFLAGS)

$(RECEIVER_LIB_A): $(RECEIVER_LIB_OBJ)
	ar rcs $@ $^

$(RECEIVER_DIR_OBJ)/%.o: $(RECEIVER)/%.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(RECEIVER_FLAG) -o $@ $< $(LDLIBS) $(DFLAGS)


$(SENDER_BIN): $(SENDER_MAIN_OBJ) $(SENDER_LIB_A)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SENDER_FLAG) -o $@ $^ $(LDLIBS) $(DFLAGS)

$(SENDER_LIB_A): $(SENDER_LIB_OBJ)
	ar rcs $@ $^

$(SENDER_DIR_OBJ)/%.o: $(SENDER)/%.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(SENDER_FLAG) -o $@ $< $(LDLIBS) $(DFLAGS)

.PHONY: cleanObj
cleanObj:
	$(RM_RF) $(RECEIVER_DIR_OBJ)
	$(RM_RF) $(SENDER_DIR_OBJ)

.PHONY: clean
clean: cleanObj
	$(RM_RF) $(BIN)
