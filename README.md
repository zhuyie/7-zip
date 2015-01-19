# 7-Zip
My own 7-Zip build.

VC2012:
a. StartMenu -> Microsoft Visual Studio 2012 -> Open VS2012 x64 Native Tools Command Prompt
b. set CPU=AMD64
c. cd 7z938\CPP\7zip\Bundles\Format7zF
d. nmake -f makefile NEW_COMPILER=1
e. cd 7z938\CPP\7zip\UI\Explorer
f. nmake -f makefile NEW_COMPILER=1
g. copy 7z.dll && 7-zip.dll to C:\Program Files\7-Zip
h. restart your computer
