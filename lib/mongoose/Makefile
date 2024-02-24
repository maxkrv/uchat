CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj

MONGOOSE = libmongoose.a

INC := -I$(INC_DIR)
SRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
LINK = -L. -lmongoose

.PHONY: all run install precomp uninstall clean reinstall

all: install

run: install
	@$(CC) $(CFLAGS) main.c $(LINK)

install: $(MONGOOSE)
	@printf "\r\033[K\033[01;36m$(MONGOOSE) $@ \033[33;1mdone\033[01;36m\n"

$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "\r\033[K\033[01;36m$(MONGOOSE) \033[33;1mcompiling \033[01;36m$< "
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(MONGOOSE): $(OBJ)
	@ar rcs $(MONGOOSE) $(OBJ)
	@printf "\r\033[K\033[01;36m$@ \033[32;1mcreated \033[01;36m\n"

uninstall:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(MONGOOSE)
	@printf "\r\033[K\033[01;36m$(MONGOOSE) \033[31;1muninstalled\033[01;36m\n"

clean:
	@rm -rf $(OBJ_DIR)
	@printf "\r\033[K$(OBJ_DIR) in $(MONGOOSE) \033[31;1mcleaned\033[01;36m\n"


reinstall: uninstall install
