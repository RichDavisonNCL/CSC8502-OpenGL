/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "OGLRenderer.h"

namespace NCL::CSC8502 {
	using namespace Rendering;
	class TutorialRenderer : public OGLRenderer {
	public:
		TutorialRenderer();
		~TutorialRenderer();

		virtual void Update(float dt);

	protected:

	};
}