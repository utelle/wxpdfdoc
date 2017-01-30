-- wxWidgets configuration file for premake5
--
-- Based on the script for premake4 created by
-- laurent.humbertclaude@gmail.com and v.krishnakumar@gmail.com 

-- Optional environment variable specifying the wxWidgets version
newoption {
  trigger     = "wx_ver",
  value       = "3.1",
  description = "Version of the wxWidgets build to be used"
}

-- Optional environment variable pointing to the root directory of wxWidgets installation
newoption {
  trigger     = "wx_env",
  value       = "WXWIN",
  description = "Environment variable for the root of the wxWidgets build to be used"
}

-- Optional root directory of wxWidgets installation
newoption  {
  trigger     = "wx_root",
  value       = "PATH",
  description = "Path to wxWidgets root directory, by default, environment variable WXWIN will be used or wx-config found in path on POSIX"
}

-- Target directory for the build files generated by premake5
newoption {
  trigger     = "builddir",
  value       = "build",
  description = "Directory for the generated build files"
}

if not _OPTIONS["wx_ver"] then
   _OPTIONS["wx_ver"] = "3.1"
end

if not _OPTIONS["wx_env"] then
   _OPTIONS["wx_env"] = "WXWIN"
end

-- Determine version of Visual Studio action
msvc_useProps = false
vc_version = "";
if _ACTION == "vs2003" then
  vc_version = 7
elseif _ACTION == "vs2005" then
  vc_version = 8
elseif _ACTION == "vs2008" then
  vc_version = 9
elseif _ACTION == "vs2010" then
  vc_version = 10
elseif _ACTION == "vs2012" then
  vc_version = 11
elseif _ACTION == "vs2013" then
  vc_version = 12
elseif _ACTION == "vs2015" then
  vc_version = 14
elseif _ACTION == "vs2017" then
  vc_version = 15
end

is_msvc = false
wx_compiler = "gcc"
if ( vc_version ~= "" ) then
  is_msvc = true
  wx_compiler = "vc"
  msvc_useProps = vc_version >= 10
  vc_with_ver = "vc"..vc_version
end

-- Activate loading of separate props file
if (msvc_useProps) then
  premake.wxProject = true
end


-- The wx_config the parameters are.
--   Root      : path to wx root folder. Can be left empty if WXWIN is defined
--               or if wx-config is accessible.
--   Debug     : "yes" use debug version of wxwidgets. Default to "no"
--   Version   : one of '2.4', '2.5', '2.6', '2.7', '2.8', '2.9'. Default to '3.0'
--   Static    : indicates how wx is to be linked. Values are
--               either "yes" for static linking or "no" for shared linking, Default to "no"
--   Unicode   : use "yes" for unicode or "no" for ansi version.
--               ansi version only available up to 2.8
--               Default to "yes"
--   Universal : use universal configuration. Default to "no"
--   Libs      : a list of wx libraries that you want to link with.
--               eg: "aui,media,html"
--               Default to "richtext,aui,xrc,qa,html,adv,core,xml,net"; base is implicit
--   WindowsCompiler : compiler used to compile windows libraries ( "vc" or "gcc" )
 
function wx_config(options)

  local wrongParam = false
  local allowedWxOptions = {"Root", "Debug", "Host", "Version", "Static", "Unicode", "Universal", "Libs", "Arch", "WindowsCompiler" }
  for option in pairs(options) do
    if not table.contains(allowedWxOptions, option) then
      print ("unrecognized option '"..option.. "'")
      wrongParam = true
    end
  end
  if wrongParam then print("valid options are : '" .. table.concat(allowedWxOptions, "', '").."'") end
 
  wx_config_Private( options.Root or "",
                     options.Debug or "",
                     options.Host or "",
                     options.Version or "3.1",
                     options.Static or "",
                     options.Unicode or "yes",
                     options.Universal or "",
                     options.Libs or "richtext,aui,xrc,qa,html,adv,core,xml,net", -- base is implicit, std not valid
                     options.Arch or "x32",
                     options.WindowsCompiler or "vc"
                   )
end
 
