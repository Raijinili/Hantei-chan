#ifndef MAINPANE_H_GUARD
#define MAINPANE_H_GUARD
#include "draw_window.h"
#include <ha6/framedata.h>

//This is the main pane on the left
class MainPane : DrawWindow
{
public:
	MainPane();
	void Draw();
	void SetFrameData(FrameData *frameData);

private:
	int counter;
	float f;

	FrameData *frameData;
	int currentPattern;
	
	bool show_another_window = false;
};

#endif /* MAINPANE_H_GUARD */
