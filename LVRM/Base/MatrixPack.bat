mkdir ..\Package
mkdir ..\Package\TMatrixPackage
del /Q ..\Package\TMatrixPackage\*.*
for %%i in (TMatrix.* TVector.* TMatrixUtility.* Affine.h Base.doxy) do copy %%i ..\Package\TMatrixPackage\%%i
cd ..\Package\TMatrixPackage
echo /** @mainpage ポータブルテンプレート行列クラスライブラリ > PTM.h
echo     @ref PTM "ポータブルテンプレート行列クラスライブラリ" >> PTM.h
echo     @ref Affine "アフィン行列クラスライブラリ" >> PTM.h
echo */ >> PTM.h

doxygen Base.doxy
start html\index.html
lha32 u PTM.2001...lzh *.h
lha32 u PTMDoc.2001...lzh html\
:end