function wx_config_Private(wxRoot, wxDebug, wxHost, wxVersion, wxStatic, wxUnicode, wxUniversal, wxLibs, wxArch, wxWindowsCompiler)
    -- some options are not allowed for newer version of wxWidgets
    if wxVersion > "2.8" then -- alphabetical comparison may fail...
    --    wxDebug = "" -- 2.9 still make debug libraries
        wxUnicode = "yes"
    end
 
    --wx_root=PATH override wxRoot parameter
    if _OPTIONS and _OPTIONS["wx_root"] then
        print ("seen option '--wx_root=" .. _OPTIONS["wx_root"] .. "' overriding default root = '" .. wxRoot .. "'")
        wxRoot = _OPTIONS["wx_root"]
    end
    -- the environment variable WXWIN override both wxRoot parameter and --wx_root option
    if os.getenv(_OPTIONS["wx_env"]) then wxRoot = "$(".._OPTIONS["wx_env"]..")" end
 
    if wxUnicode == "yes" then defines { "_UNICODE" } end
 
    if wxDebug == "yes" then defines { "__WXDEBUG__" }
    elseif wxDebug == "no" then flags { "Optimize" } end
 
    if wxStatic == "yes" then
        -- flags { "StaticRuntime" }
    else
        defines { "WXUSINGDLL" }
    end
 
 
    -- function to compensate lack of wx-config program on windows
    -- but wait, look at http://sites.google.com/site/wxconfig/ for one !
    function wx_config_for_windows(wxWindowsCompiler)
        local wxBuildType = ""  -- buildtype is one of "", "u", "d" or "ud"
        local wxDebugSuffix = "" -- debug buildsuffix is for support libraries only
        if wxUnicode ~= "" then wxBuildType = wxBuildType .. "u" end
        if wxDebug == "yes" then
            wxBuildType = wxBuildType .. "d"
            wxDebugSuffix = "d"
        end
 
        if wxArch == "x64" then
          wxArchSuffix = "_x64"
        else
          wxArchSuffix = ""
        end
        if (msvc_useProps) then
          local wxLibPath = '$(wxRootDir)\\lib\\$(wxCompilerPrefix)$(wxArchSuffix)_' .. iif(wxStatic == 'yes', 'lib', 'dll')
          -- common defines
          defines{ "__WXMSW__" }
 
          -- common include path
          includedirs {
              path.join(wxLibPath, "msw$(wxSuffix)"),   -- something like "%WXWIN%\lib\vc_lib\mswud" to find "wx/setup.h"
              path.join("$(wxRootDir)", "include")
            }
 
          -- common library path
          libdirs { wxLibPath }
          if wxDebug == "yes" and wxStatic == 'no' then
            local compLibPath = '$(ProjectDir)..\\lib\\$(wxCompilerPrefix)$(wxArchSuffix)_dll'
            local fontLibPath = '$(ProjectDir)..\\lib\\fonts'
            debugenvs { "PATH=" .. compLibPath .. ";" .. wxLibPath, "WXPDF_FONTPATH=" .. fontLibPath }
          end
        elseif (wxWindowsCompiler == "gcc") then
          local wxLibPath = '$(wxRootDir)/lib/$(wxCompilerPrefix)$(wxArchSuffix)_' .. iif(wxStatic == 'yes', 'lib', 'dll')
          -- common defines
          defines{ "__WXMSW__" }
 
          -- common include path
          includedirs {
              path.join(wxLibPath, "msw$(wxSuffix)"),   -- something like "%WXWIN%\lib\vc_lib\mswud" to find "wx/setup.h"
              path.join("$(wxRootDir)", "include")
            }
 
          -- common library path
          libdirs { wxLibPath }
          if wxDebug == "yes" and wxStatic == 'no' then
            local compLibPath = '$(ProjectDir)../lib/$(wxCompilerPrefix)$(wxArchSuffix)_dll'
            local fontLibPath = '$(ProjectDir)../lib/fonts'
            debugenvs { "PATH=" .. compLibPath .. ";" .. wxLibPath, "WXPDF_FONTPATH=" .. fontLibPath }
          end
        else
          local wxLibPath = path.join(wxRoot, "lib")
          wxLibPath = path.join(wxLibPath, wxWindowsCompiler .. wxArchSuffix .. "_" .. iif(wxStatic == 'yes', 'lib', 'dll'))
          -- common defines
          defines{ "__WXMSW__" }
 
          -- common include path
          includedirs {
              path.join(wxLibPath, "msw" .. wxBuildType),   -- something like "%WXWIN%\lib\vc_lib\mswud" to find "wx/setup.h"
              path.join(wxRoot, "include")
            }
 
          -- common library path
          libdirs { wxLibPath }
          if wxDebug == "yes" and wxStatic == 'no' then
            local compLibPath = '$(ProjectDir)..\\lib\\' .. wxWindowsCompiler .. wxArchSuffix .. '_dll'
            local fontLibPath = '$(ProjectDir)..\\lib\\fonts'
            debugenvs { "PATH=" .. compLibPath .. ";" .. wxLibPath, "WXPDF_FONTPATH=" .. fontLibPath }
          end
        end
 
        -- add the libs
        if (msvc_useProps) then
          libVersion = string.gsub(wxVersion, '%.', '') -- remove dot from version
          links { "$(wxBaseLibNamePrefix)" } -- base lib
          for i, lib in ipairs(string.explode(wxLibs, ",")) do
              local libPrefix = '$(wxToolkitLibNamePrefix)'
              if lib == "xml" or lib == "net" or lib == "odbc" then
                  libPrefix = '$(wxBaseLibNamePrefix)_'
              end
              links { libPrefix..lib}
          end
          -- link with support libraries
          for i, lib in ipairs({"wxjpeg", "wxpng", "wxzlib", "wxtiff", "wxexpat"}) do
              links { lib.."$(wxSuffixDebug)" }
          end
          links { "wxregex" .. "$(wxSuffix)" }
          links { "comctl32", "rpcrt4", "shell32", "gdi32", "kernel32", "user32", "comdlg32", "ole32", "oleaut32", "advapi32" }
        elseif (wxWindowsCompiler == "gcc") then
          libVersion = string.gsub(wxVersion, '%.', '') -- remove dot from version
          links { "$(wxBaseLibNamePrefix)" } -- base lib
          for i, lib in ipairs(string.explode(wxLibs, ",")) do
              local libPrefix = '$(wxToolkitLibNamePrefix)'
              if lib == "xml" or lib == "net" or lib == "odbc" then
                  libPrefix = '$(wxBaseLibNamePrefix)_'
              end
              links { libPrefix..lib}
          end
          -- link with support libraries
          for i, lib in ipairs({"wxjpeg", "wxpng", "wxzlib", "wxtiff", "wxexpat"}) do
              links { lib.."$(wxSuffixDebug)" }
          end
          links { "wxregex" .. "$(wxSuffix)" }
          links { "comctl32", "rpcrt4", "shell32", "gdi32", "kernel32", "user32", "comdlg32", "ole32", "oleaut32", "advapi32" }
        else
          libVersion = string.gsub(wxVersion, '%.', '') -- remove dot from version
          links { "wxbase"..libVersion..wxBuildType } -- base lib
          for i, lib in ipairs(string.explode(wxLibs, ",")) do
              local libPrefix = 'wxmsw'
              if lib == "xml" or lib == "net" or lib == "odbc" then
                  libPrefix = 'wxbase'
              end
              links { libPrefix..libVersion..wxBuildType..'_'..lib}
          end
          -- link with support libraries
          for i, lib in ipairs({"wxjpeg", "wxpng", "wxzlib", "wxtiff", "wxexpat"}) do
              links { lib..wxDebugSuffix }
          end
          links { "wxregex" .. wxBuildType }
          links { "comctl32", "rpcrt4", "shell32", "gdi32", "kernel32", "user32", "comdlg32", "ole32", "oleaut32", "advapi32" }
        end
    end
 
    -- use wx-config to figure out build parameters
    function wx_config_for_posix()
        local configCmd = "wx-config"  -- this is the wx-config command ligne
        if wxRoot ~= "" then configCmd = path.join(wxRoot, "bin/wx-config") end
 
        local function checkYesNo(value, option)
            if value == "" then return "" end
            if value == "yes" or value == "no" then return " --"..option.."="..value end
            error("wx"..option..' can only be "yes", "no" or empty' )
        end
 
        configCmd = configCmd .. checkYesNo(wxDebug, "debug")
        configCmd = configCmd .. checkYesNo(wxStatic, "static")
        configCmd = configCmd .. checkYesNo(wxUnicode, "unicode")
        configCmd = configCmd .. checkYesNo(wxUniversal, "universal")
        if wxHost ~= "" then configCmd = configCmd .. " --host=" .. wxHost end
        if wxVersion ~= "" then configCmd = configCmd .. " --version=" .. wxVersion end
 
        -- set the parameters to the curent configuration
        buildoptions{"`" .. configCmd .." --cxxflags`"}
        linkoptions{"`" .. configCmd .." --libs " .. wxLibs .. "`"}
    end
 
