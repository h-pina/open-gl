#include "Logger.hpp"
#include <iostream>


void Logger::debug(const char* message ){
	if(debugEnabled){
		std::cout << "[DEBUG] " << message << std::endl;
	}
}
void Logger::error(const char* message ){
	std::cout << "[ERROR] " << message << std::endl;
}

void Logger::info(const char* message ){
	std::cout << "[INFO] " << message << std::endl;
}
