/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial3.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

Tutorial3::Tutorial3() : TutorialRenderer() {
	shader = std::make_unique<OGLShader>("Tutorial3.vert", "Tutorial3.frag");
}

Tutorial3::~Tutorial3() {
}

void Tutorial3::Update(float dt) {

}

void Tutorial3::RenderFrame() {
	UseShader(*shader);
	BindMesh(*quadMesh);
	DrawBoundMesh();
}