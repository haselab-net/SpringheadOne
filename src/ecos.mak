include $(IHC_ROOT)/src/defs.mak

OBJS          =	HIS/HIHapticDevice.o HIS/HISpidarMotor.o HIS/HISpidar4.o \
				HIS/HISpidarCalcBase.o HIS/HISpidarCalc3DOF.o HIS/HISpidarCalc6DOF.o \
				HIS/HISpidarG6.o HIS/HISpidarG6X.o \
				Device/DRRealDevicePool.o Device/DVDeviceManager.o Device/DVVirtualDevice.o \
				Device/DVVirtualDevicePool.o \
				Device/DRRealDevice.o Device/DRIHCSh4.o \
				Base/Affine.o  Base/BaseDebug.o  Base/BaseTypeInfo.o	\
				Base/BaseUtility.o Base/StrbufBase.o	\


TARGET = $(IHC_ROOT)/lib/libSpr.a
DEPEND = ecos.mak


include $(IHC_ROOT)/src/lib.mak
