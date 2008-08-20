#ifndef __incl_lib3dx_xFieldOfView_h
#define __incl_lib3dx_xFieldOfView_h

#include "xMath.h"
#include "Figures/xBoxA.h"

struct xFieldOfView
{
public:
    bool    Empty;

    xPlane  LeftPlane;
    xPlane  RightPlane;
    xPlane  TopPlane;
    xPlane  BottomPlane;

    xFLOAT  Angle, Aspect;
    xFLOAT  FrontClip;
    xFLOAT  BackClip;
    
    xPoint3 EyePosition;
    xMatrix ViewTransform;
    xPoint3 Corners3D[5];
    xPlane  Planes[5];

    xFieldOfView() : Empty(true) {}

    void update()
    {
        Corners3D[0].z = FrontClip;
        Corners3D[0].y = -Corners3D[0].z * (xFLOAT)tan( Angle * 0.5f );
        Corners3D[0].x = Corners3D[0].y * Aspect;
        Corners3D[1].init(-Corners3D[0].x,  Corners3D[0].y, FrontClip);
        Corners3D[2].init(-Corners3D[0].x, -Corners3D[0].y, FrontClip);
        Corners3D[3].init( Corners3D[0].x, -Corners3D[0].y, FrontClip);
        Corners3D[4].zero();

        xMatrix mtxViewToWorld; xMatrix::Invert(ViewTransform, mtxViewToWorld);
        Corners3D[0] = mtxViewToWorld.preTransformP(Corners3D[0]);
        Corners3D[1] = mtxViewToWorld.preTransformP(Corners3D[1]);
        Corners3D[2] = mtxViewToWorld.preTransformP(Corners3D[2]);
        Corners3D[3] = mtxViewToWorld.preTransformP(Corners3D[3]);
        Corners3D[4] = mtxViewToWorld.preTransformP(Corners3D[4]);

        Planes[0].init(Corners3D[1], Corners3D[0], Corners3D[3]);
        Planes[1].init(Corners3D[4], Corners3D[0], Corners3D[3]);
        Planes[2].init(Corners3D[4], Corners3D[3], Corners3D[2]);
        Planes[3].init(Corners3D[4], Corners3D[2], Corners3D[1]);
        Planes[4].init(Corners3D[4], Corners3D[1], Corners3D[0]);
    }

    void init( xFLOAT angle, xFLOAT aspect, xFLOAT frontClip, xFLOAT backClip )
    {
        Empty     = false;
        Angle     = angle;
        Aspect    = aspect;
        FrontClip = frontClip;
        BackClip  = backClip;

        xVector3 p0, p1, p2;
        
        p0.init(0.f,0.f,0.f);
        p1.z = BackClip;
        p1.y = -p1.z * (xFLOAT)tan( Angle * 0.5f );
        p1.x = p1.y * Aspect;
        p2.init(p1.x, -p1.y, p1.z);
        LeftPlane.init(p0, p1, p2);

        p1.y = -p1.y; p2.x = -p2.x;
        TopPlane.init(p0, p1, p2);

        p1.x = -p1.x; p2.y = -p1.y;
        RightPlane.init(p0, p1, p2);

        p1.y = -p1.y; p2.x = -p1.x;
        BottomPlane.init(p0, p1, p2);
    }

    bool CheckSphere(const xVector3 &sphereCenter, xFLOAT sphereRadius) const
    {
        if (Empty || sphereRadius == 0.f) return true;

        float dist;

        xVector3 viewPos;
        viewPos.z = -(ViewTransform.z0 * sphereCenter.x
                    + ViewTransform.z1 * sphereCenter.y
                    + ViewTransform.z2 * sphereCenter.z
                    + ViewTransform.z3);
        // In front of the Front Clipping plane
        if ( viewPos.z + sphereRadius < FrontClip )
            return false;
        // Behind the Back Clipping plane
        if ( viewPos.z - sphereRadius > BackClip )
            return false;

        viewPos.x = -(ViewTransform.x0 * sphereCenter.x
                    + ViewTransform.x1 * sphereCenter.y
                    + ViewTransform.x2 * sphereCenter.z
                    + ViewTransform.x3);
        // Test the left plane
        dist = viewPos.x * LeftPlane.x + viewPos.z * LeftPlane.z;
        if ( dist > sphereRadius )
            return false;
        // Test the right plane
        dist = viewPos.x * RightPlane.x + viewPos.z * RightPlane.z;
        if ( dist > sphereRadius )
            return false;

        viewPos.y = -(ViewTransform.y0 * sphereCenter.x
                    + ViewTransform.y1 * sphereCenter.y
                    + ViewTransform.y2 * sphereCenter.z
                    + ViewTransform.y3);
        // Test the top plane
        dist = viewPos.y * TopPlane.y + viewPos.z * TopPlane.z;
        if ( dist > sphereRadius )
            return false;
        // Test the right plane
        dist = viewPos.y * BottomPlane.y + viewPos.z * BottomPlane.z;
        if ( dist > sphereRadius )
            return false;

        // Inside the field of view
        return true;
    }

