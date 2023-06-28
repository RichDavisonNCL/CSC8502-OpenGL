/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once

namespace NCL::CSC8502 {
	struct Light {
		NCL::Maths::Vector3 position;
		float	radius;
		NCL::Maths::Vector3 intensity;
		float	unused;

	};
}