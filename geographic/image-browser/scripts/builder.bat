@ECHO OFF

REM   Run QMake
echo Running QMake
qmake

REM  Run NMake
echo Running NMake
nmake -f Makefile
if not "%ERRORLEVEL%" == "0" ( return )

REM Copying information
if not exist "release\ImageBrowser" ( mkdir "release\ImageBrowser" )
xcopy /Y release\ImageBrowser.exe               release\ImageBrowser\
xcopy /Y C:\opt\local\bin\gdal*.dll             release\ImageBrowser\
xcopy /Y C:\opt\local\bin\boost_filesystem*dll  release\ImageBrowser\
xcopy /Y C:\opt\local\bin\boost_system*.dll     release\ImageBrowser\

xcopy /Y C:\OpenSSL\bin\*.dll release\ImageBrowser\

REM  Copying html and icons
if not exist "release\ImageBrowser\html"  ( mkdir "release\ImageBrowser\html"  )
if not exist "release\ImageBrowser\icons" ( mkdir "release\ImageBrowser\icons" )

xcopy /Y src\html\*   "release\ImageBrowser\html"
xcopy /Y src\icons\*  "release\ImageBrowser\icons"
