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
vSprite(Vao::F2F2, GL_DYNAMIC_DRAW),
vGeometry(Vao::F2F3, GL_DYNAMIC_DRAW),
curImageId(-1),
imageVertex{
	256, 256, 	0, 0,
	512, 256,  	1, 0, 
	512, 512,  	1, 1, 

	512, 512, 	1, 1,
	256, 512,  	0, 1,
	256, 256,  	0, 0,
},
x(0), offsetX(0),
y(0), offsetY(0)
{
	sSimple.BindAttrib("Position", 0);
	sSimple.BindAttrib("Color", 1);
	sSimple.LoadShader("src/simple.vert", "src/simple.frag");

	sTextured.BindAttrib("Position", 0);
	sTextured.BindAttrib("UV", 1);
	sTextured.LoadShader("src/textured.vert", "src/textured.frag");
	
	lProjectionS = sSimple.GetLoc("ProjMtx");
	lProjectionT = sTextured.GetLoc("ProjMtx");

	vSprite.Prepare(sizeof(imageVertex), imageVertex);
	vSprite.Load();

	float lines[]
	{
		-10000, 0,	1,1,1,
		10000, 0,	1,1,1,
		0, 10000,	1,1,1,
		0, -10000,	1,1,1,
	};

	geoParts[LINES] = vGeometry.Prepare(sizeof(lines), lines);
	vGeometry.Load();

	projection = glm::ortho<float>(0, clientRect.x, clientRect.y, 0, -1.f, 1.f);

	glViewport(0, 0, clientRect.x, clientRect.y);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Render::Draw()
{
	if(int err = glGetError())
	{
		std::stringstream ss;
		ss << "GL Error: 0x" << std::hex << err << "\n";
		MessageBoxA(nullptr, ss.str().c_str(), "GL Error", MB_ICONSTOP);
		//PostQuitMessage(1);
	}

	SetModelView(
		glm::scale(
			glm::translate(glm::mat4(1), glm::vec3(x,y,0)),
			glm::vec3(scale, scale, 1.f)
		)
	);
	sSimple.Use();
	SetMatrix(lProjectionS);
	vGeometry.Bind();
	vGeometry.Draw(geoParts[LINES], 0, GL_LINES);

	SetModelView(
		glm::scale(
			glm::translate(glm::mat4(1), glm::vec3(x+offsetX,y+offsetY,0)),
			glm::vec3(scale, scale, 1.f)
		)
	);
	sTextured.Use();
	SetMatrix(lProjectionT);
	if(texture.isApplied)
	{
		vSprite.Bind();
		vSprite.Draw(0);
	}
}

void Render::SetModelView(glm::mat4&& view_)
{
	view = std::move(view_);
}

void Render::SetMatrix(int lProjection)
{
	glUniformMatrix4fv(lProjection, 1, GL_FALSE, glm::value_ptr(projection*view));
}

void Render::UpdateProj(glm::mat4&& view)
{
	projection = view;
}

void Render::SetCg(CG *cg_)
{
	cg = cg_;
}

void Render::SwitchImage(int id)
{
	if(id != curImageId)
	{
		texture.Unapply();

		if(id>=0)
		{
			texture.Load(cg->draw_texture(id, false, false));
			texture.Apply();
			
			AdjustImageQuad(texture.image->offsetX, texture.image->offsetY, texture.image->width, texture.image->height);
			vSprite.UpdateBuffer(0, imageVertex);
			texture.Unload();
		}
		curImageId = id;
	}
}

void Render::AdjustImageQuad(int x, int y, int w, int h)
{
	w+=x;
	h+=y;

	imageVertex[0] = imageVertex[16] = imageVertex[20] = x;
	imageVertex[4] = imageVertex[8] = imageVertex[12] = w;

	imageVertex[1] = imageVertex[5] = imageVertex[21] = y;
	imageVertex[9] = imageVertex[13] = imageVertex[17] = h;

}