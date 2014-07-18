if _ACTION == "clean" then
	os.rmdir("Include")
end

solution "Consor"
	language "C++"
	location "Projects"
	targetdir "Binaries"
	configurations { "ReleaseShared", "DebugShared", "DebugStatic", "ReleaseStatic", "Debug" }

	configuration "Debug or DebugShared"
		flags { "Symbols" }
	configuration "Release or ReleaseShared"
		flags { "Optimize" }
	
	project "Consor"
		files
		{
			"Source/**.hpp", "Source/**.cpp"
		}
		vpaths
		{
			["Source Files"] = "Source/**.cpp",
			["Header Files"] = "Source/**.hpp"
		}

		configuration "windows"
			libdirs { "ThirdParty/Libraries" }
			includedirs { "ThirdParty/Include" }

		configuration "linux"
			buildoptions { "-std=c++11" }
			links { "pthread" }
			postbuildcommands {
				"mkdir -p ../Include/Consor",
				"cp -r --target-directory=../Include/Consor/ ../Source/*.hpp",
				
				"mkdir -p ../Include/Consor/Containers",
				"cp -r --target-directory=../Include/Consor/Containers/ ../Source/Containers/*.hpp",
				
				"mkdir -p ../Include/Consor/Controls",
				"cp -r --target-directory=../Include/Consor/Controls/ ../Source/Controls/*.hpp",
				
				"mkdir -p ../Include/Consor/Util",
				"cp -r --target-directory=../Include/Consor/Util/ ../Source/Util/*.hpp",
			}

		configuration "DebugStatic"
			targetsuffix "_sd"
		configuration "ReleaseStatic"
			targetsuffix "_s"
		configuration "DebugShared"
			targetsuffix "_d"

		configuration "DebugStatic or ReleaseStatic"
			kind "StaticLib"
		configuration "DebugShared or ReleaseShared"
			kind "SharedLib"
			defines { "CONSOR_SHARED" }
		configuration "Debug"
			kind "ConsoleApp"

		configuration { "linux", "DebugShared or ReleaseShared or Debug"}
			links { "boost_locale" }
		configuration { "windows", "DebugShared" }
			links { "boost_locale_d" }
		configuration { "windows", "ReleaseShared" }
			links { "boost_locale" }

		configuration "linux"
			excludes { "Source/WindowsInputSystem.cpp", "Source/WindowsConsoleRenderer.cpp" }
		configuration "windows"
			excludes { "Source/LinuxInputSystem.cpp", "Source/ANSIConsoleRenderer.cpp" }
