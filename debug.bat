@echo off

set includes=/I dependencies\Glad\include /I dependencies\GLFW\include /I src /I dependencies\stb\include /I dependencies\tiny\include /I dependencies\OpenFBX\include
set libs=dependencies\GLFW\lib\glfw3.lib dependencies\Glad\lib\glad.lib Shell32.lib User32.lib Gdi32.lib OpenGL32.lib msvcrt.lib
set compile_flags=/DDEBUG
set link_flags=/DEBUG /NODEFAULTLIB:LIBCMT
set flags=/Zi /EHsc

rem Libraries
cl %flags% /c dependencies\stb\src\*.cpp  /I dependencies\stb\include
cl %flags% /c dependencies\tiny\src\*.cpp /I dependencies\tiny\include

rem Source
cl %flags% /c src/universal/vecs/*.cpp %compile_flags%
cl %flags% /c src/universal/mats/*.cpp %compile_flags%
cl %flags% /c src/universal/*.cpp %compile_flags%

cl %flags% /c src/engine/*.cpp %includes% %compile_flags%
cl %flags% /c src/platform/*.cpp %includes% /DUSE_DEDICATED_GPU

cl %flags% /c src/simulation/*.cpp %includes% %compile_flags%

cl %flags% /c src/main.cpp %includes% %compile_flags%

rem Resources
rc resources.rc

rem Link and Make Executable
link *.obj *.res %libs% /OUT:solar-system-debug.exe %link_flags%

rem Delete Intermediate Files
del *.obj
del *.res
del *.lib
del *.exp