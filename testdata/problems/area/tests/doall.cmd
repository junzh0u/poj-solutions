@echo off

for /l %%i in (0,1,9) do for /l %%j in (0,1,9) do if exist %%i%%j.t copy %%i%%j.t %%i%%j > nul

for %%a in (*.dpr) do dcc32 -cc %%a

doRandom.exe      3  10  10  2 max          34250821 > 11
doRandom.exe      3  10  10 24 inf          34250822 > 12
doRandom.exe     10  10   1 24 max          34250813 > 13
doRandom.exe     10   1  10 24 max          34250814 > 14

doSmall.exe       5 100 100 26 min          34250815 > 15
doSmall.exe      10 100 100 26 min          34250816 > 16

doLine.exe       20  50  50 26 impossible   34250817 > 17
doLine.exe       30  50  20  2 max          34250818 > 18

doGrid.exe       20  13 10  -10 13  2 min       34250819 > 19
doGrid.exe       30  7   9   -9  7  2 max       34250820 > 20

doSmall.exe      30 100 100  1 max          34250821 > 21
doSmall.exe      40 100 100  2 max          34250822 > 22

doRandom.exe     15 100 100 24 impossible   34250824 > 23

doLine.exe       85 100  10 26 max          34250824 > 24
doLine.exe       90  10 100 15 max          34342525 > 25

doGrid.exe       100 2 2 -2 2 26 max        34250826 > 26
doGrid.exe       100 3 5 -1 7 26 min        34250827 > 27
doGrid.exe       100 3 5  5 -3 26 max       34250828 > 28

doRandom.exe     85 100  10 26 max          34250829 > 29
doRandom.exe     90  10 100 15 max          34250828 > 30

doSmall.exe      97 100 100 26 min          34250821 > 31
doSmall.exe      96 100 100 26 min          34250822 > 32

doLine.exe       93 100 100 26 impossible   34250823 > 33
doLine.exe       95 100 100  2 max          34250824 > 34
doLine.exe       98 100 100 14 min          34250825 > 35
doLine.exe       97 100 100 17 inf          34250826 > 36

doRandom.exe    100 100 100 25 random       34250827 > 37
doRandom.exe    100 100 100 24 min          34250828 > 38
doRandom.exe    100 100 100 26 impossible   34250829 > 39
doRandom.exe    100 100 100  1 max          34250828 > 40
