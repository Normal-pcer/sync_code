@echo off
python codeZip.py
del /s /q /f .\*.exe
del /s /q /f .\*.in
del /s /q /f .\*.out
del /s /q /f .\*.ans
del /s /q /f __temp.cpp
del /s /q /f *.dll
del /s /q /f *.pdb
del /s /q /f *.o
