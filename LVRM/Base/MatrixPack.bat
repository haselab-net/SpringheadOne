mkdir ..\Package
mkdir ..\Package\TMatrixPackage
del /Q ..\Package\TMatrixPackage\*.*
for %%i in (TMatrix.* TVector.* TMatrixUtility.* Affine.h Base.doxy) do copy %%i ..\Package\TMatrixPackage\%%i
cd ..\Package\TMatrixPackage
echo /** @mainpage �|�[�^�u���e���v���[�g�s��N���X���C�u���� > PTM.h
echo     @ref PTM "�|�[�^�u���e���v���[�g�s��N���X���C�u����" >> PTM.h
echo     @ref Affine "�A�t�B���s��N���X���C�u����" >> PTM.h
echo */ >> PTM.h

doxygen Base.doxy
start html\index.html
lha32 u PTM.2001...lzh *.h
lha32 u PTMDoc.2001...lzh html\
:end
