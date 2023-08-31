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
	class Tutorial12 : public TutorialRenderer {
	public:
		Tutorial12();
		~Tutorial12();

		void RenderFrame() override;

	protected:
		UniqueOGLShader shader;
		UniqueOGLMesh	heightmap;
		UniqueOGLTexture albedo;
		UniqueOGLTexture bump;
		StructuredOGLBuffer<Light> light;
	};
}