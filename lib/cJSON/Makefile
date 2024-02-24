CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj

CJSON = libcjson.a

INC := -I$(INC_DIR)
SRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
LINK = -L. -lcjson

.PHONY: all run install precomp uninstall clean reinstall

all: install

test: install
	@$(CC) $(CFLAGS) src/test.c $(LINK) -o test
	./test


install: $(CJSON)
	@printf "\r\033[K\033[01;36m$(CJSON) $@ \033[33;1mdone\033[01;36m\n"

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "\r\033[K\033[01;36m$(CJSON) \033[33;1mcompiling \033[01;36m$< "
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(CJSON): $(OBJ)
	@ar rcs $(CJSON) $(OBJ)
	@printf "\r\033[K\033[01;36m$@ \033[32;1mcreated \033[01;36m\n"

uninstall:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(CJSON)
	@printf "\r\033[K\033[01;36m$(CJSON) \033[31;1muninstalled\033[01;36m\n"

clean:
	@rm -rf $(OBJ_DIR)
	@printf "\r\033[K$(OBJ_DIR) in $(CJSON) \033[31;1mcleaned\033[01;36m\n"


reinstall: uninstall install
