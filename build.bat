@echo off

set includes=/I dependencies\Glad\include /I dependencies\GLFW\include /I src /I dependencies\stb\include /I dependencies\tiny\include /I dependencies\OpenFBX\include
set libs=dependencies\GLFW\lib\glfw3.lib dependencies\Glad\lib\glad.lib Shell32.lib User32.lib Gdi32.lib OpenGL32.lib msvcrt.lib

rem Libraries
cl /Ox /EHsc /c dependencies\stb\src\*.cpp  /I dependencies\stb\include
cl /Ox /EHsc /c dependencies\tiny\src\*.cpp /I dependencies\tiny\include

rem Source
cl /Ox /EHsc /c src/universal/vecs/*.cpp
cl /Ox /EHsc /c src/universal/mats/*.cpp
cl /Ox /EHsc /c src/universal/*.cpp

cl /Ox /EHsc /c src/engine/*.cpp %includes%
cl /Ox /EHsc /c src/platform/*.cpp %includes% /DUSE_DEDICATED_GPU

cl /Ox /EHsc /c src/simulation/*.cpp %includes%

cl /Ox /EHsc /c src/main.cpp %includes%

rem Resources
rc resources.rc

rem Link and Make Executable
link *.obj *.res %libs% /OUT:solar-system.exe /NODEFAULTLIB:LIBCMT /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup
@REM link *.obj *.res %libs% /OUT:solar-system.exe /NODEFAULTLIB:LIBCMT

rem Delete Intermediate Files
del *.obj
del *.res
del *.lib
del *.exp