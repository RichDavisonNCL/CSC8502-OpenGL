/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial13.h"
#include "Heightmap.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

Tutorial13::Tutorial13() : TutorialRenderer() {
	groundShader	= new OGLShader("ground.vert", "ground.frag");
	skyboxShader	= new OGLShader("skybox.vert", "skybox.frag");
	reflectShader	= new OGLShader("reflection.vert", "reflection.frag");

	albedo = (OGLTexture*)OGLTexture::RGBATextureFromFilename("Barren Reds.JPG");
	skybox = (OGLTexture*)OGLTexture::LoadCubemap(
		"/Cubemap/skyrender0004.png",
		"/Cubemap/skyrender0001.png",
		"/Cubemap/skyrender0003.png",
		"/Cubemap/skyrender0006.png",
		"/Cubemap/skyrender0002.png",
		"/Cubemap/skyrender0005.png"
	);

	heightmap = new OGLMesh();
	Heightmap::CreateHeightmap("noise.png", heightmap, { 10,1.0f,10 }, { 32, 32 });
	heightmap->UploadToGPU(this);
}

Tutorial13::~Tutorial13() {
	delete groundShader;
	delete skyboxShader;
	delete reflectShader;
	delete albedo;
	delete skybox;
	delete heightmap;
}

void Tutorial13::RenderFrame() {
	glDisable(GL_DEPTH_TEST);	//No depth test
	glDepthMask(GL_FALSE);		//No depth writes
	UseShader(skyboxShader);
	SetCameraUniforms(*defaultCamera, 0);
	BindTextureToPipeline(skybox->GetObjectID(), "cubeTex", 0, GL_TEXTURE_CUBE_MAP);
	BindMesh(quadMesh);
	DrawBoundMesh();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	UseShader(groundShader);
	SetCameraUniforms(*defaultCamera, 0);
	BindTextureToPipeline(albedo->GetObjectID(), "albedoTex", 0);
	SetUniform("modelMatrix", Matrix4());
	BindMesh(heightmap);
	DrawBoundMesh();

	UseShader(reflectShader);
	SetCameraUniforms(*defaultCamera, 0);
	BindTextureToPipeline(skybox->GetObjectID(), "cubeTex", 0, GL_TEXTURE_CUBE_MAP);
	SetUniform("modelMatrix", Matrix4::Translation({0, 10, 0}) * Matrix4::Scale({ 4,4,4 }));
	BindMesh(sphereMesh);
	DrawBoundMesh();
}