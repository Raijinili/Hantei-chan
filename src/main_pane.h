#ifndef MAINPANE_H_GUARD
#define MAINPANE_H_GUARD
#include "draw_window.h"
#include <ha6/framedata.h>
#include <string>

//This is the main pane on the left
class MainPane : DrawWindow
{
public:
	MainPane();
	void Draw();
	void SetFrameData(FrameData *frameData);

private:
	FrameData *frameData;
	int currentPattern;
	int currFrame;

	std::string *decoratedNames;
};

#endif /* MAINPANE_H_GUARD */
