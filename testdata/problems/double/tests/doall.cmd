@echo off

for /l %%i in (0,1,9) do for /l %%j in (0,1,9) do if exist %%i%%j.t copy %%i%%j.t %%i%%j > nul

for %%a in (*.dpr) do dcc32 -cc %%a

doRandom.exe 349807513 > 13
doRandom.exe 349807514 > 14
doRandom.exe 349807515 > 15
doRandom.exe 349807516 > 16
doRandom.exe 349807517 > 17
doRandom.exe 349807518 > 18
doRandom.exe 349807519 > 19
doRandom.exe 349807520 > 20
