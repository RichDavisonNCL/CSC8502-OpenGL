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
#include "KeyboardMouseController.h"
#include "SceneNode.h"
#include "Frustum.h"

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

		static SharedMesh MeshConstructionFunc();
		static SharedTexture TextureConstructionFunc(const std::string& s);

		virtual void WindowEventHandler(WindowEvent e, uint32_t w, uint32_t h);



	protected:
		UniqueOGLMesh CreateHeightmap(const std::string& filename, Vector3 dimension);
		UniqueOGLMesh CreateTriangle();
		UniqueOGLMesh CreateQuad();
		SharedOGLMesh LoadSingleMesh(const std::string& filename);

		void UpdateCamera(float dt);

		void UpdateMatrices(const OGLShader& shader, const Matrix4& projMat, const Matrix4& viewMat, const Matrix4& modelMat);
		void DrawTriScene(const OGLShader& shader, const Matrix4& projMat, const Matrix4& viewMat, float z0 = -5.0f, float z1 = -8.0f, float z2 = -11.0f);

		GLuint	CreateTexture(uint32_t width, uint32_t height, GLint internalformat, bool allocMips = false);

		void BindTextureToPipeline(GLuint texID, const std::string& name, GLuint texUnit, GLuint texType = GL_TEXTURE_2D);

		void SetUniform(const std::string& name, const Matrix3& mat);
		void SetUniform(const std::string& name, const Matrix4& mat);

		void SetUniform(const std::string& name, float f);
		void SetUniform(const std::string& name, const Vector2& vec);
		void SetUniform(const std::string& name, const Vector3& vec);
		void SetUniform(const std::string& name, const Vector4& vec);


		void SetCameraUniforms(const Camera& c, GLuint slot);
		void SetLightUniforms(const Light& l, GLuint slot);

		void CalculateNormals(Mesh& m) const;
		void CalculateNormalsTangents(Mesh& m) const;

		Frustum CreateFrustum(const Matrix4& projMatrix, const Matrix4& viewMatrix);

		UniqueOGLMesh triMesh;			//Make in tutorial 1
		UniqueOGLMesh quadMesh;			//
		SharedOGLMesh sphereMesh;

		Camera* defaultCamera;
		KeyboardMouseController controller;

		GLTFScene	scene;

		float totalRunTime;

		float	cameraSpeed = 5.0f;
		float	cameraPitch = 0.0f;
		float	cameraYaw	= 0.0f;
		Vector3 cameraPosition;
		Matrix4 cameraViewMatrix;
	};
}