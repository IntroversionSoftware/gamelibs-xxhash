# -*- Makefile -*- for xxhash

.SECONDEXPANSION:
.SUFFIXES:

ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
        QUIET          = @
        QUIET_CC       = @echo '   ' CC $<;
        QUIET_AR       = @echo '   ' AR $@;
        QUIET_RANLIB   = @echo '   ' RANLIB $@;
        QUIET_INSTALL  = @echo '   ' INSTALL $<;
        export V
endif
endif

LIB    = libxxhash.a
AR    ?= ar
ARFLAGS ?= rc
CC    ?= gcc
RANLIB?= ranlib
RM    ?= rm -f

BUILD_DIR := build
BUILD_ID  ?= default-build-id
OBJ_DIR   := $(BUILD_DIR)/$(BUILD_ID)

ifeq (,$(BUILD_ID))
$(error BUILD_ID cannot be an empty string)
endif

prefix ?= /usr/local
libdir := $(prefix)/lib
includedir := $(prefix)/include

HEADERS = xxh_x86dispatch.h xxhash.h
SOURCES = xxh_x86dispatch.c xxhash.c

HEADERS_INST := $(patsubst %,$(includedir)/%,$(HEADERS))
OBJECTS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

CFLAGS ?= -O2
CPPFLAGS += -I. -DXXH_DISPATCH_AVX512=0

.PHONY: install

all: $(OBJ_DIR)/$(LIB)

$(includedir)/%.h: %.h
	-@if [ ! -d $(includedir) ]; then mkdir -p $(includedir); fi
	$(QUIET_INSTALL)cp $< $@
	@chmod 0644 $@

$(libdir)/%.a: $(OBJ_DIR)/%.a
	-@if [ ! -d $(libdir)  ]; then mkdir -p $(libdir); fi
	$(QUIET_INSTALL)cp $< $@
	@chmod 0644 $@

install: $(HEADERS_INST) $(libdir)/$(LIB)

clean:
	$(RM) -r $(OBJ_DIR)

distclean: clean
	$(RM) -r $(BUILD_DIR)

$(OBJ_DIR)/$(LIB): $(OBJECTS) | $$(@D)/.
	$(QUIET_AR)$(AR) $(ARFLAGS) $@ $^
	$(QUIET_RANLIB)$(RANLIB) $@

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)/.cflags | $$(@D)/.
	$(QUIET_CC)$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

.PRECIOUS: $(OBJ_DIR)/. $(OBJ_DIR)%/.

$(OBJ_DIR)/.:
	$(QUIET)mkdir -p $@

$(OBJ_DIR)%/.:
	$(QUIET)mkdir -p $@

TRACK_CFLAGS = $(subst ','\'',$(CC) $(CFLAGS) $(CPPFLAGS))

$(OBJ_DIR)/.cflags: .force-cflags | $$(@D)/.
	@FLAGS='$(TRACK_CFLAGS)'; \
    if test x"$$FLAGS" != x"`cat $(OBJ_DIR)/.cflags 2>/dev/null`" ; then \
        echo "    * rebuilding xxhash: new build flags or prefix"; \
        echo "$$FLAGS" > $(OBJ_DIR)/.cflags; \
    fi

.PHONY: .force-cflags
