@REM set "CC=%LIBRARY_BIN%\gcc.exe"
@REM set "CXX=%LIBRARY_BIN%\g++.exe"
set "CC=gcc.exe"
set "CXX=g++.exe"
set "FORTRAN=%LIBRARY_BIN%\gfortran.exe"
@REM -DCMAKE_C_COMPILER="%LIBRARY_BIN%\gcc.exe" ^
@REM -DCMAKE_CXX_COMPILER="%LIBRARY_BIN%\g++.exe" ^
@REM -DCMAKE_CXX_FLAGS="--std=c++14 -U__STRICT_ANSI__" ^

cmake %SRC_DIR% ^
    -DCMAKE_BUILD_SHARED=ON ^
    -DCMAKE_CXX_FLAGS="--std=c++14 -D_USE_MATH_DEFINES" ^
    -G "MinGW Makefiles" ^
    -DCMAKE_INSTALL_PREFIX="%PREFIX%"

if errorlevel 1 exit 1

cmake --build . --target install
if errorlevel 1 exit 1
