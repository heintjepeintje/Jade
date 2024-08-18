project "JadeEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	objdir "Binaries/%{cfg.buildcfg}/Intermediate"
	targetdir "Binaries/%{cfg.buildcfg}/Output"

	files { "Source/**.cpp", "Source/**.hpp" }

	includedirs {
		"Source",
		"Libraries/glm",
		os.getenv("VK_SDK_PATH") .. "/Include"
	}
	
	warnings "High"

	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "configurations:Dist"
		defines { "NDEBUG" }
		optimize "On"