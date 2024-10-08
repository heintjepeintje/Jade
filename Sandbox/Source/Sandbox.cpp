#include <Jade.hpp>
#include <iostream>
#include <fstream>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#define JD_MAX_OBJECT_COUNT 16

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static bool s_IsRunning = true;
static Jade::Window s_Window;
static Jade::GraphicsContext s_Context;
static Jade::SwapChain s_SwapChain;
static Jade::RendererAPI s_Renderer;

static Jade::VertexBuffer s_VertexBuffer;
static Jade::VertexBuffer s_OtherVertexBuffer;
static Jade::IndexBuffer s_IndexBuffer;

static Jade::RenderPass s_RenderPass;
static Jade::Shader s_Shader;
static Jade::RenderPipeline s_RenderPipeline;
static Jade::UniformBuffer s_CameraDataBuffer;
static Jade::UniformBuffer s_ObjectDataBuffer;
static Jade::Texture2D s_Texture;

struct CameraData {
	glm::mat4 Projection;
	glm::mat4 View;
};

struct ObjectData {
	glm::mat4 Model[16];
};

static CameraData s_CameraData;
static ObjectData s_ObjectData;

bool OnWindowClose(const Jade::WindowCloseEvent &event JD_UNUSED) {
	s_IsRunning = false;
	JD_INFO("Shutting Down!");
	return true;
}

void OnEvent(const Jade::Event &e) {
	Jade::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Jade::WindowCloseEvent>(OnWindowClose);
}

Jade::Buffer ReadShader(const std::string &path) {
	std::ifstream file = std::ifstream(path, std::ios::ate | std::ios::binary);
	
	JD_ASSERT(file.is_open(), "Could not find file: %s", path.c_str());	
	
	size_t size = (size_t) file.tellg();
	Jade::Buffer shaderBinary(size);
	
	file.seekg(0);
	file.read((char*)shaderBinary.GetData(), shaderBinary.GetSize());
	
	file.close();
	
	return shaderBinary;
}

static void Run() {
	s_Window = Jade::Window::Create("Jade Engine", 1920 / 2, 1080 / 2);
	s_Window.SetVisible(true);
	s_Window.SetMaximized(true);
	s_Window.SetEventCallback(OnEvent);
	
	s_Context = Jade::GraphicsContext::Create(s_Window);
	s_SwapChain = Jade::SwapChain::Create(s_Context, 2);
	s_Renderer = Jade::RendererAPI::Create(s_Context, s_SwapChain);
	s_RenderPass = Jade::RenderPass::Create(s_Context);
	
	s_Shader = Jade::Shader::Create(
		s_Context,
		ReadShader("Binaries\\Debug\\Shaders\\vertex.spv"),
		ReadShader("Binaries\\Debug\\Shaders\\fragment.spv")
	);
	
	s_RenderPipeline = Jade::RenderPipeline::Create(s_Context, s_RenderPass, s_Shader, { 
		{ 0, Jade::RenderPipelineInputElementType::UniformBuffer },
		{ 1, Jade::RenderPipelineInputElementType::UniformBuffer },
		{ 2, Jade::RenderPipelineInputElementType::Texture2D }
	}, Jade::InputLayout::GetDefaultLayout());

	Jade::Vertex vertices[] = {
		{ { -0.5f,  0.5f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0 } },
		{ { -0.5f, -0.5f, -1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0 } },
		{ {  0.5f, -0.5f, -1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0 } },
		{ {  0.5f,  0.5f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0 } },
	};

	Jade::Vertex otherVertices[] = {
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1 } },
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1 } },
		{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1 } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1 } },
	};
	
	uint32_t indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	s_VertexBuffer = Jade::VertexBuffer::Create(s_Context, vertices, sizeof(vertices));
	s_OtherVertexBuffer = Jade::VertexBuffer::Create(s_Context, otherVertices, sizeof(otherVertices));
	s_IndexBuffer = Jade::IndexBuffer::Create(s_Context, indices, sizeof(indices));

	s_CameraData.Projection = glm::perspectiveRH_NO(glm::radians(90.0f), (float)s_Window.GetWidth() / (float)s_Window.GetHeight(), 0.01f, 1000.0f);
	s_CameraData.View = glm::mat4(1.0f);

	s_ObjectData.Model[0] = glm::mat4(1.0f);
	s_ObjectData.Model[1] = glm::mat4(1.0f);

	s_CameraDataBuffer = Jade::UniformBuffer::Create(s_Context, sizeof(CameraData));

	CameraData *cameraData = s_CameraDataBuffer.MapAs<CameraData>();
	*cameraData = s_CameraData;
	s_CameraDataBuffer.Unmap();

	s_ObjectDataBuffer = Jade::UniformBuffer::Create(s_Context, JD_MAX_OBJECT_COUNT * sizeof(ObjectData));
	
	s_RenderPipeline.SetInputElement(0, s_CameraDataBuffer);

	int width, height, bpp;
	stbi_uc *data = stbi_load("Resources/Images/Image0.jpg", &width, &height, &bpp, 4); 
	s_Texture = Jade::Texture2D::Create(s_Context, width, height, 4, data);
	
	s_RenderPipeline.SetInputElement(1, s_ObjectDataBuffer);
	s_RenderPipeline.SetInputElement(2, s_Texture);

	while (s_IsRunning) {		 
		s_Renderer.Begin();
		s_Renderer.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		s_Renderer.BeginRender(s_RenderPipeline);
		
		s_ObjectData.Model[0] = glm::rotate(s_ObjectData.Model[0], glm::radians(0.05f), glm::vec3(0.0f, 0.0f, -1.0f));
		
		s_Renderer.DrawIndexed(s_OtherVertexBuffer, s_IndexBuffer);
		
		s_ObjectData.Model[1] = glm::rotate(s_ObjectData.Model[1], glm::radians(-0.05f), glm::vec3(0.0f, 0.0f, -1.0f));
		
		s_Renderer.DrawIndexed(s_VertexBuffer, s_IndexBuffer);

		ObjectData *mappedData = s_ObjectDataBuffer.MapAs<ObjectData>();
		*mappedData = s_ObjectData;
		s_ObjectDataBuffer.Unmap();
		
		s_Renderer.EndRender();
		s_Renderer.End();
		
		s_SwapChain.Present();
		s_Window.Update();
	}
}

int main(int argc JD_UNUSED, char **argv JD_UNUSED) {
	Jade::Init();
	
	Run();
	
	Jade::Shutdown();
	
	std::cin.get();
}