#include "render.h"
#include "main.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Error
#include <windows.h>
#include <sstream>

#include <glad/glad.h>

Render::Render():
vao(Vao::F2F3, GL_STATIC_DRAW)
{
	sSimple.LoadShader("src/simple.vert", "src/simple.frag");
	sSimple.Use();
	
	lProjection = sSimple.GetLoc("ProjMtx");
	sSimple.BindAttrib("Position", 0);
	sSimple.BindAttrib("Color", 1);
	lAttribIndexSimple[1] = sSimple.GetAttribLoc("ProjMtx");
	
	float stageVertices[] = {
		0, 0, 	1, 1, 0,
		2000, 0,  	0, 1, 1,
		2000, 2000,  	1, 0, 1,
	};

	vao.Prepare(sizeof(stageVertices), stageVertices);
	vao.Load();
	
	projection = glm::ortho<float>(0, clientRect.x/2.f, clientRect.y/2.f, 0, -1.f, 1.f);
	glUniformMatrix4fv(lProjection, 1, GL_FALSE, glm::value_ptr(projection));

	glViewport(0, 0, clientRect.x, clientRect.y);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Render::Draw()
{
	vao.Bind();
	vao.Draw(0);
	if(int err = glGetError())
	{
		std::stringstream ss;
		ss << "GL Error: 0x" << std::hex << err << "\n";
		MessageBoxA(nullptr, ss.str().c_str(), "GL Error", MB_ICONSTOP);
		//PostQuitMessage(1);
	}
}

void Render::SetModelView(glm::mat4&& view)
{
	view = projection*view;
	glUniformMatrix4fv(lProjection, 1, GL_FALSE, glm::value_ptr(view));
}

void Render::UpdateProj(glm::mat4&& view)
{
	projection = view;
	glUniformMatrix4fv(lProjection, 1, GL_FALSE, glm::value_ptr(view));
}