-- BUG: here, using any configuration() function will reset the current filter
--      and apply configuration to all project configuration...
--      see http://industriousone.com/post/add-way-refine-configuration-filter
--      and http://sourceforge.net/tracker/?func=detail&aid=2936443&group_id=71616&atid=531881
--~     configuration "not windows"
--~         wx_config_for_posix()
--~     configuration "vs*"
--~         wx_config_for_windows("vc")
--~     configuration {"windows", "codeblocks or gmake or codelitle"}
--~         wx_config_for_windows("gcc")
    if os.get() ~= "windows" then
        wx_config_for_posix()
    else
        local allowedCompiler = {"vc", "gcc"}
        if not table.contains( allowedCompiler, wxWindowsCompiler) then
            print( "wrong wxWidgets Compiler specified('"..wxWindowsCompiler.."'), should be one of '".. table.concat(allowedCompiler, "', '").."'" )
            wxWindowsCompiler = "vc"
        end
--~  BUG/WISH: I need a function like compiler.get() that return the project/configuration compiler
--~         local wxWindowsCompiler = "vc"
--~  BUG? --cc=compiler standard premake option is not registered in the _OPTIONS array
--~         if _OPTIONS and _OPTIONS["cc"] then
--~             wxWindowsCompiler = _OPTIONS.cc
--~             print("seen option '--cc=" .. _OPTIONS["cc"] .. "' overriding default cc='vc'")
--~         end
        wx_config_for_windows(wxWindowsCompiler)
    end
