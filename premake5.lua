-- wxPdfDocument configuration file for premake5
--
-- Copyright (C) 2017-2021 Ulrich Telle <ulrich@telle-online.de>
--
-- This file is covered by the same licence as the entire wxpdfdoc package. 

dofile "premake/wxwidgets.lua"

-- premake.wxSetupProps = '../Build/wx_setup.props'

BUILDDIR = _OPTIONS["builddir"] or "build"

workspace "wxpdfdoc"
  configurations { "Debug", "Release", "Debug wxDLL", "Release wxDLL", "Debug DLL", "Release DLL" }
  platforms { "Win32", "Win64" }
  location(BUILDDIR)

  if (is_msvc) then
    local wks = workspace()
    wks.filename = "wxpdfdoc_" .. vc_with_ver
  end

  defines {
    "_WINDOWS",
    "_CRT_SECURE_NO_WARNINGS",
    "_CRT_SECURE_NO_DEPRECATE",
    "_CRT_NONSTDC_NO_DEPRECATE"
  }

  init_filters()

-- wxPdfDoc library
project "wxpdfdoc"
  location(BUILDDIR)
  language "C++"
  cppdialect "C++11"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_pdfdoc"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "wxpdfdoc_mono"
  end

  make_filters( "PDFDOC", "wxpdfdoc", "core,xml" )

  files { "src/*.cpp", "src/*.inc", "src/*.rc", "include/wx/*.h",
          "src/crypto/*.cpp", "src/crypto/*.h"  }
  vpaths {
    ["Header Files"] = { "**.h", "**.inc" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "include" }
  characterset "Unicode"


-- Minimal sample
project "minimal"
  location(BUILDDIR)
  language "C++"
  cppdialect "C++11"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_minimal"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "minimal_mono"
  end

  use_filters( "PDFDOC", "samples/minimal", "core,xml" )

  files { "samples/minimal/*.cpp", "samples/minimal/*.h", "samples/minimal/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "samples/minimal", "include" }
  characterset "Unicode"
  links { "wxpdfdoc" }
  
-- wxPdfDC sample
project "dcsample"
  location(BUILDDIR)
  language "C++"
  cppdialect "C++11"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_dcsample"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "dcsample_mono"
  end

  use_filters( "PDFDOC", "samples/pdfdc", "adv,html,richtext,core,xml" )

  files { "samples/pdfdc/*.cpp", "samples/pdfdc/*.h", "samples/pdfdc/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "samples/pdfdc", "include" }
  characterset "Unicode"
  entrypoint "WinMainCRTStartup"
  links { "wxpdfdoc" }

-- MakeFont utility
project "makefont"
  location(BUILDDIR)
  language "C++"
  cppdialect "C++11"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_makefont"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "makefont_mono"
  end

  use_filters( "PDFDOC", "makefont", "core,xml" )

  files { "makefont/*.cpp", "makefont/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "include" }
  characterset "Unicode"
  links { "wxpdfdoc" }

-- ShowFont utility
project "showfont"
  location(BUILDDIR)
  language "C++"
  cppdialect "C++11"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_showfont"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "showfont_mono"
  end

  use_filters( "PDFDOC", "showfont", "core,xml" )

  files { "showfont/*.cpp", "showfont/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "include" }
  characterset "Unicode"
  links { "wxpdfdoc" }
