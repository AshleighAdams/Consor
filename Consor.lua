solution "Consor"
	language "C++"
	location "Projects"
	targetdir "Binaries"
	includedirs { "Include" }
	configurations { "Debug", "Release", "DebugShared", "ReleaseShared" }

	configuration "Debug or DebugShared"
		flags { "Symbols" }
	configuration "Release or ReleaseShared"
		flags { "Optimize" }

	project "Consor"
		files
		{
			"Consor/**.hpp", "Consor/**.cpp"
		}
		vpaths
		{
			["Source Files"] = "Consor/**.cpp",
			["Header Files"] = "Consor/**.hpp"
		}

		configuration "windows"
			libdirs { "ThirdParty/Libraries" }
			includedirs { "ThirdParty/Include" }

		configuration "linux"
			buildoptions { "-std=c++11" }

		configuration "Debug"
			targetsuffix "_sd"
		configuration "Release"
			targetsuffix "_s"
		configuration "DebugShared"
			targetsuffix "_d"

		configuration "Debug or Release"
			kind "StaticLib"
		configuration "DebugShared or ReleaseShared"
			kind "SharedLib"
			defines { "CONSOR_SHARED" }

		configuration { "linux", "DebugShared or ReleaseShared"}
			links { "boost_locale" }
		configuration { "windows", "DebugShared" }
			links { "boost_locale_d" }
		configuration { "windows", "ReleaseShared" }
			links { "boost_locale" }

		configuration "linux"
			excludes { "Consor/WindowsInputSystem.cpp", "Consor/WindowsConsoleRenderer.cpp" }
		configuration "windows"
			excludes { "Consor/LinuxInputSystem.cpp", "Consor/ANSIConsoleRenderer.cpp" }
