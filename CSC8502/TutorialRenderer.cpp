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

TutorialRenderer::TutorialRenderer() : OGLRenderer(*Window::GetWindow())
, controller(*(Window::GetWindow()->GetKeyboard()), *(Window::GetWindow()->GetMouse()))

{
	GLTFLoader::SetMeshConstructionFunction(MeshConstructionFunc);
	GLTFLoader::SetTextureConstructionFunction(TextureConstructionFunc);
	triMesh		= CreateTriangle();
	quadMesh	= CreateQuad();
	sphereMesh	= LoadSingleMesh("uvSphere.gltf");

	defaultCamera	= new PerspectiveCamera();
	defaultCamera->SetController(controller);
	totalRunTime	= 0;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	controller.MapAxis(0, "Sidestep");
	controller.MapAxis(1, "UpDown");
	controller.MapAxis(2, "Forward");

	controller.MapAxis(3, "XLook");
	controller.MapAxis(4, "YLook");
}

TutorialRenderer::~TutorialRenderer() {
	delete defaultCamera;
}

void TutorialRenderer::Update(float dt) {
	controller.Update(dt);
	defaultCamera->UpdateCamera(dt);
	totalRunTime += dt;
}

void TutorialRenderer::WindowEventHandler(WindowEvent e, uint32_t w, uint32_t h) {

}

UniqueOGLMesh TutorialRenderer::CreateTriangle() {
	UniqueOGLMesh triMesh = std::make_unique<OGLMesh>();
	triMesh->SetVertexPositions({ Vector3(-1,-1,0), Vector3(1,-1,0), Vector3(0,1,0) });
	triMesh->SetVertexColours({ Vector4(1,0,0,1), Vector4(0,1,0,1), Vector4(0,0,1,1) });
	triMesh->SetVertexTextureCoords({ Vector2(0,0), Vector2(1,0), Vector2(0.5, 1) });
	triMesh->SetVertexIndices({ 0,1,2 });

	triMesh->SetDebugName("Triangle");
	triMesh->SetPrimitiveType(NCL::GeometryPrimitive::TriangleStrip);
	triMesh->UploadToGPU(this);

	return triMesh;
}

UniqueOGLMesh TutorialRenderer::CreateQuad() {
	UniqueOGLMesh quadMesh = std::make_unique<OGLMesh>();
	quadMesh->SetVertexPositions({ Vector3(-1,-1,0), Vector3(1,-1,0), Vector3(-1,1,0), Vector3(1,1,0) });
	quadMesh->SetVertexTextureCoords({ Vector2(0,0), Vector2(1,0), Vector2(0, 1), Vector2(1, 1) });

	quadMesh->SetVertexNormals(
		std::vector<Vector3>(4, Vector3(0,0,-1))
	);

	quadMesh->SetVertexTangents(
		std::vector<Vector4>(4, Vector4(1, 0, 0, 1))
	);

	quadMesh->SetVertexIndices({ 0,1,2,3 });
	quadMesh->SetDebugName("Fullscreen Quad");
	quadMesh->SetPrimitiveType(NCL::GeometryPrimitive::TriangleStrip);
	quadMesh->UploadToGPU(this);

	return quadMesh;
}

UniqueOGLMesh TutorialRenderer::CreateHeightmap(const std::string& filename, Vector3 dimension) {
	UniqueOGLMesh gridMesh = std::make_unique<OGLMesh>();

	char* outData	= nullptr;
	uint32_t texWidth	= 0;
	uint32_t texHeight	= 0;
	uint32_t texChannels = 0;
	uint32_t texFlags	= 0;

	TextureLoader::LoadTexture(filename, outData, texWidth, texHeight, texChannels, texFlags);

	int vCount = texWidth * texHeight;
	int iCount = (texWidth - 1) * (texHeight - 1) * 6;

	std::vector<Vector3> vertices(vCount, Vector3(0, 0, 0));
	for (int z = 0; z < texHeight; ++z) {
		for (int x = 0; x < texWidth; ++x) {
			int offset = (z * texWidth) + x;

			unsigned char pixel = (outData[offset * texChannels]);

			Vector3 position;
			position.x = (x / (float)texWidth) - 0.5f;
			position.y = pixel / 255.0f;
			position.z = (z / (float)texHeight) - 0.5f;

			vertices[offset] = position * dimension;
		}
	}

	std::vector<unsigned int> indices(iCount, 0);
	int i = 0;
	for (int z = 0; z < texHeight - 1; ++z) {
		for (int x = 0; x < texWidth - 1; ++x) {
			int a = (z * (texWidth)) + x;
			int b = (z * (texWidth)) + (x + 1);

			int c = ((z + 1) * (texWidth)) + x;
			int d = ((z + 1) * (texWidth)) + (x + 1);


			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = d;

			indices[i++] = a;
			indices[i++] = d;
			indices[i++] = b;
		}
	}

	gridMesh->SetVertexPositions(vertices);
	gridMesh->SetVertexIndices(indices);
	gridMesh->SetDebugName("Grid");
	gridMesh->UploadToGPU(this);

	return gridMesh;
}

