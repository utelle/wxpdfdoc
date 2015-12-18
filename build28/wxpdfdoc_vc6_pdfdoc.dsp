# Microsoft Developer Studio Project File - Name="wxpdfdoc_vc6_pdfdoc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pdfdoc - Win32 Static ANSI Release Multilib Static
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxpdfdoc_vc6_pdfdoc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxpdfdoc_vc6_pdfdoc.mak" CFG="pdfdoc - Win32 Static ANSI Release Multilib Static"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pdfdoc - Win32 DLL Unicode Debug Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL Unicode Debug Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL Unicode Debug Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL Unicode Debug Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL Unicode Release Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL Unicode Release Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL Unicode Release Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL Unicode Release Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Debug Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Debug Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Debug Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Debug Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Release Monolithic DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Release Monolithic Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Release Multilib DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 DLL ANSI Release Multilib Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Debug Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Debug Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Debug Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Debug Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Release Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Release Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Release Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static Unicode Release Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Debug Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Debug Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Debug Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Debug Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Release Monolithic DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Release Monolithic Static" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Release Multilib DLL" (based on "Win32 (x86) Static Library")
!MESSAGE "pdfdoc - Win32 Static ANSI Release Multilib Static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"
# ADD LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"
# ADD LINK32 wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"
# ADD LINK32 wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"
# ADD LINK32 wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"
# ADD LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"
# ADD LINK32 wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"
# ADD LINK32 wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"
# ADD LINK32 wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"
# ADD LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"
# ADD LINK32 wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"
# ADD LINK32 wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_DEBUG" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"
# ADD LINK32 wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib" /debug /pdb:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"
# ADD LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"
# ADD LINK32 wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"
# ADD LINK32 wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 DLL ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "WXUSINGDLL" /D "__WXMSW__" /D "WXMAKINGDLL_PDFDOC" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d WXMAKINGDLL_PDFDOC
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"
# ADD LINK32 wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /dll /machine:i386 /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.dll" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /implib:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib" /pdb:"..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswud_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswud\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswud\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28ud_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswu_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswu\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswu\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28u_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "_UNICODE" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmswd_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_dll\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmswd\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmswd\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /Zi /Fd..\lib\vc_lib\wxcode_msw28d_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXDEBUG__" /D "__WXMSW__" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_dll"
# PROP Intermediate_Dir "vcmsw_dll\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_dll\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_dll\wxcode_msw28_pdfdoc.lib"

!ELSEIF  "$(CFG)" == "pdfdoc - Win32 Static ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vcmsw\pdfdoc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\vc_lib"
# PROP Intermediate_Dir "vcmsw\pdfdoc"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /Fd..\lib\vc_lib\wxcode_msw28_pdfdoc.pdb /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_pdfdoc.lib"
# ADD LIB32 /nologo /out:"..\lib\vc_lib\wxcode_msw28_pdfdoc.lib"

!ENDIF

# Begin Target

# Name "pdfdoc - Win32 DLL Unicode Debug Monolithic DLL"
# Name "pdfdoc - Win32 DLL Unicode Debug Monolithic Static"
# Name "pdfdoc - Win32 DLL Unicode Debug Multilib DLL"
# Name "pdfdoc - Win32 DLL Unicode Debug Multilib Static"
# Name "pdfdoc - Win32 DLL Unicode Release Monolithic DLL"
# Name "pdfdoc - Win32 DLL Unicode Release Monolithic Static"
# Name "pdfdoc - Win32 DLL Unicode Release Multilib DLL"
# Name "pdfdoc - Win32 DLL Unicode Release Multilib Static"
# Name "pdfdoc - Win32 DLL ANSI Debug Monolithic DLL"
# Name "pdfdoc - Win32 DLL ANSI Debug Monolithic Static"
# Name "pdfdoc - Win32 DLL ANSI Debug Multilib DLL"
# Name "pdfdoc - Win32 DLL ANSI Debug Multilib Static"
# Name "pdfdoc - Win32 DLL ANSI Release Monolithic DLL"
# Name "pdfdoc - Win32 DLL ANSI Release Monolithic Static"
# Name "pdfdoc - Win32 DLL ANSI Release Multilib DLL"
# Name "pdfdoc - Win32 DLL ANSI Release Multilib Static"
# Name "pdfdoc - Win32 Static Unicode Debug Monolithic DLL"
# Name "pdfdoc - Win32 Static Unicode Debug Monolithic Static"
# Name "pdfdoc - Win32 Static Unicode Debug Multilib DLL"
# Name "pdfdoc - Win32 Static Unicode Debug Multilib Static"
# Name "pdfdoc - Win32 Static Unicode Release Monolithic DLL"
# Name "pdfdoc - Win32 Static Unicode Release Monolithic Static"
# Name "pdfdoc - Win32 Static Unicode Release Multilib DLL"
# Name "pdfdoc - Win32 Static Unicode Release Multilib Static"
# Name "pdfdoc - Win32 Static ANSI Debug Monolithic DLL"
# Name "pdfdoc - Win32 Static ANSI Debug Monolithic Static"
# Name "pdfdoc - Win32 Static ANSI Debug Multilib DLL"
# Name "pdfdoc - Win32 Static ANSI Debug Multilib Static"
# Name "pdfdoc - Win32 Static ANSI Release Monolithic DLL"
# Name "pdfdoc - Win32 Static ANSI Release Monolithic Static"
# Name "pdfdoc - Win32 Static ANSI Release Multilib DLL"
# Name "pdfdoc - Win32 Static ANSI Release Multilib Static"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdfannotation.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfbarcode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfcffdecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfcffindex.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfcolour.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfdc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfdecode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfdocument.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfencoding.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfencrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffont.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdata.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdatacore.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdataopentype.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdatatruetype.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdatatype0.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdatatype1.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdescription.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontdetails.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontextended.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontparser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontparsertruetype.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontparsertype1.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontsubsetcff.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontsubsettruetype.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdffontvolt.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfform.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfgradient.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfgraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfimage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfkernel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdflayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfobjects.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfocg.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfparser.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfpattern.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfprint.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfrijndael.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdftemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfutility.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pdfxml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\wx\pdfannotation.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfarraydouble.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfarraytypes.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfbarcode.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfbookmark.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfcffdecoder.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfcffindex.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfcjkfontdata.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfcolour.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfcoonspatchmesh.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfcorefontdata.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfdc.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfdc28.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfdc29.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfdoc.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfdocdef.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfdocument.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfencoding.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfencrypt.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffont.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdata.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdatacore.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdataopentype.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdatatruetype.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdatatype0.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdatatype1.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdescription.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontdetails.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontextended.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontmacosx.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontmanager.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontparser.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontparsertruetype.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontparsertype1.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontsubsetcff.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontsubsettruetype.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdffontvolt.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfform.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfgradient.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfgraphics.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfimage.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfinfo.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdflayer.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdflinestyle.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdflinks.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfobjects.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfparser.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfpattern.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfprint.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfproperties.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfrijndael.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfshape.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfspotcolour.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdftemplate.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfutility.h
# End Source File
# Begin Source File

SOURCE=..\include\wx\pdfxml.h
# End Source File
# End Group
# End Target
# End Project

