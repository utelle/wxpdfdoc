-- Barcode library zint

project "libzint"
  kind "StaticLib"
  language "C++"
  cppdialect "C++11"

  if (is_msvc) then
    local prj = project()
    prj.filename = "libzint_" .. vc_with_ver
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
    "ZINT_NO_PNG",
    "_LIB"
  }

  targetname "libzint"
  targetsuffix ""

  -- Target directory
  if (is_msvc) then
    targetdir ("%{wks.location}" .. "/lib/" .. vc_with_ver .. "/%{cfg.platform}/%{cfg.buildcfg}")
  else
    targetdir ("%{wks.location}" .. "/lib/$(wxCompilerPrefix)" .. "/%{cfg.platform}/%{cfg.buildcfg}")
  end

  filter { "configurations:Debug*" }

  filter { "configurations:Release*" }

  filter {}

  files { "thirdparty/zint/backend/*.c",
          "thirdparty/zint/backend/*.h",
          "thirdparty/zint/backend/fonts/*.h"
        }
  includedirs { "thirdparty/zint/backend" }
  vpaths {
    ["Header Files"] = { "**.h", "**.inc" },
    ["Source Files"] = { "**.cpp", "**.c", "**.def", "**.rc" }
  }
  characterset "Unicode"

  location(BUILDDIR)
