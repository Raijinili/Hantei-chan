#ifndef MAINPANE_H_GUARD
#define MAINPANE_H_GUARD
#include "draw_window.h"

//This is the main pane on the left
class MainPane : DrawWindow
{
public:
	//MainPane();
	void Draw();

private:
	int counter;
	float f;
	
	bool show_another_window = false;
};

#endif /* MAINPANE_H_GUARD */
