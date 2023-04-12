empty :=
space := $(empty) $(empty)

SRCDIR := src
INCDIR := include
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
DEPDIR := $(BUILDDIR)/deps
TARGETDIR := $(BUILDDIR)/target
BINDIR := .

SHELL := /bin/bash

MY_FLAGS := -I$(BINDIR) -I$(INCDIR)

CC = clang
CFLAGS = $(MY_FLAGS) -Wall -Wextra -Wno-unused-result -Wno-unused-command-line-argument
CXX = clang++
CXXFLAGS = $(CFLAGS) -std=c++17
LD = clang++
LDFLAGS = $(CXXFLAGS)
DEBUGGER = lldb

ifeq ($(RELEASE), 1)
	maketype := release
	CFLAGS += -O2 -ftree-vectorize -fomit-frame-pointer -march=native
	# Link Time Optimization
	CFLAGS += -flto
else
	maketype := debug
	CFLAGS += -Og -ggdb3 -DDEBUG=1
	# Overflow protection
	CFLAGS += -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fcf-protection
	CFLAGS += -Wl,-z,defs -Wl,-z,now -Wl,-z,relro
	CXXFLAGS += -D_GLIBCXX_ASSERTIONS
	CFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
endif

CFLAGS += -MMD -MP

SRCS := $(shell find $(SRCDIR) -regextype posix-extended -regex .*[.]c\(pp\|\) 2>/dev/null)

OBJS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/$(maketype)/%.o,$(SRCS))
DEPS := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$(SRCS))

COMPILE = echo $(1) $(maketype) $(2) "->" $(3) && \
		$($(1)) -c $(2) -o $(3) -MF $(4) $(5)
LINK = echo LD $(maketype) $(1) $(2) && \
		$(LD) -o $(2) $(3) $(LDFLAGS)

TARGET := $(TARGETDIR)/$(maketype)/$(notdir $(subst $(space),_,$(realpath .)))

% : %.cpp
	@$(call LINK,$@,$@,$^)
	@rm $@.d

.PHONY: all
all : $(TARGET)

.PHONY: getTarget
getTarget :
	@echo $(TARGET)

.PHONY: init
init :
	-@rm -rf build
	@mkdir -p $(SRCDIR) $(INCDIR)
	@for i in $(wildcard *.cpp) $(wildcard *.c); do mv ./$$i $(SRCDIR)/$$i; done
	@for i in $(wildcard *.hpp) $(wildcard *.h); do mv ./$$i $(INCDIR)/$$i; done
	@$(file >compile_flags.txt)
	@$(foreach i,$(CXXFLAGS),$(file >>compile_flags.txt,$(i)))

$(TARGET) : $(OBJS)
	@mkdir -p $(@D)
	@$(call LINK,"ALL ->",$@,$^)

$(OBJDIR)/$(maketype)/%.cpp.o : $(SRCDIR)/%.cpp
	@$(eval CUR_DEP := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$<))
	@mkdir -p $(@D) $(dir $(CUR_DEP))
	-@$(call COMPILE,CXX,$<,$@,$(CUR_DEP),$(CXXFLAGS))

$(OBJDIR)/$(maketype)/%.c.o : $(SRCDIR)/%.c
	@$(eval CUR_DEP := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$<))
	@mkdir -p $(@D) $(dir $(CUR_DEP))
	-@$(call COMPILE,CC,$<,$@,$(CUR_DEP),$(CFLAGS))

.PHONY: clean
clean : 
	-rm -rf $(OBJDIR)/$(maketype)/* $(DEPDIR)/* $(TARGETDIR)/$(maketype)/*

.PHONY: debug
debug : $(TARGET)
	@export ASAN_OPTIONS=detect_leaks=0; \
		$(DEBUGGER) $(TARGET)

-include $(DEPS)
