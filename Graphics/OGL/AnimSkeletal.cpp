#include "AnimSkeletal.h"

#include "Extensions/ARB_vertex_buffer_object.h"
#include "../../Models/lib3dx/xSkeleton.h"

using namespace Graphics::OGL;

void AnimSkeletal::BeginAnimation()
{
    currSkeletalShader = (ShaderSkeletal*)Shader::currShader;

    if (Shader::SkeletonState() != xState_On)
        currSkeletalShader = 0;
    aBoneIdxWghts = -1;

    m_notForceCPU = !m_forceCPU && Config::EnableShaders && currSkeletalShader && currSkeletalShader->currProgram;
    sft_vertices  = NULL;
    sft_normals   = NULL;
}

void AnimSkeletal::EndAnimation()
{
    if (m_notForceCPU && currSkeletalShader->currProgram)
    {
        if (aBoneIdxWghts > -1)
            glDisableVertexAttribArrayARB(aBoneIdxWghts);
        //Shader::Suspend();
    }
    if (sft_vertices)
    {
        delete[] sft_vertices;
        sft_vertices = NULL;
    }
    if (sft_normals)
    {
        delete[] sft_normals;
        sft_normals = NULL;
    }
}

void AnimSkeletal::SetBones(GLsizei noOfBones,
                              const xMatrix *bonesM,
                              const xQuaternion *bonesQ,
                              const xPoint3     *bonesR,
                              const xPoint3     *bonesT,
                              const xElement *element, bool VBO)
{
    currSkeletalShader = (ShaderSkeletal*)Shader::currShader;
    if (Shader::SkeletonState() != xState_On)
        currSkeletalShader = 0;
    m_notForceCPU = !m_forceCPU && Config::EnableShaders && currSkeletalShader && currSkeletalShader->currProgram;

    if (m_notForceCPU) {
        glUniform4fvARB(currSkeletalShader->uQuats, noOfBones, (GLfloat *)bonesQ);
        glUniform3fvARB(currSkeletalShader->uRoots, noOfBones, (GLfloat *)bonesR);
        glUniform3fvARB(currSkeletalShader->uTrans, noOfBones, (GLfloat *)bonesT);

        if (element)
        {
            glEnableVertexAttribArrayARB(currSkeletalShader->aBoneIdxWghts);
            aBoneIdxWghts = currSkeletalShader->aBoneIdxWghts;
            xDWORD stride = element->FL_textured ? sizeof(xVertexTexSkel) : sizeof(xVertexSkel);
            if (VBO)
                glVertexAttribPointerARB (currSkeletalShader->aBoneIdxWghts, 4, GL_FLOAT, GL_FALSE, stride, (void *)(3*sizeof(xFLOAT)));
            else
                glVertexAttribPointerARB (currSkeletalShader->aBoneIdxWghts, 4, GL_FLOAT, GL_FALSE,
                    stride, element->renderData.L_verticesS->bone);
        }
    }
    else {
        sft_bonesM    = bonesM;
        sft_bonesQ    = bonesQ;
        sft_bonesR    = bonesR;
        sft_bonesT    = bonesT;
        sft_boneCount = noOfBones;
    }
}

void AnimSkeletal::SetElement(const xElement *element, const xElementInstance *instance, bool VBO)
{
    if (m_notForceCPU) {
        xDWORD stride = element->GetVertexStride();
        if (VBO) {
            glBindBufferARB ( GL_ARRAY_BUFFER_ARB, element->renderData.gpuMain.vertexB );
            glVertexPointer (3, GL_FLOAT, stride, 0);
            if (State::RenderingShadows) glTexCoordPointer (3, GL_FLOAT, stride, 0);

            /************************* LOAD NORMALS ****************************/
            if ((Shader::NeedNormals()) && element->renderData.gpuMain.normalB) {
                glBindBufferARB ( GL_ARRAY_BUFFER_ARB, element->renderData.gpuMain.normalB );
                glNormalPointer ( GL_FLOAT, sizeof(xVector3), 0 );
                glEnableClientState(GL_NORMAL_ARRAY);
                glBindBufferARB ( GL_ARRAY_BUFFER_ARB, element->renderData.gpuMain.vertexB );
            }
        }
        else {
            glVertexPointer (3, GL_FLOAT, stride, element->renderData.L_vertices);
            if (State::RenderingShadows) glTexCoordPointer (3, GL_FLOAT, stride, element->renderData.L_vertices);

            /************************* LOAD NORMALS ****************************/
            if (element->renderData.L_normals) {
                glNormalPointer ( GL_FLOAT, sizeof(xVector3), element->renderData.L_normals );
                glEnableClientState(GL_NORMAL_ARRAY);
            }
        }
    }
    else {
        if (VBO) glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
        if (instance->L_vertices)
        {
            glVertexPointer (3, GL_FLOAT, sizeof(xVector4), instance->L_vertices);
            if (State::RenderingShadows) glTexCoordPointer (3, GL_FLOAT, sizeof(xVector4), instance->L_vertices);
            /************************* LOAD NORMALS ****************************/
            if ((VBO && Shader::NeedNormals()) ||
                (!VBO && !State::RenderingSelection))
            {
                glNormalPointer (GL_FLOAT, sizeof(xVector3), instance->L_normals);
                glEnableClientState(GL_NORMAL_ARRAY);
            }
        }
        else
        {
            xDWORD stride = element->GetVertexStride();
            glVertexPointer (3, GL_FLOAT, stride, element->renderData.L_vertices);
            if (State::RenderingShadows) glTexCoordPointer (3, GL_FLOAT, stride, element->renderData.L_vertices);
            /************************* LOAD NORMALS ****************************/
            if (element->renderData.L_normals) {
                glNormalPointer ( GL_FLOAT, sizeof(xVector3), element->renderData.L_normals );
                glEnableClientState(GL_NORMAL_ARRAY);
            }
        }
        /*
        xSkinnedData sData = element->GetSkinnedVertices(sft_bonesQ, sft_bonesR, sft_bonesT);
        sft_vertices = sData.L_vertices;
        sft_normals  = sData.L_normals;
        if (VBO) glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
        glVertexPointer (3, GL_FLOAT, sizeof(xVector3), sft_vertices);
        if (State::RenderingShadows) glTexCoordPointer (3, GL_FLOAT, sizeof(xVector3), sft_vertices);
        if ((VBO && Shader::NeedNormals()) ||
            (!VBO && !State::RenderingSelection))
        {
            glNormalPointer (GL_FLOAT, sizeof(xVector3), sft_normals);
            glEnableClientState(GL_NORMAL_ARRAY);
        }
        */
    }
}

