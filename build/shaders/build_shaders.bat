@echo off
pushd %~dp0

rem Locate shader compiler.
if not defined VULKAN_SDK (
	echo Could not find Vulkan SDK!
	goto EXIT
)
%VULKAN_SDK%\\Bin\\glslc.exe default.vert -o default_vert.spv
%VULKAN_SDK%\\Bin\\glslc.exe default.frag -o default_frag.spv

rem Convert to legible header.
bin2h default_vert.spv default_frag.spv

rem Cleanup.
del default_vert.spv
del default_frag.spv
move shader.h ..\..\src\mars
:EXIT
popd