    bool CheckBox(Math::Figures::xBoxA &box, const xMatrix &MX_LtoW) const
    {
        if (Empty || box.P_min == box.P_max) return true;

        const xVector3 *boundingPoints = box.fillCornersTransformed(MX_LtoW);

        xVector3 viewPos[8];
        const xVector3 *iterS;
        xVector3 *iterD;
        bool culled;
        int v;

        iterS = boundingPoints;
        iterD = viewPos;
        for (v = 8; v; --v, ++iterS, ++iterD)
            iterD->z = -(ViewTransform.z0 * iterS->x
                       + ViewTransform.z1 * iterS->y
                       + ViewTransform.z2 * iterS->z
                       + ViewTransform.z3);
        // In front of the Front Clipping plane
        iterD = viewPos; culled = true;
        for (v = 8; v && culled; --v, ++iterD)
            culled = iterD->z < FrontClip;
        if (culled) return false;
        // Behind the Back Clipping plane
        iterD = viewPos; culled = true;
        for (v = 8; v && culled; --v, ++iterD)
            culled = iterD->z > BackClip;
        if (culled) return false;

        iterS = boundingPoints;
        iterD = viewPos;
        for (v = 8; v; --v, ++iterS, ++iterD)
            iterD->x = -(ViewTransform.x0 * iterS->x
                       + ViewTransform.x1 * iterS->y
                       + ViewTransform.x2 * iterS->z
                       + ViewTransform.x3);
        // Test the left plane
        iterD = viewPos; culled = true;
        for (v = 8; v && culled; --v, ++iterD)
            culled = iterD->x * LeftPlane.x + iterD->z * LeftPlane.z > 0;
        if (culled) return false;
        // Test the right plane
        iterD = viewPos; culled = true;
        for (v = 8; v && culled; --v, ++iterD)
            culled = iterD->x * RightPlane.x + iterD->z * RightPlane.z > 0;
        if (culled) return false;

        iterS = boundingPoints;
        iterD = viewPos;
        for (v = 8; v; --v, ++iterS, ++iterD)
            iterD->x = -(ViewTransform.y0 * iterS->x
                       + ViewTransform.y1 * iterS->y
                       + ViewTransform.y2 * iterS->z
                       + ViewTransform.y3);
        // Test the top plane
        iterD = viewPos; culled = true;
        for (v = 8; v && culled; --v, ++iterD)
            culled = iterD->y * TopPlane.x + iterD->z * TopPlane.z > 0;
        if (culled) return false;
        // Test the bottom plane
        iterD = viewPos; culled = true;
        for (v = 8; v && culled; --v, ++iterD)
            culled = iterD->y * BottomPlane.x + iterD->z * BottomPlane.z > 0;
        if (culled) return false;

        // None of the planes could cull this box
        return true;
    }
    bool CheckPoints(const xVector4 *points, xWORD count) const
    {
        if (Empty) return true;
        if (!count) return false;

        for (int i = 0; i < 5; ++i)
        {
            const xPlane   &plane = Planes[i];
            const xVector4 *iter  = points;
            bool  culled = true;
            for (int j = count; j && culled; --j, ++iter)
                culled = xPlane::DotProduct(*iter, plane) < 0.f;
            if (culled) return false;
        }
        // None of the planes could cull this box
        return true;
    }
};

#endif
