#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
	int programId;

	Shader(const char* vertex_src_path, const char* frag_src_path);
	void use();
		void setMat4Uniform(const char* name, glm::mat4 &value);

private:
	std::string fetchSourceCode(const char* path);
	
};

