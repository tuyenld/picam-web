PROG ?= picam-web
LDFLAGS=-lmongoose

# MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK
# MODULE_CFLAGS = -DMG_ENABLE_HTTP_STREAMING_MULTIPART
CFLAGS_EXTRA = -g -W -Wall -Werror -Wno-unused-function $(MODULE_CFLAGS)
# CROSS_COMPILE	?= 
# MONGOOSE_DIR	?= $(STAGING_DIR)/usr/

# CC		:= $(CROSS_COMPILE)gcc
# KERNEL_INCLUDE	:= -I$(MONGOOSE_DIR)/include
# CFLAGS		:= -W -Wall -g $(KERNEL_INCLUDE)

all: $(PROG)

$(PROG):
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) $(LIBS) $(EXTRA) -o $(PROG) main.c $(LDFLAGS)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb log.txt
