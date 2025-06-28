Package: libdwarf:x64-windows@2.0.0

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.44.35211.0
-    vcpkg-tool version: 2025-03-22-d4dd14112c2026ee57e59f966ecfa3b9137bd143
    vcpkg-scripts version: 3f8078010e 2025-06-25 (2 days ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached davea42-libdwarf-code-v2.0.0.tar.gz
-- Cleaning sources at D:/runtime/vcpkg/buildtrees/libdwarf/src/v2.0.0-ac5a06ccbf.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source D:/runtime/vcpkg/downloads/davea42-libdwarf-code-v2.0.0.tar.gz
-- Applying patch include-dir.diff
-- Applying patch dependencies.diff
-- Applying patch msvc-runtime.diff
-- Applying patch off_t.diff
-- Applying patch dwarfdump-conf.diff
-- Using source at D:/runtime/vcpkg/buildtrees/libdwarf/src/v2.0.0-ac5a06ccbf.clean
-- Configuring x64-windows
-- Building x64-windows-dbg
-- Building x64-windows-rel
CMake Error at scripts/cmake/vcpkg_execute_build_process.cmake:134 (message):
    Command failed: D:/runtime/scoop/apps/cmake/3.30.5/bin/cmake.exe --build . --config Release --target install -- -v -j21
    Working Directory: D:/runtime/vcpkg/buildtrees/libdwarf/x64-windows-rel
    See logs for more information:
      D:\runtime\vcpkg\buildtrees\libdwarf\install-x64-windows-rel-out.log

Call Stack (most recent call first):
  D:/projects/quant1x/api/vcpkg_installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_build.cmake:74 (vcpkg_execute_build_process)
  D:/projects/quant1x/api/vcpkg_installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_install.cmake:16 (vcpkg_cmake_build)
  ports/libdwarf/portfile.cmake:27 (vcpkg_cmake_install)
  scripts/ports.cmake:206 (include)



```

<details><summary>D:\runtime\vcpkg\buildtrees\libdwarf\install-x64-windows-rel-out.log</summary>

```
Change Dir: 'D:/runtime/vcpkg/buildtrees/libdwarf/x64-windows-rel'

Run Build Command(s): D:/runtime/vcpkg/downloads/tools/ninja/1.12.1-windows/ninja.exe -v -v -j21 install
[1/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_elf_rel_detector.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_elf_rel_detector.c
[2/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_form_class_names.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_form_class_names.c
[3/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_dsc.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_dsc.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_dsc.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_dsc.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_dsc.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[4/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_fill_in_attr_form.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_fill_in_attr_form.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_fill_in_attr_form.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_fill_in_attr_form.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_fill_in_attr_form.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[5/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debugaddr.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debugaddr.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_debugaddr.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debugaddr.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debugaddr.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[6/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_crc.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_crc.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_crc.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_crc.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_crc.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[7/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debug_sup.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debug_sup.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_debug_sup.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debug_sup.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debug_sup.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[8/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_abbrev.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_abbrev.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_abbrev.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_abbrev.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_abbrev.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[9/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_crc32.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_crc32.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_crc32.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_crc32.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_crc32.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[10/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_arange.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_arange.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_arange.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_arange.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_arange.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[11/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_elf_load_headers.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_elf_load_headers.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_elf_load_headers.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_elf_load_headers.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_elf_load_headers.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[12/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debuglink.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debuglink.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_debuglink.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debuglink.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debuglink.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[13/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_error.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_error.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_error.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_error.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_error.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[14/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_elfread.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_elfread.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_elfread.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_elfread.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_elfread.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[15/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_fission_to_cu.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_fission_to_cu.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_fission_to_cu.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_fission_to_cu.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_fission_to_cu.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[16/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_form.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_form.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_form.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_form.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_form.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[17/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_die_deliv.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_die_deliv.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_die_deliv.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_die_deliv.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_die_deliv.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[18/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_find_sigref.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_find_sigref.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_find_sigref.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_find_sigref.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_find_sigref.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[19/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debugnames.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debugnames.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_debugnames.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_debugnames.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_debugnames.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[20/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_alloc.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_alloc.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_alloc.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_alloc.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_alloc.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[21/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_frame2.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_frame2.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_frame2.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_frame2.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_frame2.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[22/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_frame.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_frame.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_frame.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_frame.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_frame.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
[23/114] C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_gdbindex.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_gdbindex.c
FAILED: src/lib/libdwarf/CMakeFiles/dwarf.dir/dwarf_gdbindex.c.obj 
C:\PROGRA~1\MICROS~4\2022\COMMUN~1\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe   -DLIBDWARF_BUILD -DPIC -Ddwarf_EXPORTS -ID:\runtime\vcpkg\buildtrees\libdwarf\x64-windows-rel -ID:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf -external:ID:\projects\quant1x\api\vcpkg_installed\x64-windows\include -external:W0 /nologo /DWIN32 /D_WINDOWS /utf-8 /MP  /MD /O2 /Oi /Gy /DNDEBUG /Z7 /showIncludes /Fosrc\lib\libdwarf\CMakeFiles\dwarf.dir\dwarf_gdbindex.c.obj /Fdsrc\lib\libdwarf\CMakeFiles\dwarf.dir\ /FS -c D:\runtime\vcpkg\buildtrees\libdwarf\src\v2.0.0-ac5a06ccbf.clean\src\lib\libdwarf\dwarf_gdbindex.c
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vadefs.h(61): error C2632: “__int64”后面的“int”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“long”非法
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\vcruntime.h(190): error C2632: “__int64”后面的“int”非法
ninja: build stopped: subcommand failed.
```
</details>

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "api",
  "version": "0.1.0",
  "description": "quant1x v2 engine",
  "dependencies": [
    "openssl",
    "libiconv",
    "yaml-cpp",
    "zlib",
    "mimalloc",
    "spdlog",
    "cpr",
    "fmt",
    "duktape",
    "benchmark",
    "catch2",
    "libdwarf",
    "mio",
    "flatbuffers",
    "capnproto"
  ]
}

```
</details>