/**************** generic implementation ******************/

void AnimSkeletal::SetBoneIdxWghts(GLsizei stride, const GLfloat *pointer)
{
    if (m_notForceCPU) {
        glEnableVertexAttribArrayARB(currSkeletalShader->aBoneIdxWghts);
        glVertexAttribPointerARB(currSkeletalShader->aBoneIdxWghts, 4, GL_FLOAT, GL_FALSE, stride, pointer);
    }
    else {
        sft_boneIdxWghts  = pointer;
        sft_boneIdxStride = stride;
        sft_boneIdxArray  = true;
    }
}

void AnimSkeletal::SetVertices(GLsizei stride, const GLfloat *pointer, int count)
{
    if (m_notForceCPU)
        glVertexPointer   (3, GL_FLOAT, stride, pointer);
    else {
        xBYTE    *src = (xBYTE *) pointer;
        xVector3 *dst = new xVector3[count];
        xVector3 *itr = dst;
            
        if (sft_boneIdxArray) {
            const GLfloat *wghts = sft_boneIdxWghts;
            GLsizei boneStride = sft_boneIdxStride - 4*sizeof(GLfloat);

            memset(dst, 0, count*sizeof(xVector4));
            for (int i = 0; i<count; ++i, ++itr, src += stride)
            {
                xVector4 vec; vec.init(*((xVector3 *)src), 1.f);
                for (int j=0; j<4; ++j, ++wghts)
                    (*itr) += (sft_bonesM[(int)floor(*wghts)]
                              * vec * 10*(*wghts - floor(*wghts))).vector3;
                wghts = (GLfloat *)((xBYTE *) wghts + boneStride);
            }
        }
        else {
            int   i1 = (int) floor(sft_boneIdxWghts[0]);
            float w1 = (sft_boneIdxWghts[0] - i1)*10;
            int   i2 = (int) floor(sft_boneIdxWghts[1]);
            float w2 = (sft_boneIdxWghts[1] - i2)*10;
            int   i3 = (int) floor(sft_boneIdxWghts[2]);
            float w3 = (sft_boneIdxWghts[2] - i3)*10;
            int   i4 = (int) floor(sft_boneIdxWghts[3]);
            float w4 = (sft_boneIdxWghts[3] - i4)*10;

            for (int i = 0; i<count; ++i, ++itr, src += stride)
            {
                xVector4 vec; vec.init(*((xVector3 *)src), 1.f);
                *itr  = (sft_bonesM[i1] * vec * w1).vector3;
                *itr += (sft_bonesM[i2] * vec * w2).vector3;
                *itr += (sft_bonesM[i3] * vec * w3).vector3;
                *itr += (sft_bonesM[i4] * vec * w4).vector3;
            }
        }

        glVertexPointer (3, GL_FLOAT, sizeof(xVector3), dst);
        sft_vertices = dst;
    }
}

void AnimSkeletal::SetBoneIdxWghts(const GLfloat pointer[4])
{
    if (m_notForceCPU)
        glVertexAttrib4fv(currSkeletalShader->aBoneIdxWghts, pointer);
    else
    {
        sft_boneIdxWghts  = pointer;
        sft_boneIdxStride = 0;
        sft_boneIdxArray  = false;
    }
}

void AnimSkeletal::SetVertex(const GLfloat vertex[3])
{
    if (m_notForceCPU)
        glVertex3fv(vertex);
    else
    {
        int   i1 = (int) floor(sft_boneIdxWghts[0]);
        float w1 = (sft_boneIdxWghts[0] - i1)*10;
        int   i2 = (int) floor(sft_boneIdxWghts[1]);
        float w2 = (sft_boneIdxWghts[1] - i2)*10;
        int   i3 = (int) floor(sft_boneIdxWghts[2]);
        float w3 = (sft_boneIdxWghts[2] - i3)*10;
        int   i4 = (int) floor(sft_boneIdxWghts[3]);
        float w4 = (sft_boneIdxWghts[3] - i4)*10;

        xVector4 vec; vec.init(*((xVector3 *)vertex), 1.f);
        xVector4 res = sft_bonesM[i1] * vec * w1;
        res += sft_bonesM[i2] * vec * w2;
        res += sft_bonesM[i3] * vec * w3;
        res += sft_bonesM[i4] * vec * w4;

        glVertex3fv(res.vector3.xyz);
    }
}