SharedMesh TutorialRenderer::MeshConstructionFunc() {
	return std::shared_ptr<Mesh>(new OGLMesh());
}

SharedTexture TutorialRenderer::TextureConstructionFunc(const std::string& s) {
	return std::shared_ptr<Texture>(OGLTexture::TextureFromFile(s));
}

SharedOGLMesh TutorialRenderer::LoadSingleMesh(const std::string& filename) {
	uint32_t currentCount = scene.meshes.size();

	GLTFLoader::Load(filename,scene);

	assert(scene.meshes.size() > currentCount);

	SharedOGLMesh mesh = std::dynamic_pointer_cast<OGLMesh>(scene.meshes.back());
	mesh->UploadToGPU(this);

	return mesh;
}

void TutorialRenderer::CalculateNormals(Mesh& m) const {
	int count = m.GetPrimitiveCount();

	std::vector<Vector3> newNormals;
	newNormals.resize(m.GetVertexCount());

	for (int i = 0; i < count; ++i) {
		Vector3 a, b, c;
		unsigned int ai, bi, ci;
		m.GetTriangle(i, a, b, c);
		m.GetVertexIndicesForTri(i, ai, bi, ci);

		Vector3 n = Vector::Cross(b - a, c - a);
		n = Vector::Normalise(n);

		newNormals[ai] += n;
		newNormals[bi] += n;
		newNormals[ci] += n;
	}
	for (int i = 0; i < newNormals.size(); ++i) {
		newNormals[i] = Vector::Normalise(newNormals[i]);
	}

	m.SetVertexNormals(newNormals);
}

void TutorialRenderer::CalculateNormalsTangents(Mesh& m) const {
	size_t primCount = m.GetPrimitiveCount();
	size_t vertCount = m.GetVertexCount();

	std::vector<Vector4> newTangents;
	newTangents.resize(vertCount);

	std::vector<Vector3> newNormals;
	newNormals.resize(vertCount);

	const std::vector<Vector3>& positions	= m.GetPositionData();
	const std::vector<Vector2>& UVs			= m.GetTextureCoordData();

	if (positions.size() != UVs.size()) {
		return;
	}

	for (int i = 0; i < primCount; ++i) {
		unsigned int ai, bi, ci;
		m.GetVertexIndicesForTri(i, ai, bi, ci);

		Vector3 ba = positions[bi] - positions[ai];
		Vector3 ca = positions[ci] - positions[ai];

		Vector2 tba = UVs[bi] - UVs[ai];
		Vector2 tca = UVs[ci] - UVs[ai];

		Matrix2 mat;
		mat.SetColumn(0, tba);
		mat.SetColumn(1, tca);

		Matrix2 texMatrix = Matrix::Inverse(mat);

		Vector3 tangent		= ba * texMatrix.array[0][0] + ca * texMatrix.array[0][1];
		Vector3 binormal	= ba * texMatrix.array[1][0] + ca * texMatrix.array[1][1];

		Vector3 normal	= Vector::Cross(ba, ca);
		Vector3 biCross = Vector::Cross(tangent, normal);

		float handedness = 1.0f;
		if (Vector::Dot(biCross, binormal) < 0.0f) {
			handedness = -1.0f;
		}

		Vector4 t(tangent.x, tangent.y, tangent.z, handedness);

		newTangents[ai] += t;
		newTangents[bi] += t;
		newTangents[ci] += t;

		newNormals[ai] += normal;
		newNormals[bi] += normal;
		newNormals[ci] += normal;
	}

	for (int i = 0; i < vertCount; ++i) {
		float handedness = newTangents[i].w > 0.0f ? 1.0f : -1.0f;
		newTangents[i].w = 0.0f;
		newTangents[i] = Vector::Normalise(newTangents[i]);
		newTangents[i].w = handedness;

		newNormals[i] = Vector::Normalise(newNormals[i]);
	}

	m.SetVertexTangents(newTangents);
	m.SetVertexNormals(newNormals);
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

void TutorialRenderer::SetUniform(const std::string& name, const Matrix3& mat) {
	assert(activeShader);
	GLuint id = activeShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniformMatrix3fv(location, 1, false, (float*)&mat);
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

void TutorialRenderer::SetUniform(const std::string& name, float f) {
	assert(activeShader);
	GLuint id = activeShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniform1f(location, f);
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

void TutorialRenderer::SetUniform(const std::string& name, const Vector3& v) {
	assert(activeShader);
	GLuint id = activeShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniform3f(location, v.x, v.y, v.z);
}

void TutorialRenderer::SetUniform(const std::string& name, const Vector4& v) {
	assert(activeShader);
	GLuint id = activeShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniform4f(location, v.x, v.y, v.z, v.w);
}

void TutorialRenderer::SetCameraUniforms(const Camera& c, GLuint location) {
	float aspect = Window::GetWindow()->GetScreenAspect();
	Matrix4 viewMatrix	= c.BuildViewMatrix();
	Matrix4 projMatrix	= c.BuildProjectionMatrix(aspect);
	Vector3 position	= c.GetPosition();

	assert(activeShader);

	glUniformMatrix4fv(location		, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(location + 1	, 1, false, (float*)&projMatrix);
	glUniform3fv(location + 2, 1, (float*)&position);
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
	glUniform1i(location, texUnit);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(texType, texID);
}

void TutorialRenderer::UpdateCamera(float dt) {
	cameraPitch -= Window::GetMouse()->GetRelativePosition().y;
	cameraYaw	-= Window::GetMouse()->GetRelativePosition().x;

	Matrix4 pitchRotation	= Matrix::Rotation(cameraPitch, Vector3(1, 0, 0));
	Matrix4 yawRotation		= Matrix::Rotation(cameraYaw, Vector3(0, 1, 0));

	float moveSpeed = dt * cameraSpeed;

	Matrix4 rotation = yawRotation * pitchRotation;

	Vector3 forward = rotation * Vector4(0, 0, -1, 0);
	Vector3 right	= rotation * Vector4(1, 0, 0, 0);

	if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) {
		cameraPosition -= right * moveSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) {
		cameraPosition += right * moveSpeed;
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) {
		cameraPosition += forward * moveSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) {
		cameraPosition -= forward * moveSpeed;
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::SHIFT)) {
		cameraPosition.y -= moveSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::SPACE)) {
		cameraPosition.y += moveSpeed;
	}

	Matrix4 translation = Matrix::Translation(cameraPosition);
	cameraViewMatrix = Matrix::Inverse(translation * rotation);
}

void TutorialRenderer::UpdateMatrices(const OGLShader& shader, const Matrix4& projMat, const Matrix4& viewMat, const Matrix4& modelMat) {
	GLint modelLocation = glGetUniformLocation(shader.GetProgramID(), "modelMatrix");
	GLint projLocation = glGetUniformLocation(shader.GetProgramID(),  "projMatrix");
	GLint viewLocation = glGetUniformLocation(shader.GetProgramID(),  "viewMatrix");

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMat);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMat);
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMat);
}

