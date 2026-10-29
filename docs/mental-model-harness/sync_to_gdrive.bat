@echo off
set "DEST=G:\My Drive\UCLan\SpheresIO-MentalModel-Harness"
echo Syncing SpheresIO-MentalModel-Harness to %DEST%...

if not exist "%DEST%" (
    mkdir "%DEST%"
)

robocopy "%~dp0." "%DEST%" /MIR /XD .git
echo Sync complete!
pause
