#include <Jade.hpp>
#include <iostream>
#include <fstream>

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
	
	s_RenderPipeline = Jade::RenderPipeline(s_Context, s_RenderPass, s_Shader, Jade::InputLayout::GetDefaultLayout());
	Jade::Vertex vertices[] = {
		{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	};
	
	uint32_t indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	s_VertexBuffer = Jade::VertexBuffer(s_Context, vertices, sizeof(vertices));
	s_IndexBuffer = Jade::IndexBuffer(s_Context, indices, sizeof(indices));
	
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