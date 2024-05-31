#pragma once

#include <string>

class Shader {
public:
	int programId;
	Shader(const char* vertex_src_path, const char* frag_src_path);
	void use();

private:
	std::string fetchSourceCode(const char* path);
	
};

