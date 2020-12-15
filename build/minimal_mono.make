# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_win32
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
include config.gcc

define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_win32)
TARGETDIR = bin/gcc/Win32/Debug
TARGET = $(TARGETDIR)/minimald.exe
OBJDIR = obj/gcc/Win32/Debug/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DDEBUG -D_DEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -D__WXDEBUG__ -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -g
LIBS += ../lib/gcc_lib/libwxpdfdoc$(wxFlavour)d.a -l$(wxMonolithicLibName) -lwxjpeg$(wxSuffixDebug) -lwxpng$(wxSuffixDebug) -lwxzlib$(wxSuffixDebug) -lwxtiff$(wxSuffixDebug) -lwxexpat$(wxSuffixDebug) -lwxregex$(wxSuffix) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_lib/libwxpdfdoc$(wxFlavour)d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib" -L/usr/lib32 -m32

else ifeq ($(config),debug_win64)
TARGETDIR = bin/gcc/Win64/Debug
TARGET = $(TARGETDIR)/minimald.exe
OBJDIR = obj/gcc/Win64/Debug/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DDEBUG -D_DEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -D__WXDEBUG__ -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g
LIBS += ../lib/gcc_x64_lib/libwxpdfdoc$(wxFlavour)d.a -l$(wxMonolithicLibName) -lwxjpeg$(wxSuffixDebug) -lwxpng$(wxSuffixDebug) -lwxzlib$(wxSuffixDebug) -lwxtiff$(wxSuffixDebug) -lwxexpat$(wxSuffixDebug) -lwxregex$(wxSuffix) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_x64_lib/libwxpdfdoc$(wxFlavour)d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib" -L/usr/lib64 -m64

else ifeq ($(config),release_win32)
TARGETDIR = bin/gcc/Win32/Release
TARGET = $(TARGETDIR)/minimal.exe
OBJDIR = obj/gcc/Win32/Release/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DNDEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2
LIBS += ../lib/gcc_lib/libwxpdfdoc$(wxFlavour).a -l$(wxMonolithicLibName) -lwxjpeg$(wxSuffixDebug) -lwxpng$(wxSuffixDebug) -lwxzlib$(wxSuffixDebug) -lwxtiff$(wxSuffixDebug) -lwxexpat$(wxSuffixDebug) -lwxregex$(wxSuffix) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_lib/libwxpdfdoc$(wxFlavour).a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib" -L/usr/lib32 -m32 -s

else ifeq ($(config),release_win64)
TARGETDIR = bin/gcc/Win64/Release
TARGET = $(TARGETDIR)/minimal.exe
OBJDIR = obj/gcc/Win64/Release/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DNDEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2
LIBS += ../lib/gcc_x64_lib/libwxpdfdoc$(wxFlavour).a -l$(wxMonolithicLibName) -lwxjpeg$(wxSuffixDebug) -lwxpng$(wxSuffixDebug) -lwxzlib$(wxSuffixDebug) -lwxtiff$(wxSuffixDebug) -lwxexpat$(wxSuffixDebug) -lwxregex$(wxSuffix) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_x64_lib/libwxpdfdoc$(wxFlavour).a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_lib" -L/usr/lib64 -m64 -s

else ifeq ($(config),debug_wxdll_win32)
TARGETDIR = bin/gcc/Win32/Debug_wxDLL
TARGET = $(TARGETDIR)/minimald.exe
OBJDIR = obj/gcc/Win32/Debug_wxDLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DDEBUG -D_DEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -D__WXDEBUG__ -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -g
LIBS += ../lib/gcc_lib_wxdll/libwxpdfdoc$(wxFlavour)d.a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_lib_wxdll/libwxpdfdoc$(wxFlavour)d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib32 -m32

else ifeq ($(config),debug_wxdll_win64)
TARGETDIR = bin/gcc/Win64/Debug_wxDLL
TARGET = $(TARGETDIR)/minimald.exe
OBJDIR = obj/gcc/Win64/Debug_wxDLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DDEBUG -D_DEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -D__WXDEBUG__ -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g
LIBS += ../lib/gcc_x64_lib_wxdll/libwxpdfdoc$(wxFlavour)d.a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_x64_lib_wxdll/libwxpdfdoc$(wxFlavour)d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib64 -m64

