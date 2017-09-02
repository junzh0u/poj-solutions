@echo off

set puzzle=simple
echo.
echo Testing input file "%puzzle%.in" ...
copy %puzzle%.in puzzle.in >nul
puzzle
fc puzzle.out %puzzle%.asc | fctest
if errorlevel 1 goto fail
echo Test case passed.
pause

set puzzle=monalisa
echo.
echo Testing input file "%puzzle%.in" ...
copy %puzzle%.in puzzle.in >nul
puzzle
fc puzzle.out %puzzle%.asc | fctest
if errorlevel 1 goto fail
echo Test case passed.
pause

set puzzle=bear
echo.
echo Testing input file "%puzzle%.in" ...
copy %puzzle%.in puzzle.in >nul
puzzle
fc puzzle.out %puzzle%.asc | fctest
if errorlevel 1 goto fail
echo Test case passed.
pause

set puzzle=gemcert
echo.
echo Testing input file "%puzzle%.in" ...
copy %puzzle%.in puzzle.in >nul
puzzle
fc puzzle.out %puzzle%.asc | fctest
if errorlevel 1 goto fail
echo Test case passed.
pause

set puzzle=pcktwtch
echo.
echo Testing input file "%puzzle%.in" ...
copy %puzzle%.in puzzle.in >nul
puzzle
fc puzzle.out %puzzle%.asc | fctest
if errorlevel 1 goto fail
echo Test case passed.
pause

echo.
echo ALL TEST CASES PASSED!!!
goto end

:fail
echo Test case failed!

:end
