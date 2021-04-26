#ifndef RENDER_H_GUARD
#define RENDER_H_GUARD
#include <glm/mat4x4.hpp>
#include <ha6/cg.h>
#include "texture.h"
#include "shader.h"
#include "vao.h"

class Render
{
private:
	CG *cg;
	Vao vao;
	int lProjection;
	Shader sSimple;
	Shader sTextured;

	Texture texture;

public:
	glm::mat4 projection;
	
	Render();
	void Draw();
	void SetModelView(glm::mat4&& view);
	void UpdateProj(glm::mat4&& proj);
	void SetCg(CG *cg);
};

#endif /* RENDER_H_GUARD */
