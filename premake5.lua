dofile "premake/wxwidgets.lua"

BUILDDIR = _OPTIONS["builddir"] or "build"

workspace "wxspdfdoc"
  configurations { "Debug", "Release", "DLL Debug", "DLL Release" }
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

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_pdfdoc"
  end

  make_filters( "PDFDOC" )

  files { "src/*.cpp", "src/*.inc", "include/wx/*.h" }
  vpaths {
    ["Header Files"] = { "**.h", "**.inc" },
    ["Source Files"] = { "**.cpp" }
  }
  includedirs { "include" }
  flags { "Unicode" }  


-- Minimal sample
project "minimal"
  location(BUILDDIR)
  language "C++"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_minimal"
  end

  use_filters( "PDFDOC" )

  files { "samples/minimal/*.cpp", "samples/minimal/*.h", "samples/minimal/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "samples/minimal", "include" }
  flags { "Unicode", "WinMain" }  
  links { "wxpdfdoc" }
  targetdir "samples/minimal"

-- wxPdfDC sample
project "dcsample"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_dcsample"
  end

  use_filters( "PDFDOC" )

  files { "samples/pdfdc/*.cpp", "samples/pdfdc/*.h", "samples/pdfdc/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "samples/pdfdc", "include" }
  flags { "Unicode", "WinMain" }  
  links { "wxpdfdoc" }
  targetdir "samples/pdfdc"

-- MakeFont utility
project "makefont"
  location(BUILDDIR)
  language "C++"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_makefont"
  end

  use_filters( "PDFDOC" )

  files { "makefont/*.cpp", "makefont/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "include" }
  flags { "Unicode", "WinMain" }  
  links { "wxpdfdoc" }
  targetdir "makefont"

-- ShowFont utility
project "showfont"
  location(BUILDDIR)
  language "C++"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxpdfdoc_" .. vc_with_ver .. "_showfont"
  end

  use_filters( "PDFDOC" )

  files { "showfont/*.cpp", "showfont/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "include" }
  flags { "Unicode", "WinMain" }  
  links { "wxpdfdoc" }
  targetdir "showfont"
