EXE 	 := mycc

SRCDIR   := src
OBJDIR   := obj

SRCS 	 := $(shell find $(SRCDIR) -name '*.c')
SRCDIRS  := $(shell find . -name '*.c' -exec dirname {} \; | uniq)
OBJS 	 := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

CC 		 := gcc
CFLAGS   := -Wall -g
CPPFLAGS := -Iinclude

all: $(EXE)

$(EXE) : buildrepo $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "\n'$(EXE)' compiled!"

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@echo "\ncleaned!"

distclean: clean
	$(RM) $(EXE)

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef