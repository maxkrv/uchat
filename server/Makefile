CC = clang
RM = rm
PRINT = printf
MKDIR = mkdir
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic #-Werror #todo uncomment this please

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj

LIB_DIR = ../lib

LIBMX = $(LIB_DIR)/libmx/libmx.a
LIBMONGOOSE = $(LIB_DIR)/mongoose/libmongoose.a
LIBCJSON = $(LIB_DIR)/cJSON/libcjson.a
LIBSQLITE3 = $(LIB_DIR)/sqlite3/libsqlite3.a

TARGET = uchat_server

INC_FILES := -I$(INC_DIR) -I$(dir $(LIBMX))/inc -I$(dir $(LIBMONGOOSE))/inc -I$(dir $(LIBCJSON))/inc -I$(dir $(LIBSQLITE3))/inc -lcurl
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
LINKS := -L$(dir $(LIBMX)) -lmx -L$(dir $(LIBMONGOOSE)) -lmongoose -L$(dir $(LIBCJSON))  -lcjson -L$(dir $(LIBSQLITE3)) -lsqlite3 -lcurl

RED = \033[31;1m
GREEN = \033[32;1m
YELLOW = \033[33;1m
CYAN = \033[36;1m

BOLD = \033[1m
CLEAN_LINE = \033[K

.PHONY: all uninstall clean reinstall install

all: reinstall

install: $(TARGET)
	@$(PRINT) "\r$(CLEAN_LINE)$(CYAN)$(BOLD)$(TARGET) $@ $(YELLOW)done$(CYAN)\n"


$(OBJ_FILES): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) -p $(dir $@)
	@$(PRINT) "\r$(CLEAN_LINE)$(CYAN)$(BOLD)$(TARGET) $(YELLOW)compiling $(CYAN)$< "
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC_FILES)

$(TARGET): $(OBJ_FILES)
	@$(PRINT) "\r$(CLEAN_LINE)$(CYAN)$(BOLD)$@ $(YELLOW)files compiled$(CYAN)\n"
	@$(CC) $(CFLAGS) -o $@ $^ $(INC_FILES) $(LINKS)
	@$(PRINT) "\r$(CLEAN_LINE)$(CYAN)$(BOLD)$@ $(GREEN)created$(CYAN)\n"

uninstall:
	@$(RM) -rf $(OBJ_DIR)
	@$(RM) -rf $(TARGET)
	@$(PRINT) "\r$(CLEAN_LINE)$(TARGET) $(RED)uninstalled$(CYAN)\n"

clean:
	@$(RM) -rf $(OBJ_DIR)
	@$(PRINT) "\r$(CLEAN_LINE)$(OBJ_DIR) in $(TARGET) $(RED)cleaned$(CYAN)\n"

reinstall: uninstall install
