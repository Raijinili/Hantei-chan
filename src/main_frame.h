#ifndef MAINFRAME_H_GUARD
#define MAINFRAME_H_GUARD
#include "context_gl.h"
#include "render.h"
#include "main_pane.h"
#include <ha6/framedata.h>
#include <ha6/cg.h>
#include <glm/mat4x4.hpp>


class MainFrame
{
public:
	MainFrame(ContextGl *context);
	
	
	void Draw();
	void UpdateBackProj(glm::mat4 &&mat);
	void HandleMouseDrag(int x, int y);
	

private:
	ContextGl *context;
	float clearColor[3];
	int style_idx = 0;
	FrameData framedata;
	CG cg;


	void DrawBack();
	void DrawUi();
	void WarmStyle();
	void ChangeClearColor(float r, float g, float b);

	int mDeltaX = 0, mDeltaY = 0;
	int x=0, y=0;
	

	MainPane mainPane;
	Render render;
};


#endif /* MAINFRAME_H_GUARD */
