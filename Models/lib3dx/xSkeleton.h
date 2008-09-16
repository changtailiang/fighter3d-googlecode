#ifndef __incl_lib3dx_xSkeleton_h
#define __incl_lib3dx_xSkeleton_h

#include "../../Math/xMath.h"
#include "../../Physics/Verlet/VConstraintLengthEql.h"
#include "xBone.h"

struct xSkeleton
{
    xBone *L_bones;
    xBYTE  I_bones;

    VConstraintLengthEql  *C_boneLength;
    VConstraint          **C_constraints;
    xBYTE                  I_constraints;

    void      Clear();
    xSkeleton Clone() const;
    void      ResetQ();

    void      CalcQuats(const xPoint3 *P_current, const xQuaternion *QT_boneSkew,
                        xBYTE ID_bone, xMatrix MX_parent_Inv);
    void      CalcQuats(const xPoint3 *P_current, const xQuaternion *QT_boneSkew,
                        const bool *FL_calculate,
                        xBYTE ID_bone, xMatrix MX_parent_Inv);
    void      FillBoneConstraints();
    xBone    *BoneAdd(xBYTE ID_parent, xPoint3 P_end);

    void QuatsToArray  (xQuaternion *QT_array) const;
    void QuatsFromArray(const xQuaternion *QT_array);

    void Load( FILE *file );
    void Save( FILE *file ) const;
};

struct xModelInstance;

void    xBoneCalculateMatrices (const xSkeleton &spine, xModelInstance &instance);
void    xBoneCalculateQuats    (const xSkeleton &spine, xModelInstance &instance);

void    xBoneCalculateQuatForVerlet(const xSkeleton &spine, xBYTE ID_last,
                                    xQuaternion &QT_parent, xQuaternion &QT_current);

#endif
