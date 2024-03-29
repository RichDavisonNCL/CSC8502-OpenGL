/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "../NCLCoreClasses/Window.h"

#include "Tutorial1.h"
#include "Tutorial2.h"
#include "Tutorial3.h"
#include "Tutorial4.h"
#include "Tutorial5.h"
#include "Tutorial6.h"
#include "Tutorial7.h"
#include "Tutorial8.h"
#include "Tutorial9.h"
#include "Tutorial10.h"
#include "Tutorial11.h"
#include "Tutorial12.h"
#include "Tutorial13.h"
#include "Tutorial14.h"
#include "Tutorial15.h"

#include "NewVector.h"
#include "NewMatrix.h"

using namespace NCL;
using namespace CSC8502;

void NewVectorTest() {
	NewVector3 a(1, 1, 1);
	NewVector3 b(2, 2, 2);

	NewVector3 c = a + b;

	NewVector4 a4(c, 1.0f);

	NewVector3 d = Maths::Cross(a, b);
	float x = Maths::Dot(a, b);

	float al = Maths::Length(a);

	NewVector3 an = Maths::Normalise(a);

	NewMatrix4 ma;
	NewMatrix4 mb;
	NewMatrix4 mc = ma * mb;
}

int main() {
	Window* w = Window::CreateGameWindow("Welcome to CSC8502!", 1120, 768);

	if (!w->HasInitialised()) {
		return -1;
	}
	NewVectorTest();
	//Week 1
	//auto* r = new Tutorial1();
	//auto* r = new Tutorial2();
	//auto* r = new Tutorial3();
	//auto* r = new Tutorial4();
	//auto* r = new Tutorial5();
	// 
	//Week 2
	//auto* r = new Tutorial6();
	//auto* r = new Tutorial7();
	//auto* r = new Tutorial8();
	//auto* r = new Tutorial9();
	//auto* r = new Tutorial10();
	//Week 3
	//auto* r = new Tutorial11();
	auto* r = new Tutorial12();
	//auto* r = new Tutorial13();
	//auto* r = new Tutorial14();
	//auto* r = new Tutorial15();
	// 
	//Extras

	w->SetConsolePosition(2000, 200);

	w->LockMouseToWindow(true);
	w->ShowOSPointer(false);

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
		float time = w->GetTimer().GetTimeDeltaSeconds();
		r->Update(time);
		r->Render();
	}

	Window::DestroyGameWindow();
	return 0;
}