# 7-Zip
My own 7-Zip build.

VC2012:  
1. StartMenu -> Microsoft Visual Studio 2012 -> Open VS2012 x64 Native Tools Command Prompt  
2. set CPU=AMD64  
3. cd 7z938\CPP\7zip\Bundles\Format7zF  
4. nmake -f makefile NEW_COMPILER=1  
5. cd 7z938\CPP\7zip\UI\Explorer  
6. nmake -f makefile NEW_COMPILER=1  
7. copy 7z.dll && 7-zip.dll to C:\Program Files\7-Zip  
8. restart your computer  
