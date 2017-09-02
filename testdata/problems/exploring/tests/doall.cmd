@echo off

for /l %%i in (0,1,9) do for /l %%j in (0,1,9) do if exist %%i%%j.t copy %%i%%j.t %%i%%j > nul

for %%i in (*.dpr) do dcc32 -cc %%i

fill 9 A                          > 03
genab 5                           > 04
genab 10                          > 05
genrandtree 10 A B 1234           > 06
genbigtree 10 5134                > 07

fill 99  A                        > 08
fill 100 A                        > 09
fill 299 Z                        > 10
fill 300 Z                        > 11

combine < 12.pre                  > 12
combine < 13.pre                  > 13
combine < 14.pre                  > 14
combine < 15.pre                  > 15

genrandtree  50 A Z 2514          > 16
genrandtree 100 A Z 3114          > 17
genrandtree 120 A Z 4589          > 18
genrandtree 149 A Z 1415          > 19
genrandtree 149 A Z 1155          > 20
genrandtree 149 Q Z 1524          > 21
genrandtree 149 A B 5124          > 22
genrandtree 149 A B 1151          > 23

genbigtree 100 1523               > 24
genbigtree 149 6256               > 25

combine < 26.pre                  > 26

genab 100                         > 27
genab 148                         > 28
