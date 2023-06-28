/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial5.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

Tutorial5::Tutorial5() : TutorialRenderer() {
	heightmap = new OGLMesh();
	Heightmap::CreateHeightmap("noise.png", heightmap, {10,1.0f,10}, {32, 32});
	heightmap->UploadToGPU(this);

	shader = new OGLShader("Tutorial3.vert", "Tutorial3.frag");
}

Tutorial5::~Tutorial5() {
	delete heightmap;
}

void Tutorial5::RenderFrame() {
	UseShader(shader);
	BindMesh(heightmap);
	DrawBoundMesh();
}