set ProjectName=MySample
set ProgramName=VMWalk.exe
set VrmlFile=2.wrl

cd c:\home\demo\project\humanscale\%ProjectName%
del stop
%ProgramName% %VrmlFile%
