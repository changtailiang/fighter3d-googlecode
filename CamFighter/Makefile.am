
#SUBDIRS = Math Math/Cameras Math/Figures Math/Tracking Graphics Graphics/OGL Graphics/OGL/Render Graphics/Textures AppFramework AppFramework/OGL AppFramework/Input Models Models/lib3dx Physics Physics/Colliders Physics/Verlet SourceFiles Utils World Lua

bin_PROGRAMS = fighter

LDADD   = -lGLEW -lGLU -lGL -lXxf86vm -l3ds -lm -llua5.1
SOURCES = \
	./Physics/Colliders/FigureCollider.cpp \
	./Physics/Colliders/CollisionInfo.cpp \
	./Physics/Colliders/BVHCollider.cpp \
	./Physics/Verlet/VConstraintLengthEql.cpp \
	./Physics/Verlet/VConstraintLengthMin.cpp \
	./Physics/Verlet/VConstraintLengthMax.cpp \
	./Physics/Verlet/VConstraintAngular.cpp \
	./Physics/Verlet/VConstraintCollision.cpp \
	./Physics/Verlet/VConstraint.cpp \
	./Physics/Verlet/VerletSolver.cpp \
	./Physics/PhysicalWorld.cpp \
	./Physics/PhysicalBody.cpp \
	./Models/lib3dx/xIKNode.cpp \
	./Models/lib3dx/xImport.cpp \
	./Models/lib3dx/xElementCalc.cpp \
	./Models/lib3dx/xAction.cpp \
	./Models/lib3dx/xAnimation.cpp \
	./Models/lib3dx/xBone.cpp \
	./Models/lib3dx/xElementData.cpp \
	./Models/lib3dx/xSkeleton.cpp \
	./Models/lib3dx/xAnimationMgr.cpp \
	./Models/lib3dx/xElement.cpp \
	./Models/lib3dx/xElementRunTime.cpp \
	./Models/lib3dx/xMaterial.cpp \
	./Models/lib3dx/xModel.cpp \
	./Models/lib3dx/xUtils.cpp \
	./Models/Model3dx.cpp \
	./Models/ModelMgr.cpp \
	./World/SkeletizedObj.cpp \
	./World/World.cpp \
	./World/RigidObj.cpp \
	./Math/Tracking/ObjectTracker.cpp \
	./Math/Figures/xBoxO.cpp \
	./Math/Figures/xCylinder.cpp \
	./Math/Figures/xMesh.cpp \
	./Math/Figures/xIFigure3d.cpp \
	./Math/Cameras/CameraHuman.cpp \
	./Math/Cameras/FieldOfView.cpp \
	./Math/Cameras/CameraSet.cpp \
	./Math/Cameras/Camera.cpp \
	./Math/Cameras/CameraFree.cpp \
	./Math/xLight.cpp \
	./Math/xMatrix.cpp \
	./Math/xQuaternion.cpp \
	./Graphics/OGL/Render/RendererGLSkeleton.cpp \
	./Graphics/OGL/Render/RendererGLDepth.cpp \
	./Graphics/OGL/Render/RendererGLDiffuse.cpp \
	./Graphics/OGL/Render/RendererGLShadowV.cpp \
	./Graphics/OGL/Render/RendererGL.cpp \
	./Graphics/OGL/Render/RendererGLAmbient.cpp \
	./Graphics/OGL/Render/RendererGLBVH.cpp \
	./Graphics/OGL/Render/RendererGLShadowM.cpp \
	./Graphics/OGL/UtilsOGL.cpp \
	./Graphics/OGL/TextureOGL.cpp \
	./Graphics/OGL/Shader.cpp \
	./Graphics/OGL/FontOGL.cpp \
	./Graphics/OGL/AnimSkeletal.cpp \
	./Graphics/OGL/ISelectionProvider.cpp \
	./Graphics/OGL/WorldRenderGL.cpp \
	./Graphics/Textures/BMPFile.cpp \
	./Graphics/Textures/TextureMgr.cpp \
	./Graphics/Textures/TGAFile.cpp \
	./Graphics/Renderer.cpp \
	./Graphics/FontMgr.cpp \
	./Graphics/ShadowVolume.cpp \
	./Graphics/D3D/UtilsD3D.cpp \
	./Graphics/D3D/TextureD3D.cpp \
	./Graphics/D3D/FontD3D.cpp \
	./Graphics/D3D/d3d.cpp \
	./Utils/Utils.cpp \
	./Utils/Profiler.cpp \
	./Utils/Filesystem.cpp \
	./Utils/Debug.cpp \
	./Config.cpp \
	./Lua/LuaScript.cpp \
	./Lua/LuaVirtualMachine.cpp \
	./Lua/LuaDebugger.cpp \
	./SourceFiles/SceneSkeleton.cpp \
	./SourceFiles/SceneSkeletonUpd.cpp \
	./SourceFiles/SceneConsole.cpp \
	./SourceFiles/SceneMenu.cpp \
	./SourceFiles/main.cpp \
	./SourceFiles/ComBoard.cpp \
	./SourceFiles/SceneGame.cpp \
	./SourceFiles/SceneTest.cpp \
	./AppFramework/Input/InputMgr.cpp \
	./AppFramework/OGL/GLWindowL.cpp \
	./AppFramework/OGL/GLWindow.cpp \
	./AppFramework/Application.cpp \
	./AppFramework/IWindow.cpp \
	./AppFramework/WindowProcL.cpp \
	./AppFramework/IScene.cpp \
	./AppFramework/D3D/DXWindow.cpp \
	./AppFramework/WindowProc.cpp

fighter_SOURCES  = $(SOURCES)
fighter_LDADD    = $(LDADD)
fighter_CPPFLAGS = -Wall
