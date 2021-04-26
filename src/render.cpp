#include "render.h"
#include "main.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Error
#include <windows.h>
#include <glad/glad.h>
#include <sstream>


Render::Render():
cg(nullptr),
vao(Vao::F2F2, GL_STATIC_DRAW)
{
	sSimple.BindAttrib("Position", 0);
	sSimple.BindAttrib("Color", 1);
	sSimple.LoadShader("src/simple.vert", "src/simple.frag");

	sTextured.BindAttrib("Position", 0);
	sTextured.BindAttrib("UV", 1);
	sTextured.LoadShader("src/textured.vert", "src/textured.frag");
	sTextured.Use();
	
	lProjection = sSimple.GetLoc("ProjMtx");
	

	float imageVertex[] = {
		256, 256, 	0, 0,
		512, 256,  	1, 0, 
		512, 512,  	1, 1, 

		512, 512, 	1, 1,
		256, 512,  	0, 1,
		256, 256,  	0, 0,
	};

	vao.Prepare(sizeof(imageVertex), imageVertex);
	vao.Load();
	
	projection = glm::ortho<float>(0, clientRect.x, clientRect.y, 0, -1.f, 1.f);
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
}

void Render::SetCg(CG *cg_)
{
	cg = cg_;
	texture.Load(cg->draw_texture(100, false, false));
	glActiveTexture(GL_TEXTURE0);
	texture.Apply();
	texture.Unload();

}