@echo off
for %%a in (genSplit.dpr) do dcc32 -cc %%a
genSplit
