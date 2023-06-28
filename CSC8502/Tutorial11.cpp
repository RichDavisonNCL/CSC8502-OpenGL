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
	shader = new OGLShader("lighting.vert", "lighting.frag");

	heightmap = new OGLMesh();
	Heightmap::CreateHeightmap("noise.png", heightmap, { 10,1.0f,10 }, { 32, 32 });
	heightmap->UploadToGPU(this);

	albedo = (OGLTexture*)OGLTexture::RGBATextureFromFilename("Barren Reds.JPG");

	light.position	= Vector3(50, 50, 50);
	light.intensity = Vector3(100.0f, 100.0f, 100.0f) * 100;
	light.radius	= 1000.0f;

	glEnable(GL_DEPTH_TEST);
}

Tutorial11::~Tutorial11() {
	delete shader;
	delete heightmap;
	delete albedo;
}

void Tutorial11::RenderFrame() {
	UseShader(shader);
	SetCameraUniforms(*defaultCamera, 0);
	SetLightUniforms(light, 5);
	BindTextureToPipeline(albedo->GetObjectID(), "albedoTex", 0);
	SetUniform("modelMatrix", Matrix4());
	BindMesh(heightmap);
	DrawBoundMesh();
}