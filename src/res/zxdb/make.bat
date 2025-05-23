@echo off

rem checks. compile if needed
rem where /q rar.exe || (echo cannot find rar.exe in path && exit /b)
where /q python.exe || (echo cannot find python.exe in path && exit /b)
where /q sqlite3.exe || (cl sqlite3.c shell.c /MT || exit /b)
where /q zxdb2txt.exe || (cl zxdb2txt.c sqlite3.c -DDEV=0 /MT /Ox /Oy || exit /b)
where /q encrypt.exe || (cl encrypt.c /MT || exit /b)
if exist *.obj del *.obj
if exist Z*.sql* del Z*.sql*

rem clone
rd /q /s ZXDB >nul 2>nul
git clone --depth 1 https://github.com/ZXDB/ZXDB && ^
pushd ZXDB && (git log --oneline --pretty="#define ZXDB_VERSION \"%%s\"" ZXDB_mysql.sql.zip > ..\ZXDB_version.h) && popd && ^
python -m zipfile -e ZXDB\ZXDB_mysql.sql.zip . && ^
python ZXDB\scripts\ZXDB_to_SQLite.py && ^
rd /q /s ZXDB >nul 2>nul

rem generate full zxdb, 133 mib
if not exist ZXDB_sqlite.sql (echo Cannot find ZXDB_sqlite.sql && exit /b)
echo.|sqlite3 -init ZXDB_sqlite.sql  ZXDB_FULL.sqlite

rem generate lite zxdb, 55 mib
copy /y ZXDB_FULL.sqlite ZXDB.sqlite
echo.|sqlite3 -init ZXDB_trim.script ZXDB.sqlite

rem clean up
del ZXDB_mysql.sql
del ZXDB_sqlite.sql

rem prompt user
echo We have to convert the SQLite database (133 MiB) into a custom one (1 MiB).
echo The conversion is painfully slow, though.
choice /C YN /M "Convert? "

rem convert if requested
if "%errorlevel%"=="1" (
python -m gzip -d Spectral.db.gz && git add Spectral.db

rem do 0..64K range in reverse order to avoid being threated as false positive (Trojan:Win32/Wacatac.B!ml) (Windows Defender)
zxdb2txt 65536..0 > Spectral.db && python -m gzip --best Spectral.db && echo Ok!

rem copy /y Spectral.db.gz app.bin
rem where rar.exe && rar a Spectral.db.rar Spectral.db
rem encrypt Spectral.db.gz  Spectral.db.gz  FuckM$Defender 72
rem encrypt Spectral.db.rar Spectral.db.rar FuckM$Defender 72
    
git diff Spectral.db >> Spectral.db.diff && git add Spectral.db.diff && git rm Spectral.db -f
)
