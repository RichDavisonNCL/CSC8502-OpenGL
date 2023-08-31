/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial12.h"
#include "Heightmap.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

Tutorial12::Tutorial12() : TutorialRenderer() {
	shader = std::make_unique<OGLShader>("BumpLighting.vert", "BumpLighting.frag");

	heightmap = std::make_unique<OGLMesh>();
	Heightmap::CreateHeightmap("noise.png", *heightmap, { 1024,32.0f,1024 }, { 32, 32 });
	CalculateNormalsTangents(*heightmap); //A new line!
	heightmap->UploadToGPU(this);

	albedo	= OGLTexture::TextureFromFile("Barren Reds.JPG");
	bump	= OGLTexture::TextureFromFile("Barren RedsDOT3.JPG");

	light.elements[0].position	= Vector3(50, 50, 50);
	light.elements[0].intensity = Vector3(100.0f, 100.0f, 100.0f) * 100;
	light.elements[0].radius	= 1000.0f;

	light.GPUSync();

	glEnable(GL_DEPTH_TEST);
}

Tutorial12::~Tutorial12() {
}

void Tutorial12::RenderFrame() {
	UseShader(*shader);
	SetCameraUniforms(*defaultCamera, 0);
	BindBufferAsUBO(light, 0);
	BindTextureToPipeline(albedo->GetObjectID(), "albedoTex", 0);
	BindTextureToPipeline(bump->GetObjectID(), "bumpTex", 1);
	SetUniform("modelMatrix", Matrix4());
	BindMesh(*heightmap);
	DrawBoundMesh();
}