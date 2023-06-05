@echo off

cd %APPVEYOR_BUILD_FOLDER%\.build\build

cmake .. -DBUILD_STATIC=ON -G "Visual Studio 17 2022" -A Win32

for %%c in (Release Debug RelWithDebInfo) do (
    msbuild .\PACKAGE.vcxproj /p:Configuration="%%c" /p:Platform=Win32 /m ^
        /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll" ^
        /nologo /ConsoleLoggerParameters:NoSummary;Verbosity=quiet
)