else ifeq ($(config),release_wxdll_win32)
TARGETDIR = bin/gcc/Win32/Release_wxDLL
TARGET = $(TARGETDIR)/minimal.exe
OBJDIR = obj/gcc/Win32/Release_wxDLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DNDEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2
LIBS += ../lib/gcc_lib_wxdll/libwxpdfdoc$(wxFlavour).a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_lib_wxdll/libwxpdfdoc$(wxFlavour).a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib32 -m32 -s

else ifeq ($(config),release_wxdll_win64)
TARGETDIR = bin/gcc/Win64/Release_wxDLL
TARGET = $(TARGETDIR)/minimal.exe
OBJDIR = obj/gcc/Win64/Release_wxDLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DNDEBUG -DWXUSINGLIB_PDFDOC -D_UNICODE -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2
LIBS += ../lib/gcc_x64_lib_wxdll/libwxpdfdoc$(wxFlavour).a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_x64_lib_wxdll/libwxpdfdoc$(wxFlavour).a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib64 -m64 -s

else ifeq ($(config),debug_dll_win32)
TARGETDIR = bin/gcc/Win32/Debug_DLL
TARGET = $(TARGETDIR)/minimald.exe
OBJDIR = obj/gcc/Win32/Debug_DLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DDEBUG -D_DEBUG -DWXUSINGDLL_PDFDOC -D_UNICODE -D__WXDEBUG__ -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -g
LIBS += ../lib/gcc_dll/libwxpdfdoc$(wxFlavour)d.a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_dll/libwxpdfdoc$(wxFlavour)d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib32 -m32

else ifeq ($(config),debug_dll_win64)
TARGETDIR = bin/gcc/Win64/Debug_DLL
TARGET = $(TARGETDIR)/minimald.exe
OBJDIR = obj/gcc/Win64/Debug_DLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DDEBUG -D_DEBUG -DWXUSINGDLL_PDFDOC -D_UNICODE -D__WXDEBUG__ -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g
LIBS += ../lib/gcc_x64_dll/libwxpdfdoc$(wxFlavour)d.a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_x64_dll/libwxpdfdoc$(wxFlavour)d.a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib64 -m64

else ifeq ($(config),release_dll_win32)
TARGETDIR = bin/gcc/Win32/Release_DLL
TARGET = $(TARGETDIR)/minimal.exe
OBJDIR = obj/gcc/Win32/Release_DLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DNDEBUG -DWXUSINGDLL_PDFDOC -D_UNICODE -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -O2
LIBS += ../lib/gcc_dll/libwxpdfdoc$(wxFlavour).a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_dll/libwxpdfdoc$(wxFlavour).a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib32 -m32 -s

else ifeq ($(config),release_dll_win64)
TARGETDIR = bin/gcc/Win64/Release_DLL
TARGET = $(TARGETDIR)/minimal.exe
OBJDIR = obj/gcc/Win64/Release_DLL/minimal
DEFINES += -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -DNDEBUG -DWXUSINGDLL_PDFDOC -D_UNICODE -DWXUSINGDLL -D__WXMSW__
INCLUDES += -I"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll/msw$(wxSuffix)" -I"$(wxRootDir)/include" -I../samples/minimal -I../include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2
LIBS += ../lib/gcc_x64_dll/libwxpdfdoc$(wxFlavour).a -l$(wxMonolithicLibName) -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
LDDEPS += ../lib/gcc_x64_dll/libwxpdfdoc$(wxFlavour).a
ALL_LDFLAGS += $(LDFLAGS) -L"$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll" -L/usr/lib64 -m64 -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

CUSTOM :=
GENERATED :=
OBJECTS :=

