class xQuaternion : public xVector4
{
public:
    static xQuaternion Create(xFLOAT X, xFLOAT Y, xFLOAT Z, xFLOAT W) { xQuaternion res; return res.init(X,Y,Z,W); }
    static xQuaternion Create(const xVector3 &src, xFLOAT W)          { xQuaternion res; return res.init(src,W); }
    static xQuaternion Create(const xVector3 &src)                    { xQuaternion res; return res.init(src); }
    xQuaternion       &init  (xFLOAT X, xFLOAT Y, xFLOAT Z, xFLOAT W) { x = X; y = Y; z = Z; w = W; return *this; }
    xQuaternion       &init  (const xVector3 &src, xFLOAT W)          { vector3 = src; w = W;       return *this; }
    xQuaternion       &init  (const xVector3 &src)                    { vector3 = src; w = 0.f;     return *this; }
    xQuaternion       &init  (const xFLOAT   *src)                    { memcpy(this, src, 4*sizeof(xFLOAT)); return *this; }
    xQuaternion       &zeroQ ()                                       { x = y = z = 0.f; w = 1.f;   return *this; }

    xQuaternion &operator = (const xVector4 &source) {
        memcpy(this, &source, sizeof(xVector4));
        return *this;
    }

    static xQuaternion GetRotation     (const xPoint3 &srcP, const xPoint3 &dstP);
    static xQuaternion GetRotation     (const xPoint3 &srcP, const xPoint3 &dstP, const xPoint3 &center)
    { return GetRotation(srcP-center, dstP-center); }

    xVector3 rotate(const xVector3 &p) const;
    xPoint3  rotate(const xPoint3 &p, const xPoint3 &center) const
    { return rotate(p - center) + center; }
    xVector3 unrotate(const xVector3 &p) const;
    xPoint3  unrotate(const xPoint3 &p, const xPoint3 &center) const
    { return unrotate(p - center) + center; }

           xVector3    angularVelocity() const;
    static xQuaternion AngularVelocity(const xVector3 &omega);

    static xQuaternion Product         (const xQuaternion &a, const xQuaternion &b);

           xQuaternion &interpolate     (xFLOAT weight);
           xQuaternion &interpolateFull (xFLOAT weight);
    static xQuaternion  Interpolate     (const xQuaternion &q, xFLOAT weight)
    {
        xQuaternion ret = q;
        return ret.interpolate(weight);
    }
    static xQuaternion  InterpolateFull (const xQuaternion &q, xFLOAT weight)
    {
        xQuaternion ret = q;
        return ret.interpolateFull(weight);
    }

    static xQuaternion Exp             (const xQuaternion &q);
    static xQuaternion Log             (const xQuaternion &q);

    static xQuaternion Lerp            (const xQuaternion &q1,const xQuaternion &q2,float t);
    static xQuaternion SLerp           (const xQuaternion &q1,const xQuaternion &q2,float t);
    static xQuaternion SLerpNoInvert   (const xQuaternion &q1,const xQuaternion &q2,float t);
    static xQuaternion SQuad           (const xQuaternion &q1,const xQuaternion &q2,const xQuaternion &a,const xQuaternion &b,float t);
    static xQuaternion SQuad           (const xQuaternion &q1,const xQuaternion &q2,const xQuaternion &a,float t);
    static xQuaternion Spline          (const xQuaternion &q1,const xQuaternion &q2,const xQuaternion &q3);
};

inline xQuaternion &xQuaternionCast(xVector4 &QT)
{ return *(xQuaternion*)&QT; }

inline const xQuaternion &xQuaternionCast(const xVector4 &QT)
{ return *(xQuaternion*)&QT; }
