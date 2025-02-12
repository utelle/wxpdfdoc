-- Support for WOFF/WOFF2 font files

project "libwoff2"
  kind "StaticLib"
  language "C++"
  cppdialect "C++11"

  if (is_msvc) then
    local prj = project()
    prj.filename = "libwoff2_" .. vc_with_ver
    if (msvc_useProps) then
      wxUseProps(true)
    end
  end

  makesettings { "include config.gcc" }

  -- Intermediate directory
  if (is_msvc) then
    objdir ("%{wks.location}" .. "/obj/%{prj.name}/" .. vc_with_ver)
  else
    objdir ("%{wks.location}" .. "/obj/%{prj.name}/$(wxCompilerPrefix)")
  end

  defines {
    "_LIB",
    "WOFF2_DUMMY"
  }

  targetname "libwoff2"
  targetsuffix ""
  if (is_msvc) then
    targetdir ("%{wks.location}" .. "/lib/" .. vc_with_ver .. "/%{cfg.platform}/%{cfg.buildcfg}")
  else
    targetdir ("%{wks.location}" .. "/lib/$(wxCompilerPrefix)" .. "/%{cfg.platform}/%{cfg.buildcfg}")
  end

  filter { "configurations:Debug*" }

  filter { "configurations:Release*" }

  filter {}

  files { "thirdparty/woff2/src/woff2_dec.cc", "thirdparty/woff2/src/woff2_common.cc",
          "thirdparty/woff2/src/woff2_out.cc",
          "thirdparty/woff2/src/table_tags.cc", "thirdparty/woff2/src/variable_length.cc",
          "thirdparty/woff2/src/buffer.h", "thirdparty/woff2/src/port.h",
          "thirdparty/woff2/src/round.h", "thirdparty/woff2/src/store_bytes.h",
          "thirdparty/woff2/src/table_tags.h", "thirdparty/woff2/src/variable_length.h",
          "thirdparty/woff2/src/woff2_common.h",
          "thirdparty/woff2/brotli/dec/*.c", "thirdparty/woff2/brotli/dec/*.h",
          "thirdparty/woff2/brotli/common/*.c", "thirdparty/woff2/brotli/common/*.h"
        }
  includedirs { "thirdparty/woff2/include", "thirdparty/woff2/src", "thirdparty/woff2/brotli/include" }
  vpaths {
    ["Header Files"] = { "**.h", "**.inc" },
    ["Source Files"] = { "**.cc", "**.c" }
  }
  characterset "Unicode"

  location(BUILDDIR)
