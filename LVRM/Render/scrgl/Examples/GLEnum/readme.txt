GL Enum
--------
By Lucian Wischik. www.wischik.com/lu

This program goes through the GL drivers on your system
(OPENGL32.DLL, 3dfxopengl.dll, 3dfxquake.dll, ...)
For each driver, it enumerates all pixel formats.
You can save the list to a comma-separated file ready
for importing into Excel or whatever.

You will have to ensure that the box of search paths,
to the top right of the application window, includes
all the drivers you want to list.

Note to 3dfx users: this program will cause the screen
to switch mode up to 20 times in the space of several seconds.
If you are worried that this might damage your monitor, do
not run the program! (Or at least remove your 3dfx drivers
from the list box of driver search paths). During this time
the screen will be black.


Source code:
The source code is for Borland C++Builder 4. It uses the
'ScrGL' package to work through the list of driver search
paths, and to get the version information from the DLLs.
This package can be downloaded from my web site:
www.wischik.com/lu/programmer.html#opengl