end

function init_filters()
  filter { "platforms:x32" }
    system "Windows"
    architecture "x32"

  filter { "platforms:x64" }
    system "Windows"
    architecture "x64"

  filter { "configurations:*Debug*" }
    defines {
      "DEBUG", 
      "_DEBUG"
    }
    symbols "On"
    targetsuffix "d"


  filter { "configurations:*Release*" }
    defines {
      "NDEBUG"
    }
    flags { "Optimize" }  

  filter {}
end

function make_filters(libname,libtarget,wxlibs)
  if (msvc_useProps) then
    targetname(libtarget .. "$(wxFlavour)")
  else
    targetname(libtarget)
  end
  makesettings { "include config.gcc" }
  filter { "configurations:not DLL*" }
    kind "StaticLib"
    defines {
      "_LIB",
      "WXMAKINGLIB_" .. libname
    }
  filter { "configurations:not DLL*", "platforms:x32" }
    if (is_msvc) then
      if (msvc_useProps) then
        targetdir("$(wxOutDir)")
      else
        targetdir("lib/vc_lib")
      end
    else
      targetdir("lib/gcc_lib")
    end
  filter { "configurations:not DLL*", "platforms:x64" }
    if (is_msvc) then
      if (msvc_useProps) then
        targetdir("$(wxOutDir)")
      else
        targetdir("lib/vc_x64_lib")
      end
    else
      targetdir("lib/gcc_x64_lib")
    end

  filter { "configurations:DLL*" }
    kind "SharedLib"
    defines {
      "_USRDLL",
      "WXMAKINGDLL_" .. libname
    }

  filter { "configurations:DLL*", "platforms:x32" }
    if (is_msvc) then
      if (msvc_useProps) then
        targetdir("$(wxOutDir)")
      else
        targetdir("lib/vc_dll")
      end
    else
      targetdir("lib/gcc_dll")
    end
  filter { "configurations:DLL*", "platforms:x64" }
    if (is_msvc) then
      if (msvc_useProps) then
        targetdir("$(wxOutDir)")
      else
        targetdir("lib/vc_x64_dll")
      end
    else
      targetdir("lib/gcc_x64_dll")
    end
    
  filter { "configurations:*Debug*" }
    targetsuffix "d"

  filter { "configurations:*Release*" }
    targetsuffix ""

  filter { "configurations:Debug", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="yes", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Debug", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="yes", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:Debug wxDLL", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Debug wxDLL", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:DLL Debug", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:DLL Debug", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:Release", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="no", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Release", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="no", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:Release wxDLL", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Release wxDLL", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:DLL Release", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:DLL Release", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter {}
end

function use_filters(libname, debugcwd, wxlibs)
  makesettings { "include config.gcc" }

  filter { "configurations:not DLL*" }
    defines {
      "WXUSINGLIB_" .. libname
    }

  filter { "configurations:DLL*" }
    defines {
      "WXUSINGDLL_" .. libname
    }
    
  filter { "configurations:*Debug*" }
    targetsuffix "d"
    debugdir(debugcwd)

  filter { "configurations:*Release*" }
    targetsuffix ""

  filter { "configurations:Debug", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="yes", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Debug", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="yes", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:Debug wxDLL", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Debug wxDLL", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:DLL Debug", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:DLL Debug", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="yes", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:Release", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="no", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Release", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="yes", Debug="no", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:Release wxDLL", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", WindowsCompiler=wx_compiler, Libs=wxlibs }
  filter { "configurations:Release wxDLL", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter { "configurations:DLL Release", "platforms:x32" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", WindowsCompiler=wx_compiler, Libs=wxlibs}
  filter { "configurations:DLL Release", "platforms:x64" }
    wx_config {Unicode="yes", Version=_OPTIONS["wx_ver"], Static="no", Debug="no", Arch="x64", WindowsCompiler=wx_compiler, Libs=wxlibs }

  filter {}
end
