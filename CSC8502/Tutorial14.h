/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TutorialRenderer.h"
#include "Light.h"

namespace NCL::CSC8502 {
	using namespace Rendering;
	class Tutorial14 : public TutorialRenderer {
	public:
		Tutorial14();
		~Tutorial14();

		void RenderFrame() override;

	protected:
		void ShadowFillPass();
		void SceneDrawPass();

		void RenderSceneObjects();

		OGLShader* fillShader;
		OGLShader* drawShader;

		Matrix4 shadowMatrix;

		Light light;

		GLuint shadowFBO;
		GLuint shadowMap;

		OGLTexture* brickAlbedo;
	};
}