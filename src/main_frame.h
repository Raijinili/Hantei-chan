#ifndef MAINFRAME_H_GUARD
#define MAINFRAME_H_GUARD
#include "context_gl.h"
#include "render.h"
#include "main_pane.h"
#include <ha6/framedata.h>
#include <glm/mat4x4.hpp>


class MainFrame
{
public:
	MainFrame(ContextGl *context);
	
	
	void Draw();
	void UpdateBackProj(glm::mat4 &&mat);

private:
	ContextGl *context;
	float clearColor[3];
	int style_idx = 0;
	FrameData framedata;


	void DrawBack();
	void DrawUi();
	void WarmStyle();
	void ChangeClearColor(float r, float g, float b);
	

	MainPane mainPane;
	Render render;
};


#endif /* MAINFRAME_H_GUARD */
