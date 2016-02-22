include config.mk

# OS-specific config
ifeq ($(OSTARGET), Linux)
CC=gcc
PKGCONFIG=pkg-config
#OSTARGETFLAGS=$(LINUXFLAGS)
#OSTARGETLIBS=$(LINUXLIBS)
else ifeq ($(OSTARGET), Windows)
#OSTARGETFLAGS=$(WINFLAGS)
#OSTARGETLIBS=$(WINLIBS)
CC=i686-w64-mingw32-gcc
PKGCONFIG=i686-w64-mingw32-pkg-config
else
	$(error Unknown OS selected for output. Possible options: Linux, Windows)
endif

# Flags
CFLAGS=-Wall -Werror `$(PKGCONFIG) --cflags dfgame-common`
CLIBS=`$(PKGCONFIG) --libs dfgame-common`
GAMEFLAGS=-I$(SRCPATH)/$(LIBGAMETARGET) `$(PKGCONFIG) --cflags dfgame-game-front`
GAMELIBS=`$(PKGCONFIG) --libs dfgame-game-front`
LIBGAMEFLAGS=`$(PKGCONFIG) --cflags dfgame-game`
LIBGAMELIBS=-lm `$(PKGCONFIG) --libs dfgame-game`

# Type-specific config
ifeq ($(BUILDTYPE), Debug)
	CFLAGS += -g
else ifeq ($(BUILDTYPE), Release)
	CFLAGS += -O3
else
	$(error Unknown build type selected for output. Possible options: Debug, Release)
endif

# Targets
GAMETARGET=$(APPNAME)
LIBGAMETARGET=lib$(APPNAME)

# Dependency lists
GAMESRCS:=$(wildcard $(SRCPATH)/$(GAMETARGET)/*.c)
GAMEOBJS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.o,$(GAMESRCS))
GAMEDEPS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.depend,$(GAMESRCS))

LIBGAMESRCS:=$(wildcard $(SRCPATH)/$(LIBGAMETARGET)/*.c)
LIBGAMEOBJS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.o,$(LIBGAMESRCS))
LIBGAMEDEPS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.depend,$(LIBGAMESRCS))

# Directory inits
$(shell mkdir -p $(OBJPATH))
$(shell mkdir -p $(LIBPATH))
$(shell mkdir -p $(OBJPATH)/$(GAMETARGET))
$(shell mkdir -p $(OBJPATH)/$(LIBGAMETARGET))

# Filetype rules
$(OBJPATH)/$(GAMETARGET)/%.depend: $(SRCPATH)/$(GAMETARGET)/%.c
	@echo -e "Building dependecies for $<..."
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $(GAMEFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)/$(LIBGAMETARGET)/%.depend: $(SRCPATH)/$(LIBGAMETARGET)/%.c
	@echo -e "Building dependecies for $<..."
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $(LIBGAMEFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)/$(GAMETARGET)/%.o: $(SRCPATH)/$(GAMETARGET)/%.c
	@echo -e "Building $<..."
	$(CC) -c $< -o $@ $(GAMEFLAGS)

$(OBJPATH)/$(LIBGAMETARGET)/%.o: $(SRCPATH)/$(LIBGAMETARGET)/%.c
	@echo -e "Building $<..."
	$(CC) -c $< -o $@ $(LIBGAMEFLAGS)

all: $(LIBGAMETARGET) $(GAMETARGET)

$(GAMETARGET): $(GAMEOBJS) $(GAMEDEPS)
	gcc -o $(GAMETARGET) $(GAMEFLAGS) $(GAMEOBJS) $(LIBPATH)/$(LIBGAMETARGET).a $(GAMELIBS)

$(LIBGAMETARGET): $(LIBGAMEOBJS) $(LIBGAMEDEPS)
	ar -rcs $(LIBPATH)/$(LIBGAMETARGET).a $(LIBGAMEOBJS)

# Dynamic includes
-include $(GAMEDEPS)
-include $(LIBGAMEDEPS)
