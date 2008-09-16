#ifndef __incl_MotionCapture_CaptureInput_h
#define __incl_MotionCapture_CaptureInput_h

#include "../Utils/Singleton.h"
#include "../Models/lib3dx/xSkeleton.h"

#define g_CaptureInput CaptureInput::GetSingleton()

class CaptureInput : public Singleton<CaptureInput>
{
private:
    const xSkeleton *Spine;

public:
    bool Initialize(const xSkeleton &spine)
    {
        this->Spine = &spine;
        return true;
    }
    void Finalize()
    {}

    xQuaternion * GetTransformations()
    {
        xQuaternion *QT_bones = new xQuaternion[Spine->I_bones];

        // kwaternion QT_bones[0] nie opisuje obrotu, a przesuni�cie ca�ego modelu, pozosta�e to obroty w formacie
        // x = axis.x * sin(alpha/2)
        // y = axis.y * sin(alpha/2)
        // z = axis.z * sin(alpha/2)
        // w = cos(alpha/2)
        // o� Z wskazuje do g�ry

        for (int i=0; i < Spine->I_bones; ++i)
            QT_bones[i].zeroQ(); // no rotation

        return QT_bones;
    }

    CaptureInput() {}
   ~CaptureInput() {}

private:
     // disable copy constructor & assignment operator 
    CaptureInput(const CaptureInput&) {}
    CaptureInput& operator=(const CaptureInput&) { return *this; }
};

#endif
