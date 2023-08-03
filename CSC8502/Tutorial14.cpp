/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial14.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

const int SHADOW_WIDTH = 2048;
const int SHADOW_HEIGHT = 2048;


namespace NCL::Rendering {
#include "./Shaders/ShaderInterop.h"
#include "./Shaders/LightStruct.h"
}


Light light;

Tutorial14::Tutorial14() : TutorialRenderer() {
	fillShader = new OGLShader("shadowFill.vert", "shadowFill.frag");
	drawShader = new OGLShader("shadowDraw.vert", "shadowDraw.frag");

	brickAlbedo = (OGLTexture*)OGLTexture::RGBATextureFromFilename("Barren Reds.JPG");

	shadowMap = CreateTexture(SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT24);

	glCreateFramebuffers(1, &shadowFBO);

	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

	}

	defaultCamera->SetPitch(-30.0f)
		.SetYaw(90.0f)
		.SetPosition({80,50,0});

	light.position	= Vector3(50, 50, 50);
	light.intensity = Vector3(100.0f, 100.0f, 100.0f) * 100;
	light.radius	= 1000.0f;
}

Tutorial14::~Tutorial14() {
	delete fillShader;
	delete drawShader;
	delete brickAlbedo;

	glDeleteFramebuffers(1, &shadowFBO);
	glDeleteTextures(1, &shadowMap);
}

void Tutorial14::RenderFrame() {
	ShadowFillPass();
	SceneDrawPass();
}

void Tutorial14::ShadowFillPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	UseShader(fillShader);

	Matrix4 projMatrix = Matrix4::Perspective(10.0f, 200.0f, 1.0f, 45.0f);
	Matrix4 viewMatrix = Matrix4::BuildViewMatrix(light.position, Vector3(), Vector3(0, 1, 0));

	//Matrix4 biasMatrix = Matrix4::Translation({ 0.5, 0.5, 0.5 }) * Matrix4::Scale({0.5, 0.5, 0.5});

	shadowMatrix = projMatrix * viewMatrix;

	SetUniform("projMatrix", projMatrix);
	SetUniform("viewMatrix", viewMatrix);

	RenderSceneObjects();
}

void Tutorial14::SceneDrawPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowSize.x, windowSize.y);

	UseShader(drawShader);
	SetCameraUniforms(*defaultCamera, 0);
	SetLightUniforms(light, 5);
	SetUniform("shadowMatrix", shadowMatrix);	
	BindTextureToPipeline(brickAlbedo->GetObjectID(), "albedoTex", 0);
	BindTextureToPipeline(shadowMap, "shadowTex", 2);

	RenderSceneObjects();
}

void Tutorial14::RenderSceneObjects() {
	SetUniform("modelMatrix", Matrix4::Rotation(-90.0f, {1,0,0}) * Matrix4::Scale({25,25,25}));
	BindMesh(quadMesh);
	DrawBoundMesh();

	SetUniform("modelMatrix", Matrix4::Translation({0, sin(totalRunTime) * 5, 0}));
	BindMesh(sphereMesh);
	DrawBoundMesh();
}