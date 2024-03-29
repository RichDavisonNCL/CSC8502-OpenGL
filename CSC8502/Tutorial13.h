/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TutorialRenderer.h"

namespace NCL::CSC8502 {
	using namespace Rendering;
	class Tutorial13 : public TutorialRenderer {
	public:
		Tutorial13();
		~Tutorial13();

		void RenderFrame() override;

	protected:
		UniqueOGLShader groundShader;
		UniqueOGLShader skyboxShader;
		UniqueOGLShader reflectShader;

		UniqueOGLTexture albedo;
		UniqueOGLTexture skybox;

		UniqueOGLMesh	heightmap;
	};
}