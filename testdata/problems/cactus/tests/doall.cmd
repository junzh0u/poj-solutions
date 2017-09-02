@echo off

for /l %%i in (0,1,9) do for /l %%j in (0,1,9) do if exist %%i%%j.t copy %%i%%j.t %%i%%j > nul

for %%i in (*.dpr) do dcc32 -cc %%i

gencycle 10    > 05.pre
gencycle 1000  > 06.pre
gencycle 20000 > 07.pre
gencyclebotva 19999 > 08.pre

gentree 100  1415 > 09.pre
gentree 500  6256 > 10.pre
gentree 1000 5626 > 11.pre

gentwocycles 100     200 > 12.pre
gentwocycles 1000   4000 > 13.pre
gentwocycles 10000 10001 > 14.pre

gentree 100  1415 | boost 1  5    0 1224 > 15.pre
gentree 500  6256 | boost 5  10  10 5664 > 16.pre
gentree 1000 5626 | boost 1  5   10 4524 > 17.pre
gentree 100  1415 | boost 1  5   20 4564 > 18.pre
gentree 500  6256 | boost 3  10  10 1234 > 19.pre
gentree 1000 5626 | boost 5  10  10 4514 > 20.pre
gentree 50   1345 | boost 50 100  0 1344 > 21.pre

gentree 100  1415 | boost 1 5    0 1224   | addbotva 1    1241 > 22.pre
gentree 500  6256 | boost 5 10   0 5664   | addbotva 45   6526 > 23.pre
gentree 1000 5626 | boost 1 5    0 4524   | addbotva 1    1545 > 24.pre
gentree 100  1415 | boost 1 5    0 4564   | addbotva 2    6224 > 25.pre
gentree 500  6256 | boost 3 10   0 1234   | addbotva 100  6245 > 26.pre
gentree 1000 5626 | boost 5 10   0 4514   | addbotva 23   6324 > 27.pre
gentree 50   1345 | boost 50 100 0 1344 | addbotva 1    6733 > 28.pre

genbigtree 1000 | boost 1 10 0 5417 > 29.pre
genbigtree 1000 | boost 1 10 10 1347 > 30.pre
genbigtree 1000 | boost 1 5  10 2117 > 31.pre
genbigtree 1000 | boost 1 5  10 2117 | addbotva 1000 5142 > 32.pre

genbigcactus 1000  2 | shuffle 1546 > 33.pre
genbigcactus 10000 3 | shuffle 6526 > 34.pre
genbigcactus 20000 2 | shuffle 2146 > 35.pre
genbigcactus 20000 5 | shuffle 6536 > 36.pre

gentriline 100   | shuffle 4387 > 37.pre
gentriline 10001 | shuffle 1347 > 38.pre
gentriline 19999 | shuffle 1565 > 39.pre
gentriline 20000 | shuffle 6754 > 40.pre

gentricycle 100   | shuffle 4387 > 41.pre
gentricycle 10001 | shuffle 1347 > 42.pre
gentricycle 19999 | shuffle 1565 > 43.pre
gentricycle 20000 | shuffle 6754 > 44.pre


for %%i in (*.pre) do call :regen %%i

exit /b

:regen
echo regenerating test %~n1
regen < %1 > %~n1
del %1
exit /b
