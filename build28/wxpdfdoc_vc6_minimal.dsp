# Microsoft Developer Studio Project File - Name="wxpdfdoc_vc6_minimal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=minimal - Win32 Static ANSI Release Multilib Static
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxpdfdoc_vc6_minimal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxpdfdoc_vc6_minimal.mak" CFG="minimal - Win32 Static ANSI Release Multilib Static"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "minimal - Win32 DLL Unicode Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL Unicode Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL Unicode Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL Unicode Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL Unicode Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL Unicode Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL Unicode Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL Unicode Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 DLL ANSI Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static Unicode Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "minimal - Win32 Static ANSI Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "minimal - Win32 DLL Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 DLL ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswud\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswud\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswu\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswu\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmswd\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmswd\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\samples\minimal" /Zi /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\samples\minimal" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw_dll\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw_dll\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ELSEIF  "$(CFG)" == "minimal - Win32 Static ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\samples\minimal"
# PROP BASE Intermediate_Dir "vcmsw\minimal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\samples\minimal"
# PROP Intermediate_Dir "vcmsw\minimal"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\samples\minimal" /Fd..\samples\minimal\minimal.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\samples\minimal
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\samples\minimal\minimal.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\samples\minimal\minimal.pdb"

!ENDIF

# Begin Target

# Name "minimal - Win32 DLL Unicode Debug Monolithic DLL"
# Name "minimal - Win32 DLL Unicode Debug Monolithic Static"
# Name "minimal - Win32 DLL Unicode Debug Multilib DLL"
# Name "minimal - Win32 DLL Unicode Debug Multilib Static"
# Name "minimal - Win32 DLL Unicode Release Monolithic DLL"
# Name "minimal - Win32 DLL Unicode Release Monolithic Static"
# Name "minimal - Win32 DLL Unicode Release Multilib DLL"
# Name "minimal - Win32 DLL Unicode Release Multilib Static"
# Name "minimal - Win32 DLL ANSI Debug Monolithic DLL"
# Name "minimal - Win32 DLL ANSI Debug Monolithic Static"
# Name "minimal - Win32 DLL ANSI Debug Multilib DLL"
# Name "minimal - Win32 DLL ANSI Debug Multilib Static"
# Name "minimal - Win32 DLL ANSI Release Monolithic DLL"
# Name "minimal - Win32 DLL ANSI Release Monolithic Static"
# Name "minimal - Win32 DLL ANSI Release Multilib DLL"
# Name "minimal - Win32 DLL ANSI Release Multilib Static"
# Name "minimal - Win32 Static Unicode Debug Monolithic DLL"
# Name "minimal - Win32 Static Unicode Debug Monolithic Static"
# Name "minimal - Win32 Static Unicode Debug Multilib DLL"
# Name "minimal - Win32 Static Unicode Debug Multilib Static"
# Name "minimal - Win32 Static Unicode Release Monolithic DLL"
# Name "minimal - Win32 Static Unicode Release Monolithic Static"
# Name "minimal - Win32 Static Unicode Release Multilib DLL"
# Name "minimal - Win32 Static Unicode Release Multilib Static"
# Name "minimal - Win32 Static ANSI Debug Monolithic DLL"
# Name "minimal - Win32 Static ANSI Debug Monolithic Static"
# Name "minimal - Win32 Static ANSI Debug Multilib DLL"
# Name "minimal - Win32 Static ANSI Debug Multilib Static"
# Name "minimal - Win32 Static ANSI Release Monolithic DLL"
# Name "minimal - Win32 Static ANSI Release Monolithic Static"
# Name "minimal - Win32 Static ANSI Release Multilib DLL"
# Name "minimal - Win32 Static ANSI Release Multilib Static"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\samples\minimal\attachment.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\barcodes.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\bookmark.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\charting.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\cjktest.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\clipping.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\drawing.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\glyphwriting.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\gradients.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\indicfonts.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\jsform.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\kerning.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\labels.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\layers.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\minimal.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\minimal.rc
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\protection.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\rotation.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\templates.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\transformation.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\transparency.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\tutorial1.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\tutorial2.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\tutorial3.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\tutorial4.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\tutorial5.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\tutorial6.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\tutorial7.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\wmf.cpp
# End Source File
# Begin Source File

SOURCE=..\samples\minimal\xmlwrite.cpp
# End Source File
# End Group
# End Target
# End Project

