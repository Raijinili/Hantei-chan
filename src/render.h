#ifndef RENDER_H_GUARD
#define RENDER_H_GUARD
#include <glm/mat4x4.hpp>
#include "cg.h"
#include "texture.h"
#include "shader.h"
#include "vao.h"

class Render
{
private:
	glm::mat4 projection, view;
	
	CG *cg;
	Vao vSprite;
	Vao vGeometry;
	
	enum{
		LINES = 0,
		GEO_SIZE
	};
	int geoParts[GEO_SIZE];

	int lProjectionS, lProjectionT;
	Shader sSimple;
	Shader sTextured;
	Texture texture;
	int curImageId;
	float imageVertex[6*4];

	void AdjustImageQuad(int x, int y, int w, int h);

	void SetModelView(glm::mat4&& view);
	void SetMatrix(int location);

public:
	int x, offsetX;
	int y, offsetY;
	float scale;
	
	Render();
	void Draw();
	void UpdateProj(glm::mat4&& proj);


	void SetCg(CG *cg);
	void SwitchImage(int id);
};

#endif /* RENDER_H_GUARD */
