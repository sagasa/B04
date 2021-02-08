@echo off
copy /Y ..\Release\TeamB04.exe GameB04.exe /D
xcopy ..\Assets Assets /D /S /R /Y /I /K

pause