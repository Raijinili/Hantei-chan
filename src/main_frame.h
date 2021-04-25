#ifndef MAINFRAME_H_GUARD
#define MAINFRAME_H_GUARD
#include "context_gl.h"
#include "main_pane.h"
#include <ha6/framedata.h>

class MainFrame
{
public:
	MainFrame(ContextGl *context);
	
	
	void Draw();

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
	
};

#endif /* MAINFRAME_H_GUARD */
