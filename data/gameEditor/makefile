include config.mk

# OS-specific config
ifeq ($(OSTARGET), Linux)
CC=gcc
VALAC=valac
PKGCONFIG=pkg-config
#OSTARGETFLAGS=$(LINUXFLAGS)
#OSTARGETLIBS=$(LINUXLIBS)
else ifeq ($(OSTARGET), Windows)
#OSTARGETFLAGS=$(WINFLAGS)
#OSTARGETLIBS=$(WINLIBS)
CC=i686-w64-mingw32-gcc
VALAC=i686-w64-mingw32-valac
PKGCONFIG=i686-w64-mingw32-pkg-config
else
	$(error Unknown OS selected for output. Possible options: Linux, Windows)
endif

# Flags
VFLAGS=-c --vapidir=$(VAPIPATH) -X -I$(SRCPATH)/$(LIBEDITORTARGET) -X -I$(SRCPATH)/$(LIBGAMETARGET) -X -I/usr/include/dfgame/common/ --cc=$(CC)
VLIBS=--pkg=glib-2.0 --pkg=gtk+-3.0 --pkg=gee-0.8 --pkg=libxml-2.0 --pkg=dflib --pkg=dfgame-editor-frontend --pkg dfgame-editor --pkg dfgame-game --pkg=$(LIBEDITORTARGET) --pkg=$(LIBGAMETARGET)
CFLAGS=-Wall -Werror -D BUILD_TYPE=$(BUILDTYPE) -D PROJECT_NAME="$(APPNAME)" -D VERSION="$(VERSION)" `$(PKGCONFIG) --cflags dfgame-common`
CLIBS=`$(PKGCONFIG) --libs dfgame-common`
EDITORFLAGS=$(CFLAGS) -I$(SRCPATH)/$(LIBEDITORTARGET) `$(PKGCONFIG) --cflags dfgame-game-front dfgame-editor-front`
EDITORLIBS=$(CLIBS) `$(PKGCONFIG) --libs dfgame-game-front dfgame-editor-front`
LIBEDITORFLAGS=$(CFLAGS) `$(PKGCONFIG) --cflags dfgame-game dfgame-editor`
LIBEDITORLIBS=$(CLIBS) -lm `$(PKGCONFIG) --libs dfgame-game dfgame-editor`
GAMEFLAGS=$(CFLAGS) -I$(SRCPATH)/$(LIBGAMETARGET) `$(PKGCONFIG) --cflags dfgame-game-front`
GAMELIBS=$(CLIBS) `$(PKGCONFIG) --libs dfgame-game-front`
LIBGAMEFLAGS=$(CFLAGS) `$(PKGCONFIG) --cflags dfgame-game`
LIBGAMELIBS=$(CLIBS) -lm `$(PKGCONFIG) --libs dfgame-game`

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
EDITORTARGET=$(APPNAME)-editor
LIBEDITORTARGET=lib$(APPNAME)-editor

# Dependency lists
GAMESRCS:=$(wildcard $(SRCPATH)/$(GAMETARGET)/*.c)
GAMEOBJS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.o,$(GAMESRCS))
GAMEDEPS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.depend,$(GAMESRCS))

LIBGAMESRCS:=$(wildcard $(SRCPATH)/$(LIBGAMETARGET)/*.c)
LIBGAMEOBJS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.o,$(LIBGAMESRCS))
LIBGAMEDEPS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.depend,$(LIBGAMESRCS))

EDITORSRCS:=$(wildcard $(SRCPATH)/$(EDITORTARGET)/*.vala)
EDITOROBJS:=$(patsubst $(SRCPATH)/%.vala,$(OBJPATH)/%.vala.o,$(EDITORSRCS))

LIBEDITORSRCS:=$(wildcard $(SRCPATH)/$(LIBEDITORTARGET)/*.c)
LIBEDITOROBJS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.o,$(LIBEDITORSRCS))
LIBEDITORDEPS:=$(patsubst $(SRCPATH)/%.c,$(OBJPATH)/%.depend,$(LIBEDITORSRCS))

# Directory inits
$(shell mkdir -p $(OBJPATH))
$(shell mkdir -p $(LIBPATH))
$(shell mkdir -p $(VAPIPATH))
$(shell mkdir -p $(OBJPATH)/$(GAMETARGET))
$(shell mkdir -p $(OBJPATH)/$(LIBGAMETARGET))
$(shell mkdir -p $(OBJPATH)/$(EDITORTARGET))
$(shell mkdir -p $(OBJPATH)/$(LIBEDITORTARGET))

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

$(OBJPATH)/$(LIBEDITORTARGET)/%.depend: $(SRCPATH)/$(LIBEDITORTARGET)/%.c
	@echo -e "Building dependecies for $<..."
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $(LIBEDITORFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)/$(GAMETARGET)/%.o: $(SRCPATH)/$(GAMETARGET)/%.c
	@echo -e "Building $<..."
	$(CC) -c $< -o $@ $(GAMEFLAGS)

$(OBJPATH)/$(LIBGAMETARGET)/%.o: $(SRCPATH)/$(LIBGAMETARGET)/%.c
	@echo -e "Building $<..."
	$(CC) -c $< -o $@ $(LIBGAMEFLAGS)

$(OBJPATH)/$(LIBEDITORTARGET)/%.o: $(SRCPATH)/$(LIBEDITORTARGET)/%.c
	@echo -e "Building $<..."
	$(CC) -c $< -o $@ $(LIBEDITORFLAGS)

all: $(LIBGAMETARGET) $(GAMETARGET) $(LIBEDITORTARGET) $(EDITORTARGET)

$(GAMETARGET): $(GAMEOBJS) $(GAMEDEPS)
	$(CC) -o $(GAMETARGET) $(GAMEFLAGS) $(GAMEOBJS) $(LIBPATH)/$(LIBGAMETARGET).a $(GAMELIBS)

$(LIBGAMETARGET): $(LIBGAMEOBJS) $(LIBGAMEDEPS)
	ar -rcs $(LIBPATH)/$(LIBGAMETARGET).a $(LIBGAMEOBJS)

$(EDITORTARGET): $(EDITORSRCS)
	$(VALAC) $(VFLAGS) $(EDITORSRCS) $(VLIBS)
	mv *.o $(OBJPATH)/$(EDITORTARGET)
	$(CC) -o $(EDITORTARGET) $(EDITOROBJS) $(LIBPATH)/$(LIBEDITORTARGET).a $(LIBPATH)/$(LIBGAMETARGET).a $(CFLAGS) $(EDITORFLAGS) $(CLIBS) $(EDITORLIBS)

$(LIBEDITORTARGET): $(LIBEDITOROBJS) $(LIBEDITORDEPS)
	ar -rcs $(LIBPATH)/$(LIBEDITORTARGET).a $(LIBEDITOROBJS)

# Dynamic includes
-include $(GAMEDEPS)
-include $(LIBGAMEDEPS)
-include $(LIBEDITORDEPS)
