#ifndef MAINPANE_H_GUARD
#define MAINPANE_H_GUARD
#include "draw_window.h"
#include "framedata.h"
#include "render.h"
#include <string>

//This is the main pane on the left
class MainPane : DrawWindow
{
public:
	MainPane(Render* render);
	void Draw();
	void SetFrameData(FrameData *frameData);
	void RegenerateNames();
	void AdvancePattern(int dir);
	void ForceUpdate();

private:
	Render *render;
	FrameData *frameData;
	int currentPattern;
	int currFrame;
	int spriteId;

	std::string *decoratedNames;

	
};

#endif /* MAINPANE_H_GUARD */
