#include "main_pane.h"
#include <imgui.h>	

MainPane::MainPane():
frameData(nullptr),
currentPattern(-1)
{
	
} 

void MainPane::SetFrameData(FrameData *frameData_)
{
	if(frameData_)
		currentPattern = 0;
	else
		currentPattern = -1;

	frameData = frameData_;
}


void MainPane::Draw()
{	

	ImGui::Begin("Left Pane",0 , ImGuiWindowFlags_NoMove );
	if(frameData)
	{
		if (ImGui::BeginCombo("Pattern", frameData->GetName(currentPattern).c_str(), ImGuiComboFlags_HeightLargest))
		{
			auto count = frameData->get_sequence_count();
			for (int n = 0; n < count; n++)
			{
				const bool is_selected = (currentPattern == n);
				if (ImGui::Selectable(frameData->GetName(n).c_str(), is_selected))
					currentPattern = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}


	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();

	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}