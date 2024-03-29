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
	class Tutorial9 : public TutorialRenderer {
	public:
		Tutorial9();
		~Tutorial9();

		void RenderFrame() override;
		void Update(float dt);

	protected:
		UniqueOGLShader	shader;
		SharedOGLMesh	mesh;
		SharedMeshAnim	anim;

		float	frameTime;
		int		currentFrame;
	
		StructuredOGLBuffer<Matrix4> joints;
	};
}