#include <Jade.hpp>
#include <iostream>

int main(int argc JD_UNUSED, char **argv JD_UNUSED) {
	Jade::Init();
	
	JD_INFO("Hello %s", "World!");
	
	Jade::Buffer buf = Jade::Buffer(256);
	
	for (uint32_t i = 0; i < buf.GetSize(); i += sizeof(uint32_t)) {
		uint32_t data = i / sizeof(uint32_t);
		buf.Write<uint32_t>(i, &data);
	}
	
	for (uint32_t i = 0; i < buf.GetSize(); i += sizeof(uint32_t)) {
		uint32_t data = 0;
		buf.Read<uint32_t>(i, &data);
		JD_TRACE("[%u]: %u", i, data);
	}
	
	JD_ASSERT(0, "Testing assertions");
	
	Jade::Shutdown();
	
	std::cin.get();
}