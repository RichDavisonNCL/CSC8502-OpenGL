/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial9.h"
#include "MeshAnimation.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

Tutorial9::Tutorial9() : TutorialRenderer()	{
	shader = new OGLShader("skeletal.vert", "skeletal.frag");

	gltf.Load("CesiumMan/CesiumMan.gltf");

	mesh = (OGLMesh*)gltf.outMeshes[0];
	mesh->UploadToGPU(this);

	anim = gltf.outAnims[0];

	joints = new StructuredOGLBuffer<Matrix4>(mesh->GetJointCount());

	frameTime	 = 0.0f;
	currentFrame = 0;
}

Tutorial9::~Tutorial9() {
	delete shader;
	delete joints;

}

void Tutorial9::RenderFrame() {
	UseShader(shader);
	BindTextureToPipeline(((OGLTexture*)(gltf.outTextures[0]))->GetObjectID(), "albedoTex", 0);

	BindBufferAsSSBO(*joints, 0);
	SetCameraUniforms(*defaultCamera, 0);
	SetUniform("modelMatrix", Matrix4());

	BindMesh(mesh);
	DrawBoundMesh();
}

void Tutorial9::Update(float dt) {
	TutorialRenderer::Update(dt);

	frameTime -= dt;

	if (frameTime <= 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += anim->GetFrameTime();
		vector<Matrix4> invBindPos = mesh->GetInverseBindPose();

		const Matrix4* frameMats = anim->GetJointData(currentFrame);

		vector<Matrix4> jointData(invBindPos.size());

		for (int i = 0; i < invBindPos.size(); ++i) {
			joints->cpuData[i] = (frameMats[i] * invBindPos[i]);
		}

		joints->SyncData();
	}
}