@echo off

set main_folder=".\"

del /S /Q %main_folder%\*.orig
del /S /Q %main_folder%\*.bak
del /S /Q %main_folder%\*~Pisha
del /S /Q %main_folder%\*~HEAD
del /S /Q %main_folder%\*.obj