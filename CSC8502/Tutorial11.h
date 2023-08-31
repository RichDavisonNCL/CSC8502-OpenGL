/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TutorialRenderer.h"


namespace NCL::CSC8502 {
	#include "./Shaders/ShaderInterop.h"
	#include "./Shaders/LightStruct.h"

	using namespace Rendering;
	class Tutorial11 : public TutorialRenderer {
	public:
		Tutorial11();
		~Tutorial11();

		void RenderFrame() override;

	protected:
		UniqueOGLShader		shader;
		UniqueOGLMesh		heightmap;
		UniqueOGLTexture	albedo;
		StructuredOGLBuffer<Light> light;
	};
}