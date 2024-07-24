#include <Jade.hpp>
#include <iostream>

static bool s_IsRunning = true;

bool OnWindowClose(const Jade::WindowCloseEvent &event JD_UNUSED) {
	s_IsRunning = false;
	JD_INFO("Shutting Down!");
	return true;
}

void OnEvent(const Jade::Event &e) {
	Jade::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Jade::WindowCloseEvent>(OnWindowClose);
}

int main(int argc JD_UNUSED, char **argv JD_UNUSED) {
	Jade::Init();
	
	JD_INFO("Hello %s", "World!");
	
	Jade::Window window("Hello World!", 1920 / 2, 1080 / 2);
	window.SetEventCallback(OnEvent);
	window.SetVisible(true);
	window.SetMaximized(true);
	
	while (s_IsRunning) {
		window.Update();
	} 
	
	Jade::Shutdown();
	
	std::cin.get();
}