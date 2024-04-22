MV = mv
RM = rm

LIB_DIR = lib

LIBMX = $(LIB_DIR)/libmx/libmx.a
LIBMONGOOSE = $(LIB_DIR)/mongoose/libmongoose.a
LIBCJSON = $(LIB_DIR)/cJSON/libcjson.a
LIBSQLITE3 = $(LIB_DIR)/sqlite3/libsqlite3.a
CLIENT = client/uchat
SERVER = server/uchat_server

TARGET_CLIENT = uchat
TARGET_SERVER = uchat_server

.PHONY: all uninstall clean reinstall install server client

all: install

server: install
	@$(MAKE) -sC $(dir $(SERVER)) install
	@$(MV) $(SERVER) ./$(TARGET_SERVER)
	./$(TARGET_SERVER) 3000 --secret SECRET 
client: install
	@$(MAKE) -sC $(dir $(CLIENT))
	@$(MV) $(CLIENT) ./$(TARGET_CLIENT)
	./$(TARGET_CLIENT) localhost 3000

install: $(LIBMX) $(LIBMONGOOSE) $(LIBCJSON) $(LIBSQLITE3) $(TARGET_SERVER) $(TARGET_CLIENT)

$(LIBMX):
	@$(MAKE) -sC $(dir $@) install

$(LIBMONGOOSE):
	@$(MAKE) -sC $(dir $@) install

$(LIBCJSON):
	@$(MAKE) -sC $(dir $@) install

$(LIBSQLITE3):
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
	@$(MAKE) -sC $(dir $(LIBSQLITE3)) uninstall
	@$(MAKE) -sC $(dir $(SERVER)) uninstall
	@$(MAKE) -sC $(dir $(CLIENT)) uninstall
	@$(RM) -rf $(TARGET_CLIENT) $(TARGET_SERVER)

clean:
	@$(MAKE) -sC $(dir $(LIBMX)) clean
	@$(MAKE) -sC $(dir $(LIBMONGOOSE)) clean
	@$(MAKE) -sC $(dir $(LIBCJSON)) clean
	@$(MAKE) -sC $(dir $(LIBSQLITE3)) clean
	@$(MAKE) -sC $(dir $(SERVER)) clean
	@$(MAKE) -sC $(dir $(CLIENT)) clean

reinstall: uninstall install
