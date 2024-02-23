MV = mv
RM = rm

LIB_DIR = lib

LIBMX = $(LIB_DIR)/libmx/libmx.a
LIBMONGOOSE = $(LIB_DIR)/mongoose/libmongoose.a
LIBCJSON = $(LIB_DIR)/cJSON/libcjson.a
CLIENT = client/uchat
SERVER = server/uchat_server

TARGET_CLIENT = uchat
TARGET_SERVER = uchat_server

.PHONY: all uninstall clean reinstall install server client

all: install

server: install
	./$(TARGET_SERVER)

client: install
	./$(TARGET_CLIENT)

install: $(LIBMX) $(LIBMONGOOSE) $(LIBCJSON) $(TARGET_SERVER) $(TARGET_CLIENT)

$(LIBMX):
	@$(MAKE) -sC $(dir $@) install

$(LIBMONGOOSE):
	@$(MAKE) -sC $(dir $@) install

$(LIBCJSON):
	@$(MAKE) -sC $(dir $@) install

$(TARGET_CLIENT):
	@$(MAKE) -sC $(dir $(CLIENT)) install
	@$(MV) $(CLIENT) ./$(TARGET_CLIENT)

$(TARGET_SERVER):
	@$(MAKE) -sC $(dir $(SERVER)) install
	@$(MV) $(SERVER) ./$(TARGET_SERVER)

uninstall: clean
	@$(MAKE) -sC $(dir $(LIBMX)) uninstall
	@$(MAKE) -sC $(dir $(LIBMONGOOSE)) uninstall
	@$(MAKE) -sC $(dir $(LIBCJSON)) uninstall
	@$(MAKE) -sC $(dir $(SERVER)) uninstall
	@$(MAKE) -sC $(dir $(CLIENT)) uninstall
	@$(RM) -rf $(TARGET_CLIENT) $(TARGET_SERVER)

clean:
	@$(MAKE) -sC $(dir $(LIBMX)) clean
	@$(MAKE) -sC $(dir $(LIBMONGOOSE)) clean
	@$(MAKE) -sC $(dir $(LIBCJSON)) clean
	@$(MAKE) -sC $(dir $(SERVER)) clean
	@$(MAKE) -sC $(dir $(CLIENT)) clean

reinstall: uninstall install
