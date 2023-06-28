/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial1.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

Tutorial1::Tutorial1() : TutorialRenderer(){
	shader = new OGLShader("Tutorial1.vert", "Tutorial1.frag");
}

Tutorial1::~Tutorial1() {
	delete shader;
}

void Tutorial1::RenderFrame() {
	UseShader(shader);
	BindMesh(triMesh);
	DrawBoundMesh();
}