void TutorialRenderer::DrawTriScene(const OGLShader& shader, const Matrix4& projMat, const Matrix4& viewMat, float z0, float z1, float z2) {
	UseShader(shader);
	BindMesh(*triMesh);

	GLint colourLocation = glGetUniformLocation(shader.GetProgramID(), "objectColour");

	float depths[3] = {
		z0,z1,z2
	};

	for (int i = 0; i < 3; ++i) {
		Vector4 colour(0, 0, 0, 0.25f + (i * 0.25f));
		colour[i] = 1.0f;
		glUniform4fv(colourLocation, 1, &colour[0]);

		Matrix4 modelMat = Matrix::Translation(Vector3(2.0f, 0.0f, depths[i]));

		UpdateMatrices(shader, projMat, viewMat, modelMat);
		DrawBoundMesh();
	}
}

Frustum TutorialRenderer::CreateFrustum(const Matrix4& projMatrix, const Matrix4& viewMatrix) {
	Matrix4 viewProj			= projMatrix * viewMatrix;
	Matrix4 inverseViewProj		= Matrix::Inverse(viewProj);

	Vector4 camPos = inverseViewProj * Vector4(0, 0, -1.0f, 1.0f);

	const int cornerCount = 8;

	Vector4 clipCorners[cornerCount] = {
		inverseViewProj * Vector4(-1, -1, -1.0f, 1.0f),
		inverseViewProj * Vector4(-1,  1, -1.0f, 1.0f),
		inverseViewProj * Vector4( 1, -1, -1.0f, 1.0f),
		inverseViewProj * Vector4( 1,  1, -1.0f, 1.0f),

		inverseViewProj * Vector4(-1, -1, 1.0f, 1.0f),
		inverseViewProj * Vector4(-1,  1, 1.0f, 1.0f),
		inverseViewProj * Vector4( 1, -1, 1.0f, 1.0f),
		inverseViewProj * Vector4( 1,  1, 1.0f, 1.0f),
	};

	Vector3 worldCorners[cornerCount];
	//These now represent the world space corners of the view frustum
	for (int i = 0; i < cornerCount; ++i) {
		Vector3 temp = clipCorners[i];
		worldCorners[i] = temp / clipCorners[i].w;
	}

	Frustum f; //So let's form planes!


	return f;
}