

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra  -include include/pch.h  #-fPIC  -g0 -Os -s -Wl,--strip-all -fdata-sections -ffunction-sections 
ifeq ($(OS),Windows_NT)
	LIBS = -mwindows -lmingw32  -lSDL2main -lSDL2 -lm -lopengl32 -lgdi32 -luser32 -lkernel32 
	CXXFLAGS += -Dmain=SDL_main -DWIN32 -D__WIN32__
else
	LIBS = -lSDL2 -ldl -lGL
	CXXFLAGS +=-g  -fsanitize=undefined -fno-omit-frame-pointer
endif

CXXFLAGS += -Iinclude -Isrc -Isrc/core -Isrc/utils -Isrc/main -Isrc/gui -Isrc/gl -Isrc/input -Isrc/render -Isrc/scene 
SRCDIR = src
OBJDIR = obj

LIBDIR = lib

SUBDIRS = core utils main gui gl input render scene


SRCDIRS = $(addprefix $(SRCDIR)/,$(SUBDIRS))
OBJDIRS = $(addprefix $(OBJDIR)/,$(SUBDIRS))



SRCS_ALL = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))
OBJS_ALL = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS_ALL))

# excluindo a pasta main
SRCS_WITHOUT_MAIN = $(foreach dir,$(filter-out $(SRCDIR)/main,$(SRCDIRS)),$(wildcard $(dir)/*.cpp))
OBJS_WITHOUT_MAIN = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS_WITHOUT_MAIN))


TARGET = main
STATICLIB = libcore.a
SHAREDLIB = libcore.so

ifeq ($(OS),Windows_NT)
	TARGET = core.exe
	STATICLIB = libcore.a
	SHAREDLIB = libcore.dll

endif



all: $(TARGET) 

$(TARGET): $(OBJS_ALL)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	./$(TARGET)
	#rm -f $(TARGET)
	#rm -f ob/main/main.o



$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(OBJDIRS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $@
	@mkdir -p $(LIBDIR)

$(OBJDIRS):
	@mkdir -p $@	

static : CXXFLAGS +=-std=c++11 -DSTATIC -w  -Wl,--strip-all -fdata-sections -ffunction-sections
static: $(STATICLIB)

$(STATICLIB): $(OBJS_WITHOUT_MAIN)
	ar rcs $(LIBDIR)/$@ $^

shared: CXXFLAGS += -w -DSHARED -fPIC  -Wl,--strip-all -fdata-sections -ffunction-sections -DBUILDING_CORE_DLL
shared: $(SHAREDLIB)

$(SHAREDLIB): $(OBJS_WITHOUT_MAIN)
	$(CXX) -shared -o  $(LIBDIR)/$@ $^ $(LIBS)
	strip --strip-unneeded $(LIBDIR)/$@
	cp $(LIBDIR)/$@ ~/code/lib/$@

bin: $(TARGET)

clean:
	rm -rf $(OBJDIR)  $(LIBDIR) $(TARGET)

re: clean all

.PHONY: all clean re static shared bin