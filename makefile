# Compilers
CC=gcc
VALAC=valac
PKGCONFIG=pkg-config
OS=Linux

# Filepaths
SRCPATH=src/
OBJPATH=obj/
LIBPATH=lib/
VAPIPATH=vapi/

# Targets
PREFIX=dflib
COMMONTARGET=common/
EDITORBACKTARGET=editor/
EDITORFRONTTARGET=editor-frontend/
GAMEBACKTARGET=game/
GAMEFRONTTARGET=game-frontend/

# Source Files
COMMONSRC=$(wildcard ${SRCPATH}${COMMONTARGET}*.c)
EDITORBACKSRC=$(wildcard ${SRCPATH}${EDITORBACKTARGET}*.c)
EDITORFRONTSRC=$(wildcard ${SRCPATH}${EDITORFRONTTARGET}*.vala)
GAMEBACKSRC=$(wildcard ${SRCPATH}${GAMEBACKTARGET}*.c)
GAMEFRONTSRC=$(wildcard ${SRCPATH}${GAMEFRONTTARGET}*.c)

COMMONOBJ=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.o,$(COMMONSRC))
EDITORBACKOBJ=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.o,$(EDITORBACKSRC))
EDITORFRONTOBJ=$(patsubst $(SRCPATH)%.vala,$(OBJPATH)%.vala.o,$(EDITORFRONTSRC))
GAMEBACKOBJ=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.o,$(GAMEBACKSRC))
GAMEFRONTOBJ=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.o,$(GAMEFRONTSRC))

COMMONDEPS=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.depend,$(COMMONSRC))
EDITORBACKDEPS=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.depend,$(EDITORBACKSRC))
GAMEBACKDEPS=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.depend,$(GAMEBACKSRC))
GAMEFRONTDEPS=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.depend,$(GAMEFRONTSRC))

# Flags
VFLAGS=-c --vapidir=$(VAPIPATH) -X -I$(SRCPATH)$(EDITORFRONTTARGET) -X -I$(SRCPATH)$(GAMEBACKTARGET) -X -I$(SRCPATH)$(COMMONTARGET) --cc=$(CC) -H $(SRCPATH)dfgame-editor-front.h --vapi=$(VAPIPATH)dfgame-editor-frontend.vapi
VLIBS=--pkg=glib-2.0 --pkg=gtk+-3.0 --pkg=gee-0.8 --pkg=libxml-2.0 --pkg=dflib --pkg=dfgame-editor --pkg=dfgame-game --pkg=dfgame-common

CFLAGS=-g -Wall -Werror -Wno-unused-variable -Wno-traditional -Wno-pedantic -I$(SRCPATH)$(COMMONTARGET) `$(PKGCONFIG) --cflags libxml-2.0 glew gl openal`
CLIBS=`$(PKGCONFIG) --libs glew gl lua openal`

GAMEBACKFLAGS=${CFLAGS}
GAMEBACKLIBS=${CFLAGS}
GAMEFRONTFLAGS=${CFLAGS} -I$(SRCPATH)$(GAMEBACKTARGET) ${GAMEBACKLIBS}
GAMEFRONTLIBS=${CFLAGS} ${GAMEBACKLIBS}
EDITORBACKFLAGS=${CFLAGS} ${GAMEBACKFLAGS}
EDITORBACKLIBS=${CFLAGS} ${GAMEBACKLIBS}
EDITORFRONTFLAGS=${CFLAGS} ${EDITORBACKFLAGS} -I${SRCPATH}${EDITORBACKTARGET} -I${SRCPATH}${GAMEBACKTARGET} -L${LIBPATH} `${PKGCONFIG} --cflags glib-2.0 gtk+-3.0 gee-0.8`
EDITORFRONTLIBS=`${PKGCONFIG} --libs glib-2.0 gtk+-3.0 gee-0.8`

# Directory inits
$(shell mkdir -p $(OBJPATH))
$(shell mkdir -p $(LIBPATH))
$(shell mkdir -p $(VAPIPATH))
$(shell mkdir -p $(OBJPATH)$(COMMONTARGET))
$(shell mkdir -p $(OBJPATH)$(EDITORBACKTARGET))
$(shell mkdir -p $(OBJPATH)$(EDITORFRONTTARGET))
$(shell mkdir -p $(OBJPATH)$(GAMEBACKTARGET))
$(shell mkdir -p $(OBJPATH)$(GAMEFRONTTARGET))

ifeq ($(OS), Linux)
else ifeq ($(OS), Windows)
CC=i686-w64-mingw32-gcc
PKGCONFIG=i686-w64-mingw32-pkg-config
else
$(error Unknown OS selected for output)
endif

