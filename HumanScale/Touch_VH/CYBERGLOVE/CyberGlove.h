00001 /*************** <auto-copyright.pl BEGIN do not edit this line> **************
00002  *
00003  * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
00004  *
00005  * Original Authors:
00006  *   Allen Bierbaum, Christopher Just,
00007  *   Patrick Hartling, Kevin Meinert,
00008  *   Carolina Cruz-Neira, Albert Baker
00009  *
00010  * This library is free software; you can redistribute it and/or
00011  * modify it under the terms of the GNU Library General Public
00012  * License as published by the Free Software Foundation; either
00013  * version 2 of the License, or (at your option) any later version.
00014  *
00015  * This library is distributed in the hope that it will be useful,
00016  * but WITHOUT ANY WARRANTY; without even the implied warranty of
00017  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
00018  * Library General Public License for more details.
00019  *
00020  * You should have received a copy of the GNU Library General Public
00021  * License along with this library; if not, write to the
00022  * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
00023  * Boston, MA 02111-1307, USA.
00024  *
00025  * -----------------------------------------------------------------
00026  * File:          CyberGlove.h,v
00027  * Date modified: 2003/03/05 16:54:50
00028  * Version:       1.35
00029  * -----------------------------------------------------------------
00030  *
00031  *************** <auto-copyright.pl END do not edit this line> ***************/
00032 
00033 #ifndef _GADGET_CYBER_GLOVE_H_
00034 #define _GADGET_CYBER_GLOVE_H_
00035 
00036 #include <gadget/Devices/DriverConfig.h>
00037 #include <gadget/Type/Input.h>
00038 #include <gadget/Type/Glove.h>
00039 #include <gadget/Devices/VirtualTechnologies/CyberGlove/CyberGloveBasic.h>
00040 
00041 
00042 namespace gadget
00043 {
00044    class InputManager;
00045 }
00046 
00047 extern "C" GADGET_DRIVER_API(void) initDevice(gadget::InputManager* inputMgr);
00048 
00049 namespace gadget
00050 {
00051 
00053 class CyberGlove : virtual public Input, public Glove
00054 {
00055 public:
00057    CyberGlove() : mGlove( NULL ), mCalDir( NULL )
00058    {
00059    }
00060 
00062    virtual ~CyberGlove();
00063 
00064    virtual bool config(jccl::ConfigChunkPtr c);
00065 
00066    static std::string getChunkType() { return std::string("CyberGlove");}
00067 
00068    virtual int startSampling();
00069    virtual int stopSampling();
00070    virtual int sample();
00071    virtual void updateData ();
00072 
00077    void operator delete(void* p)
00078    {
00079       ::operator delete(p);
00080    }
00081 
00082 protected:
00087    virtual void destroy()
00088    {
00089       delete this;
00090    }
00091 
00093    void controlLoop(void* nullParam);
00094 
00095    void copyDataFromGlove();
00096 
00097 protected:
00098    CyberGloveBasic*  mGlove;              
00099    char*             mCalDir;             
00100    std::string       mPortName;
00101    int               mBaudRate;
00102 };
00103 
00104 } // End of gadget namespace
00105 
00106 #endif   /* _GADGET_CYBER_GLOVE_H_ */

