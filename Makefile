CJSON = libcjson
SQLITE = libsqlite3

CJSON_DIR = lib/cjson
SQLITE_DIR = lib/sqlite3
LIB_DIR = lib/inc lib/obj

UTILSO = $(UTILSD)/$(UTILS).a
CJSONO = lib/obj/$(CJSON).a
SQLITEO = lib/obj/$(SQLITE).a

all: $(SQLITE) $(CJSON)

$(SQLITE): $(SQLITEO)
$(CJSON): $(CJSONO)

$(SQLITEO): $(SQLITE_DIR)
$(CJSONO): $(CJSON_DIR)

$(SQLITEO) $(CJSONO): | $(LIB_DIR)
	@make -sC $<

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

uninstall: clean
	@make -sC $(SQLITE_DIR) $@
	@make -sC $(CJSON_DIR) $@
	@rm -rf $(LIB_DIR)

clean:
	@make -sC $(SQLITE_DIR) $@
	@make -sC $(CJSON_DIR) $@

reinstall: uninstall all

.PHONY: + uninstall clean reinstall $(SQLITEO) $(CJSONO)

