#ifndef RENDER_H_GUARD
#define RENDER_H_GUARD
#include <glm/mat4x4.hpp>
#include "shader.h"
#include "vao.h"

class Render
{
private:
	Vao vao;
	int lProjection;
	Shader sSimple;
	Shader sTextured;

public:
	glm::mat4 projection;
	
	Render();
	void Draw();
	void Render::SetModelView(glm::mat4&& view);
	void Render::UpdateProj(glm::mat4&& proj);
};

#endif /* RENDER_H_GUARD */
