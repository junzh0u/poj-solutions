@echo off

for /l %%i in (0,1,9) do for /l %%j in (0,1,9) do if exist %%i%%j.t copy %%i%%j.t %%i%%j > nul

cl do09-41.cpp
cl do42-50.cpp
cl do51-52.cpp
do09-41
do42-50
do51-52