CUSTOM += $(OBJDIR)/minimal.res
GENERATED += $(OBJDIR)/attachment.o
GENERATED += $(OBJDIR)/barcodes.o
GENERATED += $(OBJDIR)/bookmark.o
GENERATED += $(OBJDIR)/charting.o
GENERATED += $(OBJDIR)/cjktest.o
GENERATED += $(OBJDIR)/clipping.o
GENERATED += $(OBJDIR)/drawing.o
GENERATED += $(OBJDIR)/glyphwriting.o
GENERATED += $(OBJDIR)/gradients.o
GENERATED += $(OBJDIR)/indicfonts.o
GENERATED += $(OBJDIR)/jsform.o
GENERATED += $(OBJDIR)/kerning.o
GENERATED += $(OBJDIR)/labels.o
GENERATED += $(OBJDIR)/layers.o
GENERATED += $(OBJDIR)/minimal.o
GENERATED += $(OBJDIR)/minimal.res
GENERATED += $(OBJDIR)/protection.o
GENERATED += $(OBJDIR)/rotation.o
GENERATED += $(OBJDIR)/templates.o
GENERATED += $(OBJDIR)/transformation.o
GENERATED += $(OBJDIR)/transparency.o
GENERATED += $(OBJDIR)/tutorial1.o
GENERATED += $(OBJDIR)/tutorial2.o
GENERATED += $(OBJDIR)/tutorial3.o
GENERATED += $(OBJDIR)/tutorial4.o
GENERATED += $(OBJDIR)/tutorial5.o
GENERATED += $(OBJDIR)/tutorial6.o
GENERATED += $(OBJDIR)/tutorial7.o
GENERATED += $(OBJDIR)/wmf.o
GENERATED += $(OBJDIR)/xmlwrite.o
OBJECTS += $(OBJDIR)/attachment.o
OBJECTS += $(OBJDIR)/barcodes.o
OBJECTS += $(OBJDIR)/bookmark.o
OBJECTS += $(OBJDIR)/charting.o
OBJECTS += $(OBJDIR)/cjktest.o
OBJECTS += $(OBJDIR)/clipping.o
OBJECTS += $(OBJDIR)/drawing.o
OBJECTS += $(OBJDIR)/glyphwriting.o
OBJECTS += $(OBJDIR)/gradients.o
OBJECTS += $(OBJDIR)/indicfonts.o
OBJECTS += $(OBJDIR)/jsform.o
OBJECTS += $(OBJDIR)/kerning.o
OBJECTS += $(OBJDIR)/labels.o
OBJECTS += $(OBJDIR)/layers.o
OBJECTS += $(OBJDIR)/minimal.o
OBJECTS += $(OBJDIR)/protection.o
OBJECTS += $(OBJDIR)/rotation.o
OBJECTS += $(OBJDIR)/templates.o
OBJECTS += $(OBJDIR)/transformation.o
OBJECTS += $(OBJDIR)/transparency.o
OBJECTS += $(OBJDIR)/tutorial1.o
OBJECTS += $(OBJDIR)/tutorial2.o
OBJECTS += $(OBJDIR)/tutorial3.o
OBJECTS += $(OBJDIR)/tutorial4.o
OBJECTS += $(OBJDIR)/tutorial5.o
OBJECTS += $(OBJDIR)/tutorial6.o
OBJECTS += $(OBJDIR)/tutorial7.o
OBJECTS += $(OBJDIR)/wmf.o
OBJECTS += $(OBJDIR)/xmlwrite.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(CUSTOM) $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking minimal
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning minimal
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

$(CUSTOM): | prebuild
ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/attachment.o: ../samples/minimal/attachment.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/barcodes.o: ../samples/minimal/barcodes.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/bookmark.o: ../samples/minimal/bookmark.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/charting.o: ../samples/minimal/charting.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cjktest.o: ../samples/minimal/cjktest.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/clipping.o: ../samples/minimal/clipping.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/drawing.o: ../samples/minimal/drawing.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/glyphwriting.o: ../samples/minimal/glyphwriting.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gradients.o: ../samples/minimal/gradients.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/indicfonts.o: ../samples/minimal/indicfonts.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/jsform.o: ../samples/minimal/jsform.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/kerning.o: ../samples/minimal/kerning.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/labels.o: ../samples/minimal/labels.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/layers.o: ../samples/minimal/layers.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/minimal.o: ../samples/minimal/minimal.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/minimal.res: ../samples/minimal/minimal.rc
	@echo $(notdir $<)
	$(SILENT) $(RESCOMP) $< -O coff -o "$@" $(ALL_RESFLAGS)
$(OBJDIR)/protection.o: ../samples/minimal/protection.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rotation.o: ../samples/minimal/rotation.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/templates.o: ../samples/minimal/templates.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/transformation.o: ../samples/minimal/transformation.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/transparency.o: ../samples/minimal/transparency.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial1.o: ../samples/minimal/tutorial1.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial2.o: ../samples/minimal/tutorial2.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial3.o: ../samples/minimal/tutorial3.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial4.o: ../samples/minimal/tutorial4.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial5.o: ../samples/minimal/tutorial5.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial6.o: ../samples/minimal/tutorial6.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial7.o: ../samples/minimal/tutorial7.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/wmf.o: ../samples/minimal/wmf.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/xmlwrite.o: ../samples/minimal/xmlwrite.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif