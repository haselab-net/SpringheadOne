Springhead                  June 2004 Shoichi HASEGAWA

1.Background and aim of developing
    to be translated

2.The outline of the library
    TBW

3.How to install
    1. Prepare source tree of Springhead. Please download it from
     http://springhead.info/archive/
     
    2. Set environment variable of SPRINGHEAD on the Windows system.
     The variable should indicate top directory of the source tree.
     For example, SPRINGHEAD should be "C:\Home\Project\Springhead",
     should not be "C:\Home\Project\Springhead\src"
    3. Install DirectX8 or 9 (9 is better). 

    For Visual C++ 6.0 users:
    4. Install Visual C++ Service Pack 6
    5. Oepn Visual C++ [Tool]-[Option]-[Directory] dialog.
      Add C:\DXSDK\include to the include directories
      Add C:\DXSDK\lib to the library directories
    6. Open src/samples/DynaHaptic/DynaHaptic.dsw. Select MFC Release
     configuration. Then, build and run.

    For C++ Builder 6 users:
    4. Install Borland C++Builder 6 update pack #4.
    5. Open src/SpringheadBCB.bpg and build ann projects.
    6. Open src/BCBGui/TreeViewTest/TreeTestView.bpg and run.

    For Visual C++ 7.1 users.
    Same as VC6. But we don't use VC7.1 for development. Sometimes, we
    don't update .sln .vcproj files. Adding files to some projects may 
    solve problems.
 
4. Sample program for SPIDAR4 (four strings Spidar)
    Please open  ../src/HIS/samples/Spidar/Spidar.dsw.
    You will find some project. Then, activate "Spidar" project and run it.
    Once you hit space key, the program will present virtual floor.

    The program consists of small source file "main.cpp" and some library.

    Please open  ../src/HIS/samples/Spidar/Spidar.dsw.
    You will find some project. Then, activate "Spidar" project and run it.
    Once you hit space key, the program will present virtual floor.

    The program consists of small source file "main.cpp" and some library.
    "main.cpp" have some comments. Please see it.

    If you use grip type spidar (SpidarG). please use
        .../src/HIS/samples/SpidarG6/SpidarG6.dsw.

5. License
    User can choose from GPL, Apache lisence or the following original
    lisence.
    ... to be translated
    
    This software contains paintlib code. paintlib is copyright (c) 1996-2002
    Ulrich von Zadow and other contributors
    

* Contact:
Shoichi HASEGAWA<hase@hi.pi.titech.ac.jp>  Tel:+81-45-924-5050 Fax:+81-45-924-5016
Sato & Koike group, Precision and Intelligence Lab, Tokyo institute of technology,
R2 4259 Nagatsuta-cho Midori-ku Yokohama-shi Kanagawa-ken Japan, 226-8503.
