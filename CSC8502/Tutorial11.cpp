/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial11.h"
#include "Heightmap.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

Tutorial11::Tutorial11() : TutorialRenderer() {
	shader = std::make_unique<OGLShader>("lighting.vert", "lighting.frag");

	heightmap = std::make_unique<OGLMesh>();
	Heightmap::CreateHeightmap("noise.png", *heightmap, { 10,1.0f,10 }, { 32, 32 });
	CalculateNormals(*heightmap);
	CalculateNormals(*heightmap);
	heightmap->UploadToGPU(this);

	albedo = OGLTexture::TextureFromFile("Barren Reds.JPG");

	light.elements[0].position	= Vector3(50, 50, 50);
	light.elements[0].intensity	= Vector3(100.0f, 100.0f, 100.0f) * 100.0f;
	light.elements[0].radius		= 1000.0f;

	light.GPUSync();

	glEnable(GL_DEPTH_TEST);
}

Tutorial11::~Tutorial11() {
}

void Tutorial11::RenderFrame() {
	UseShader(*shader);
	SetCameraUniforms(*defaultCamera, 0);
	BindBufferAsUBO(light, 0);
	BindTextureToPipeline(albedo->GetObjectID(), "albedoTex", 0);
	SetUniform("modelMatrix", Matrix4());
	BindMesh(*heightmap);
	DrawBoundMesh();
}