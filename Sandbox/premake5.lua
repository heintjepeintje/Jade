project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	objdir "Binaries/%{cfg.buildcfg}/Intermediate"
	targetdir "Binaries/%{cfg.buildcfg}/Output"

	files { "Source/**.cpp", "Source/**.hpp" }

	libdirs {
		os.getenv("VK_SDK_PATH") .. "/Lib"
	}

	includedirs {
		"../JadeEngine/Source",
		"../JadeEngine/Libraries/glm",
		"../JadeEngine/Libraries/stb"
	}

	links {
		"JadeEngine",
		"vulkan-1",
		"Ws2_32"
	}

	warnings "High"

	filter "configurations:Debug"
		defines { "_DEBUG" }
		prebuildcommands {
			"glslc -Werror -g -fshader-stage=vertex ./Resources/Shaders/vertex.glsl -o ./Binaries/Debug/Shaders/vertex.spv",
			"glslc -Werror -g -fshader-stage=fragment ./Resources/Shaders/fragment.glsl -o ./Binaries/Debug/Shaders/fragment.spv"
		}
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		prebuildcommands {
			"glslc -Werror -fshader-stage=vertex ./Resources/Shaders/vertex.glsl -o ./Binaries/Release/Shaders/vertex.spv",
			"glslc -Werror -fshader-stage=fragment ./Resources/Shaders/fragment.glsl -o ./Binaries/Release/Shaders/fragment.spv"
		}
		optimize "On"

	filter "configurations:Dist"
		defines { "NDEBUG" }
		prebuildcommands {
			"glslc -Werror -fshader-stage=vertex ./Resources/Shaders/vertex.glsl -o ./Binaries/Dist/Shaders/vertex.spv",
			"glslc -Werror -fshader-stage=fragment ./Resources/Shaders/fragment.glsl -o ./Binaries/Dist/Shaders/fragment.spv"
		}
		optimize "On"