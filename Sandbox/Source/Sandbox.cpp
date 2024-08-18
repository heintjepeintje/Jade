#include <Jade.hpp>
#include <iostream>
#include <fstream>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

static bool s_IsRunning = true;
static Jade::Window s_Window;
static Jade::GraphicsContext s_Context;
static Jade::SwapChain s_SwapChain;
static Jade::RendererAPI s_Renderer;
static Jade::VertexBuffer s_VertexBuffer;
static Jade::IndexBuffer s_IndexBuffer;
static Jade::RenderPass s_RenderPass;
static Jade::Shader s_Shader;
static Jade::RenderPipeline s_RenderPipeline;
static Jade::UniformBuffer s_UniformBuffer;

struct CameraData {
	glm::mat4 Projection;
};

static CameraData s_CameraData;

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
	s_Window = Jade::Window("Jade Engine", 1920 / 2, 1080 / 2);
	s_Window.SetVisible(true);
	s_Window.SetMaximized(true);
	s_Window.SetEventCallback(OnEvent);
	
	s_Context = Jade::GraphicsContext(s_Window);
	s_SwapChain = Jade::SwapChain(s_Context, 2);
	s_Renderer = Jade::RendererAPI(s_Context, s_SwapChain);
	s_RenderPass = Jade::RenderPass(s_Context);
	
	s_Shader = Jade::Shader(
		s_Context,
		ReadShader("C:\\dev\\Jade\\Sandbox\\Binaries\\Debug\\Shaders\\vertex.spv"),
		ReadShader("C:\\dev\\Jade\\Sandbox\\Binaries\\Debug\\Shaders\\fragment.spv")
	);
	
	s_RenderPipeline = Jade::RenderPipeline(s_Context, s_RenderPass, s_Shader, { 
		{ 0, Jade::RenderPipelineInputElementType::UniformBuffer }
	}, Jade::InputLayout::GetDefaultLayout());

	Jade::Vertex vertices[] = {
		{ { -0.5f,  0.5f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, -1.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f, -1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ {  0.5f,  0.5f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	};
	
	uint32_t indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	s_VertexBuffer = Jade::VertexBuffer::Create(s_Context, vertices, sizeof(vertices));
	s_IndexBuffer = Jade::IndexBuffer::Create(s_Context, indices, sizeof(indices));

	s_CameraData.Projection = glm::mat4(1.0f);
	s_CameraData.Projection = glm::perspectiveRH_NO(glm::radians(90.0f), (float)s_Window.GetWidth() / (float)s_Window.GetHeight(), 0.01f, 1000.0f);

	s_UniformBuffer = Jade::UniformBuffer::Create(s_Context, &s_CameraData, sizeof(s_CameraData));
	s_RenderPipeline.SetInputElement(0, s_UniformBuffer);
	
	float redValue = 1.0f;
	bool subtract = false;
	
	const float increment = 0.01f;
	
	while (s_IsRunning) {
		if (subtract) {
			redValue -= increment;
		} else {
			redValue += increment;
		}
		
		if (redValue <= 0.0f) {
			subtract = false;
		} else if (redValue >= 1.0f) {
			subtract = true;
		}
		 
		s_Renderer.Begin();
		s_Renderer.Clear(redValue, 1.0f - redValue, 1.0f, 1.0f);
		s_Renderer.BeginRender(s_RenderPipeline);
		s_Renderer.DrawIndexed(s_VertexBuffer, s_IndexBuffer);	
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