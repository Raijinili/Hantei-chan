#ifndef MAINFRAME_H_GUARD
#define MAINFRAME_H_GUARD
#include "context_gl.h"

class MainFrame
{
public:
	MainFrame(ContextGl *context);
	
	
	void Draw();

private:
	ContextGl *context;
	float clearColor[3];

	void DrawBack();
	void DrawUi();
	void WarmStyle();
	
};

#endif /* MAINFRAME_H_GUARD */
