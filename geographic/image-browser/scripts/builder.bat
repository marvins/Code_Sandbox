@ECHO OFF

REM   Run QMake
echo Running QMake
qmake

REM  Run NMake
echo Running NMake
nmake -f Makefile

REM Copying information
if not exist "release\ImageBrowser" ( mkdir "release\ImageBrowser" )
xcopy release\ImageBrowser.exe               release\ImageBrowser\
xcopy C:\opt\local\bin\gdal*.dll             release\ImageBrowser\
xcopy C:\opt\local\bin\boost_filesystem*dll  release\ImageBrowser\
xcopy C:\opt\local\bin\boost_system*.dll     release\ImageBrowser\
