Attribute VB_Name = "TUSBADAPIO"
Declare Function Adapio_Device_Open Lib "TUADAPIO.DLL" (ByVal id As Integer) As Integer
Declare Sub Adapio_Device_Close Lib "TUADAPIO.DLL" (ByVal id As Integer)
Declare Function Adapio_Pio_HighDirection Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal dir As Byte) As Integer
Declare Function Adapio_Pio_LowDirection Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal dir As Byte) As Integer
Declare Function Adapio_Pio_LowByteWrite Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal dat As Byte) As Integer
Declare Function Adapio_Pio_HighByteWrite Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal dat As Byte) As Integer
Declare Function Adapio_Pio_LowByteRead Lib "TUADAPIO.DLL" (ByVal id As Integer, ByRef dat As Byte) As Integer
Declare Function Adapio_Pio_HighByteRead Lib "TUADAPIO.DLL" (ByVal id As Integer, ByRef dat As Byte) As Integer
Declare Function Adapio_Pio_WordWrite Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal dat As Long) As Integer
Declare Function Adapio_Pio_WordRead Lib "TUADAPIO.DLL" (ByVal id As Integer, ByRef dat As Long) As Integer
Declare Function Adapio_Pio_Clock_Out Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal count As Long) As Integer
Declare Function Adapio_Pio_Clock_Stop Lib "TUADAPIO.DLL" (ByVal id As Integer) As Integer
Declare Function Adapio_Dac_Out Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal ch As Integer, ByVal dat As Byte) As Integer
Declare Function Adapio_Adc_SingleSample Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal ch As Integer, ByRef dat As Integer) As Integer
Declare Function Adapio_Adc_DigitalTrg Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal EndCh As Integer, ByVal buffSize As Integer) As Integer
Declare Function Adapio_Adc_AnalogTrg Lib "TUADAPIO.DLL" (ByVal id As Integer, ByVal EndCh As Integer, ByVal buffSize As Integer, ByVal threshold As Integer, ByVal TrgCh As Integer, ByVal upOrDown As Integer) As Integer
Declare Function Adapio_Adc_GetStatus Lib "TUADAPIO.DLL" (ByVal id As Integer, ByRef Running As Integer, ByRef sampledNum As Integer) As Integer
Declare Function Adapio_Adc_GetDatas Lib "TUADAPIO.DLL" (ByVal id As Integer, ByRef databuf As Integer, ByVal Leng As Integer) As Integer

