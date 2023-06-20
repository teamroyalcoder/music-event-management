@echo off
setlocal enabledelayedexpansion

REM Prompt for root directory
set /p "root_directory=Enter root directory [%CD%]: "
if "%root_directory%"=="" set "root_directory=%CD%"

REM Prompt for file extensions
set /p "file_extensions=Enter file extensions (comma-separated) [.save]: "
set "file_extensions=%file_extensions: =%"
if "%file_extensions%"=="" set "file_extensions=.save"

echo.
echo Performing recursive file deletion in "%root_directory%" with file extensions "%file_extensions%"
echo.

call :delete_files "%root_directory%"
exit /b

:delete_files
for /R "%~1" %%F in (*) do (
    set "file_name=%%~nxF"
    set "file_ext=%%~xF"
    for %%E in (%file_extensions%) do (
        if "!file_ext!"=="%%~E" (
            echo Deleting file: "%%F"
            del "%%F" >nul
        )
    )
)
for /D %%D in ("%~1\*") do (
    call :delete_files "%%D"
)
exit /b
