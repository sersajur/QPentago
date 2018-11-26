@echo off

:set_generator_part
if /i "%1" == "msvc15" (
  set CMAKE_GENERATOR_PART=Visual Studio 15 2017
  goto :set_platform_part
)

echo Invalid argument: "%1"
exit -1

:set_platform_part
if /i "%2" == "x64" (
  set CMAKE_PLATFORM_PART= Win64
  goto :compose_result
)

echo Invalid argument: "%2"
exit -1

:compose_result
set CMAKE_GENERATOR=%CMAKE_GENERATOR_PART%%CMAKE_PLATFORM_PART%
echo set CMAKE_GENERATOR=%CMAKE_GENERATOR%