# Filetype Rules
$(OBJPATH)$(COMMONTARGET)%.depend: $(SRCPATH)$(COMMONTARGET)%.c
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/$(COMMONTARGET)\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)$(EDITORBACKTARGET)%.depend: $(SRCPATH)$(EDITORBACKTARGET)%.c
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CC) -M $(EDITORBACKFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/$(EDITORBACKTARGET)\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)$(GAMEBACKTARGET)%.depend: $(SRCPATH)$(GAMEBACKTARGET)%.c
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CC) -M $(GAMEBACKFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/$(GAMEBACKTARGET)\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)$(GAMEFRONTTARGET)%.depend: $(SRCPATH)$(GAMEFRONTTARGET)%.c
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CC) -M $(GAMEFRONTFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/$(GAMEFRONTTARGET)\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)$(COMMONTARGET)%.o: $(SRCPATH)$(COMMONTARGET)%.c
	@echo -e "Building \e[1;35m$<\e[0m..."
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJPATH)$(EDITORBACKTARGET)%.o: $(SRCPATH)$(EDITORBACKTARGET)%.c
	@echo -e "Building \e[1;35m$<\e[0m..."
	$(CC) -c $< -o $@ $(EDITORBACKFLAGS)

$(OBJPATH)$(GAMEBACKTARGET)%.o: $(SRCPATH)$(GAMEBACKTARGET)%.c
	@echo -e "Building \e[1;35m$<\e[0m..."
	$(CC) -c $< -o $@ $(GAMEBACKFLAGS)

$(OBJPATH)$(GAMEFRONTTARGET)%.o: $(SRCPATH)$(GAMEFRONTTARGET)%.c
	@echo -e "Building \e[1;35m$<\e[0m..."
	$(CC) -c $< -o $@ $(GAMEFRONTFLAGS)

# Target Rules
all: $(COMMONTARGET) $(GAMEBACKTARGET) $(GAMEFRONTTARGET) $(EDITORBACKTARGET) $(EDITORFRONTTARGET) tags

$(COMMONTARGET): $(COMMONOBJ)
	ar -rs $(LIBPATH)libdfgame-common.a $(COMMONOBJ)
$(GAMEBACKTARGET): $(COMMONTARGET) $(GAMEBACKOBJ)
	ar -rs $(LIBPATH)libdfgame-game.a $(GAMEBACKOBJ)
$(GAMEFRONTTARGET): $(COMMONTARGET) $(GAMEBACKTARGET) $(GAMEFRONTOBJ)
	ar -rs $(LIBPATH)libdfgame-game-front.a $(GAMEFRONTOBJ)
$(EDITORBACKTARGET): $(COMMONTARGET) $(EDITORBACKOBJ)
	ar -rs $(LIBPATH)libdfgame-editor.a $(EDITORBACKOBJ)
$(EDITORFRONTTARGET): $(COMMONTARGET) $(EDITORBACKTARGET) $(GAMEBACKTARGET) $(EDITORFRONTSRC)
	$(VALAC) $(VFLAGS) $(EDITORFRONTSRC) $(VLIBS)
	mv *.o $(OBJPATH)/$(EDITORFRONTTARGET)
	ar -rs $(LIBPATH)libdfgame-editor-front.a $(EDITORFRONTOBJ)

-include $(COMMONDEPS)
-include $(EDITORBACKDEPS)
-include $(GAMEBACKDEPS)
-include $(GAMEFRONTDEPS)

tags: $(COMMONSRC) $(LIBEDITORSRC) $(LIBGAMESRC) $(EDITORSRC) $(GAMESRC)
	ctags -R $(SRCPATH)

# TODO: Unit tests
test: all

# Clean
.PHONY: clean
clean:
	@echo -e "\e[0;31mCleaning up...\e[0m"
	$(RM) -r $(OBJPATH)
	$(RM) -r $(LIBPATH)*.a
	@echo -e "\e[0;32mCleanup completed.\e[0m"

# TODO: Add -f flags
install:
	mkdir -p /usr/lib/dfgame
	cp -t /usr/lib/dfgame lib/*.a
	mkdir -p /usr/include/dfgame
	cp -t /usr/include/dfgame src/*.h
	mkdir -p /usr/include/dfgame/common
	cp -t /usr/include/dfgame/common src/common/*.h
	mkdir -p /usr/include/dfgame/editor
	cp -t /usr/include/dfgame/editor src/editor/*.h
	mkdir -p /usr/include/dfgame/game
	cp -t /usr/include/dfgame/game src/game/*.h
	mkdir -p /usr/include/dfgame/game-frontend
	cp -t /usr/include/dfgame/game-frontend src/game-frontend/*.h
	cp -t /usr/share/vala/vapi/ vapi/*.vapi
	cp -t /usr/lib/pkgconfig/ lib/*.pc
	mkdir -p /usr/share/dfgame
	cp -rft /usr/share/dfgame data/*
	mv /usr/share/dfgame/dfgame.sh /usr/bin/dfgame
