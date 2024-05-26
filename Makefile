TARGET = graphics
GLFLAGS = -lglfw -lGL 
INCLUDES = -I./include 
CPPFLAGS = -Wall $(INCLUDES)

main:
	g++ ${CPPFLAGS} ${GLFLAGS} -o ${TARGET} src/*.cpp include/glad/glad.c 
	@./graphics



