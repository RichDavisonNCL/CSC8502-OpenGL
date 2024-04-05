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
	shader = std::make_unique<OGLShader>("skeletal.vert", "skeletal.frag");

	GLTFLoader::Load("CesiumMan/CesiumMan.gltf",
		scene,
		[](void) ->  Mesh* {
			return new OGLMesh();
		},
		[](std::string& input) -> OGLTexture* { return nullptr; }
	);


	mesh = std::dynamic_pointer_cast<OGLMesh>(scene.meshes[0]);

	mesh->UploadToGPU(this);

	anim = scene.animations[0];

	joints = StructuredOGLBuffer<Matrix4>(mesh->GetJointCount());

	frameTime	 = 0.0f;
	currentFrame = 0;
}

Tutorial9::~Tutorial9() {
}

void Tutorial9::RenderFrame() {
	UseShader(*shader);
	BindTextureToPipeline(((OGLTexture*)&(*scene.textures[0]))->GetObjectID(), "albedoTex", 0);

	BindBufferAsSSBO(joints, 0);
	SetCameraUniforms(*defaultCamera, 0);
	SetUniform("modelMatrix", Matrix4());

	BindMesh(*mesh);
	DrawBoundMesh();
}

void Tutorial9::Update(float dt) {
	TutorialRenderer::Update(dt);

	frameTime -= dt;

	if (frameTime <= 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += anim->GetFrameTime();
		std::vector<Matrix4> invBindPos = mesh->GetInverseBindPose();

		const Matrix4* frameMats = anim->GetJointData(currentFrame);

		std::vector<Matrix4> jointData(invBindPos.size());

		for (int i = 0; i < invBindPos.size(); ++i) {
			joints.elements[i] = (frameMats[i] * invBindPos[i]);
		}

		joints.GPUSync();
	}
}