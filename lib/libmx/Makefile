CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic  -Werror

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj

LIBMX = libmx.a

INC := -I$(INC_DIR)
SRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
LINK = -L. -lmx

.PHONY: all run install precomp uninstall clean reinstall

all: install

run: install
	@$(CC) $(CFLAGS) main.c $(LINK)

install: $(LIBMX)
	@printf "\r\033[K\033[01;36m$(LIBMX) $@ \033[33;1mdone\033[01;36m\n"

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "\r\033[K\033[01;36m$(LIBMX) \033[33;1mcompiling \033[01;36m$< "
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(LIBMX): $(OBJ)
	@ar rcs $(LIBMX) $(OBJ)
	@printf "\r\033[K\033[01;36m$@ \033[32;1mcreated \033[01;36m\n"

uninstall:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(LIBMX)
	@printf "\r\033[K\033[01;36m$(LIBMX) \033[31;1muninstalled\033[01;36m\n"

clean:
	@rm -rf $(OBJ_DIR)
	@printf "\r\033[K$(OBJ_DIR) in $(LIBMX) \033[31;1mcleaned\033[01;36m\n"


reinstall: uninstall install
