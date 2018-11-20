@echo off

:set_qt_toolchain_part
if /i "%1" == "msvc14" (
  set QT_TOOLCHAIN_PART=msvc2015
  goto :set_qt_platform_part
)

echo Invalid argument: "%1"
exit -1

:set_qt_platform_part
if /i "%2" == "x86" (
  set QT_PLATFORM_PART=
  goto :compose_result
)
if /i "%2" == "x64" (
  set QT_PLATFORM_PART=_64
  goto :compose_result
)

echo Invalid argument: "%2"
exit -1

:compose_result
set CMAKE_QT_DIR=C:/Qt/5.11.2/%QT_TOOLCHAIN_PART%%QT_PLATFORM_PART%
echo set CMAKE_QT_DIR=%CMAKE_QT_DIR%