/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "../NCLCoreClasses/Window.h"

#include "FirstTriangle.h"
#include "Uniforms.h"
#include "ModelMatrix.h"
#include "ProjectionMatrix.h"
#include "ViewMatrix.h"
#include "DepthBuffer.h"
#include "StencilBuffer.h"

#include "IndexBuffers.h"
#include "Instancing.h"
#include "BuffersInterop.h"

#include "SceneManagement.h"
#include "MoreSceneManagement.h"
#include "FrustumCulling.h"

#include "Texturing.h"
#include "Transparency.h"
#include "SceneManagement.h"

#include "SkeletalAnimation.h"

#include "CubeMapping.h"
#include "ShadowMapping.h"
#include "DeferredRendering.h"



#include "Vector.h"
#include "Matrix.h"

using namespace NCL;
using namespace CSC8502;

int main() {
	NCL::WindowInitialisation winInit = {
		.width = 1120,
		.height = 768,
		.windowTitle = "Welcome to OpenGL!",
	};

	Window* w = Window::CreateGameWindow(winInit);

	if (!w->HasInitialised()) {
		return -1;
	}

	//Week 1
	//auto* r = new FirstTriangle();
	//auto* r = new Uniforms();
	//auto* r = new ModelMatrix();
	//auto* r = new ProjectionMatrix();
	//auto* r = new ViewMatrix();
	//auto* r = new DepthBuffer();
	//auto* r = new StencilBuffer();
	// 
	//Week 2
	//auto* r = new Texturing();
	//auto* r = new Transparency();
	//auto* r = new IndexBuffers();
	//auto* r = new Instancing();
	//auto* r = new BuffersInterop();
	//auto* r = new SceneManagement();
	//auto* r = new MoreSceneManagement();
	//auto* r = new FrustumCulling();
	auto* r = new SkeletalAnimation();
	//Week 3
	//auto* r = new BasicLighting();
	//auto* r = new BumpMapping();
	//auto* r = new CubeMapping();
	//auto* r = new ShadowMapping();
	//auto* r = new DeferredRendering();
	// 
	//Extras

	w->SetConsolePosition(2000, 200);

	w->LockMouseToWindow(true);
	w->ShowOSPointer(false);

	WindowEventHandler e = [&](WindowEvent e, uint32_t w, uint32_t h) {
		r->WindowEventHandler(e, w, h);
	};

	w->SetWindowEventHandler(e);

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
		float time = w->GetTimer().GetTimeDeltaSeconds();
		r->Update(time);
		r->Render();
	}

	Window::DestroyGameWindow();
	return 0;
}