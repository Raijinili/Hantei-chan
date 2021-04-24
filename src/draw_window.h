#ifndef DRAWWINDOW_H_GUARD
#define DRAWWINDOW_H_GUARD

//ImGui Windows that draw themselves. Just for utility.
class DrawWindow
{
public:
	bool isVisible = false;
	virtual void Draw() = 0;
};

#endif /* DRAWWINDOW_H_GUARD */
