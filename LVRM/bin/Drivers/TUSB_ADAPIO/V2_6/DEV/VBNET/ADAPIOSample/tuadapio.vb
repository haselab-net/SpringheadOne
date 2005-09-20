Option Strict Off
Option Explicit On
Module TUSBADAPIO
	Declare Function Adapio_Device_Open Lib "TUADAPIO.DLL" (ByVal id As Short) As Short
	Declare Sub Adapio_Device_Close Lib "TUADAPIO.DLL" (ByVal id As Short)
	'UPGRADE_NOTE: dir は dir_Renamed にアップグレードされました。 詳細については、'ms-help://MS.VSCC/commoner/redir/redirect.htm?keyword="vbup1061"' をクリックしてください。
	Declare Function Adapio_Pio_HighDirection Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal dir_Renamed As Byte) As Short
	'UPGRADE_NOTE: dir は dir_Renamed にアップグレードされました。 詳細については、'ms-help://MS.VSCC/commoner/redir/redirect.htm?keyword="vbup1061"' をクリックしてください。
	Declare Function Adapio_Pio_LowDirection Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal dir_Renamed As Byte) As Short
	Declare Function Adapio_Pio_LowByteWrite Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal dat As Byte) As Short
	Declare Function Adapio_Pio_HighByteWrite Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal dat As Byte) As Short
	Declare Function Adapio_Pio_LowByteRead Lib "TUADAPIO.DLL" (ByVal id As Short, ByRef dat As Byte) As Short
	Declare Function Adapio_Pio_HighByteRead Lib "TUADAPIO.DLL" (ByVal id As Short, ByRef dat As Byte) As Short
	Declare Function Adapio_Pio_WordWrite Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal dat As Integer) As Short
	Declare Function Adapio_Pio_WordRead Lib "TUADAPIO.DLL" (ByVal id As Short, ByRef dat As Integer) As Short
	Declare Function Adapio_Pio_Clock_Out Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal count As Integer) As Short
	Declare Function Adapio_Pio_Clock_Stop Lib "TUADAPIO.DLL" (ByVal id As Short) As Short
	Declare Function Adapio_Dac_Out Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal ch As Short, ByVal dat As Byte) As Short
	Declare Function Adapio_Adc_SingleSample Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal ch As Short, ByRef dat As Short) As Short
	Declare Function Adapio_Adc_DigitalTrg Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal EndCh As Short, ByVal buffSize As Short) As Short
	Declare Function Adapio_Adc_AnalogTrg Lib "TUADAPIO.DLL" (ByVal id As Short, ByVal EndCh As Short, ByVal buffSize As Short, ByVal threshold As Short, ByVal TrgCh As Short, ByVal upOrDown As Short) As Short
	Declare Function Adapio_Adc_GetStatus Lib "TUADAPIO.DLL" (ByVal id As Short, ByRef Running As Short, ByRef sampledNum As Short) As Short
	Declare Function Adapio_Adc_GetDatas Lib "TUADAPIO.DLL" (ByVal id As Short, ByRef databuf As Short, ByVal Leng As Short) As Short
End Module