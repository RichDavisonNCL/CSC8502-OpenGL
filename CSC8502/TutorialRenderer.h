/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "OGLRenderer.h"
#include "GLTFLoader.h"
#include "OGLBuffer.h"

namespace NCL::Rendering {
	struct Light;
}

namespace NCL::CSC8502 {
	using namespace Rendering;

	class TutorialRenderer : public OGLRenderer {
	public:
		TutorialRenderer();
		~TutorialRenderer();

		virtual void Update(float dt);

	protected:
		UniqueOGLMesh CreateTriangle();
		UniqueOGLMesh CreateQuad();
		SharedOGLMesh LoadSingleMesh(const std::string& filename);

		GLuint	CreateTexture(uint32_t width, uint32_t height, GLint internalformat, bool allocMips = false);

		void BindTextureToPipeline(GLuint texID, const std::string& name, GLuint texUnit, GLuint texType = GL_TEXTURE_2D);

		void SetUniform(const std::string& name, const Matrix4& mat);
		void SetUniform(const std::string& name, const Vector2& mat);

		void BindBufferAsUBO(const OGLBuffer& b, GLuint slotID);
		void BindBufferAsSSBO(const OGLBuffer& b, GLuint slotID);

		void SetCameraUniforms(const Camera& c, GLuint slot);
		void SetLightUniforms(const Light& l, GLuint slot);

		void CalculateNormals(Mesh& m) const;
		void CalculateNormalsTangents(Mesh& m) const;

		UniqueOGLMesh triMesh;			//Make in tutorial 1
		UniqueOGLMesh quadMesh;			//
		SharedOGLMesh sphereMesh;

		Camera* defaultCamera;

		GLTFLoader gltf;

		float totalRunTime;
	};
}