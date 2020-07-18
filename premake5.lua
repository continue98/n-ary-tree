

root_dir = os.getcwd()

workspace "2gis_test_work"
	cppdialect "C++17"
	
	configurations { "Debug32", "Release32" }

	filter "configurations:*32"
		architecture "x32"
	

	filter { "action:vs*", "toolset:msc*" }
		disablewarnings { "4996" }
		staticruntime "on"
	filter { "action:gmake*", "toolset:gcc" }
		linkoptions  { "-static -static-libstdc++" } 

	
project "2gis_test_work"
	location "build"
	language "C++"
	kind "ConsoleApp"
	targetname "N_array_tree"
	vpaths {
		["src"] = vpaths { } 
	}
	files {
		"premake5.lua",
		"**.h",
		"**.hpp",
		"**.hxx",
		"**.cpp",
		"**.cxx",
	}

	includedirs { root_dir .. "/src/3rd-party/nlohmann" }
	includedirs { root_dir .. "/src/3rd-party/fifo-map" }
	includedirs { root_dir .. "/src/3rd-party/cxxopts" }

	filter { "action:vs*", "toolset:msc*" }
		print "test"
		targetextension ".exe" 
	filter { "action:gmake*", "toolset:gcc" }
		targetextension "" 
	

