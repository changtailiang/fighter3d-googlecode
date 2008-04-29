#ifndef __incl_Object3D_h
#define __incl_Object3D_h

#include "../App Framework/System.h"
#include <GL/gl.h>
#include "../Math/xMath.h"

class Object3D
{
    protected:
        xMatrix mLocationMatrix;

        virtual void RenderObject() = 0;

    public:
        virtual ~Object3D() {}

        void Translate(xFLOAT x, xFLOAT y, xFLOAT z);
        void Rotate   (xFLOAT rotX, xFLOAT rotY, xFLOAT rotZ);

        void Render()
        {
            glPushMatrix();
            glMultMatrixf(&mLocationMatrix.x0);
            RenderObject();
            glPopMatrix();
        }

        Object3D ();
        Object3D (xFLOAT x, xFLOAT y, xFLOAT z);
        Object3D (xFLOAT x, xFLOAT y, xFLOAT z,
                  xFLOAT rotX, xFLOAT rotY, xFLOAT rotZ);
};

#endif
