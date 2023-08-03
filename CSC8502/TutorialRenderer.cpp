/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "TutorialRenderer.h"
#include "GLTFLoader.h"
namespace NCL::Rendering {
#include "./Shaders/ShaderInterop.h"
#include "./Shaders/LightStruct.h"
}

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

TutorialRenderer::TutorialRenderer() : OGLRenderer(*Window::GetWindow()) , 
	gltf(
		[](void) ->  Mesh* {
			return new OGLMesh();
		},
		[](std::string& input) -> OGLTexture* { return nullptr; }
	) 
{
	triMesh		= CreateTriangle();
	quadMesh	= CreateQuad();
	sphereMesh	= LoadSingleMesh("uvSphere.gltf");

	defaultCamera	= new PerspectiveCamera();
	totalRunTime	= 0;
}

TutorialRenderer::~TutorialRenderer() {
	delete triMesh;
	delete quadMesh;
	delete sphereMesh;
	delete defaultCamera;
}

void TutorialRenderer::Update(float dt) {
	defaultCamera->UpdateCamera(dt);
	totalRunTime += dt;
}

OGLMesh* TutorialRenderer::CreateTriangle() {
	OGLMesh* triMesh = new OGLMesh();
	triMesh->SetVertexPositions({ Vector3(-1,-1,0), Vector3(1,-1,0), Vector3(0,1,0) });
	triMesh->SetVertexColours({ Vector4(1,0,0,1), Vector4(0,1,0,1), Vector4(0,0,1,1) });
	triMesh->SetVertexTextureCoords({ Vector2(0,0), Vector2(1,0), Vector2(0.5, 1) });
	triMesh->SetVertexIndices({ 0,1,2 });

	triMesh->SetDebugName("Triangle");
	triMesh->SetPrimitiveType(NCL::GeometryPrimitive::TriangleStrip);
	triMesh->UploadToGPU(this);

	return triMesh;
}

OGLMesh* TutorialRenderer::CreateQuad() {
	OGLMesh* quadMesh = new OGLMesh();
	quadMesh->SetVertexPositions({ Vector3(-1,-1,0), Vector3(1,-1,0), Vector3(1,1,0), Vector3(-1,1,0) });
	quadMesh->SetVertexTextureCoords({ Vector2(0,0), Vector2(1,0), Vector2(1, 1), Vector2(0, 1) });

	quadMesh->SetVertexNormals(
		std::vector<Vector3>(4, Vector3(0,0,-1))
	);

	quadMesh->SetVertexTangents(
		std::vector<Vector4>(4, Vector4(1, 0, 0, 1))
	);

	quadMesh->SetVertexIndices({ 0,1,3,2 });
	quadMesh->SetDebugName("Fullscreen Quad");
	quadMesh->SetPrimitiveType(NCL::GeometryPrimitive::TriangleStrip);
	quadMesh->UploadToGPU(this);

	return quadMesh;
}

OGLMesh* TutorialRenderer::LoadSingleMesh(const std::string& filename) {
	gltf.Load(filename);

	assert(!gltf.outMeshes.empty());
	gltf.outMeshes[0]->UploadToGPU(this);
	OGLMesh* m = (OGLMesh*)gltf.outMeshes[0];
	gltf.outMeshes.pop_back();
	return m;
}


GLuint	TutorialRenderer::CreateTexture(uint32_t width, uint32_t height, GLint internalFormat, bool allocMips) {
	GLuint tex = 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLsizei levels = 1;

	if (allocMips) {

	}

	glTexStorage2D(GL_TEXTURE_2D, levels, internalFormat, width, height);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

void TutorialRenderer::SetUniform(const std::string& name, const Matrix4& mat) {
	assert(activeShader);
	GLuint id		= activeShader->GetProgramID();
	GLint  location	= glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniformMatrix4fv(location, 1, false, (float*)&mat);
}

void TutorialRenderer::SetUniform(const std::string& name, const Vector2& v) {
	assert(activeShader);
	GLuint id = activeShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniform2f(location, v.x, v.y);
}

void TutorialRenderer::SetCameraUniforms(const Camera& c, GLuint location) {
	float aspect = Window::GetWindow()->GetScreenAspect();
	Matrix4 viewMatrix	= c.BuildViewMatrix();
	Matrix4 projMatrix	= c.BuildProjectionMatrix(aspect);
	Vector3 position	= c.GetPosition();

	assert(activeShader);
	GLuint id = activeShader->GetProgramID();

	glUniformMatrix4fv(location		, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(location + 1	, 1, false, (float*)&projMatrix);
	glUniform3fv(location + 2, 1, (float*)&position);
}

void TutorialRenderer::BindBufferAsUBO(const OGLBuffer& b, GLuint slotID) {
	glBindBufferBase(GL_UNIFORM_BUFFER, slotID, b.gpuID);
}

void TutorialRenderer::BindBufferAsSSBO(const OGLBuffer& b, GLuint slotID) {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slotID, b.gpuID);
}

void TutorialRenderer::SetLightUniforms(const Light& l, GLuint location) {
	assert(activeShader);
	GLuint id = activeShader->GetProgramID();

	glUniform3fv(location	, 1, (float*)&l.position);
	glUniform1f(location + 1, l.radius);
	glUniform3fv(location + 2, 1, (float*)&l.intensity);
}

void TutorialRenderer::BindTextureToPipeline(GLuint texID, const std::string& name, GLuint texUnit, GLuint texType)  {
	assert(activeShader);
	GLuint id = activeShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(texType, texID);
	glUniform1i(location, texUnit);
}