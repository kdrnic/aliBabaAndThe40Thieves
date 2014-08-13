@echo off
del ..\gbdk\bin\*.c
del ..\gbdk\bin\*.h
del ..\gbdk\bin\*.gb
del ..\gbdk\bin\*.log
copy *.c ..\gbdk\bin\
copy *.h ..\gbdk\bin\
cd ..\gbdk\bin
lcc -o alibaba.gb main.c > build.log 2>&1
copy *.gb ..\..\AliBabaAndThe40Thieves\
copy *.log ..\..\AliBabaAndThe40Thieves\
cd ..\..\AliBabaAndThe40Thieves
cls
type build.log
pause
del build.log