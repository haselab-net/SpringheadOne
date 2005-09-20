REM CommandInterpreter: $(COMSPEC)
del VRBCB.lib
TLIB /P128 VRBCB.lib +Base\BaseBCB.lib +Device\DeviceBCB.lib +HIS\HISBCB.lib +Render\RenderBCB.lib +WinCmp\WinCmpBCB.lib
del *.~???
