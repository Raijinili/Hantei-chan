#include <imgui.h>
#include "framedata.h"

inline void PatternDisplay(Sequence *seq)
{
	constexpr float spacing = 80;
	float width = 115;
	float height = ImGui::GetTextLineHeightWithSpacing()*5.5;

	ImGui::BeginChild("child", ImVec2(width, height), true);
	ImGui::Text("Hitboxes:"); ImGui::SameLine(spacing);
	ImGui::Text("%llu", seq->hitboxes.size());
	
	ImGui::Text("nAT:"); ImGui::SameLine(spacing);
	ImGui::Text("%llu", seq->AT.size());

	ImGui::Text("nAS:"); ImGui::SameLine(spacing);
	ImGui::Text("%llu", seq->AS.size());

	ImGui::Text("nEF:"); ImGui::SameLine(spacing);
	ImGui::Text("%llu", seq->EF.size());

	ImGui::Text("nIF:"); ImGui::SameLine(spacing);
	ImGui::Text("%llu", seq->IF.size());
	ImGui::EndChild();

	constexpr float ptcnSpacing=22.f;
	ImGui::SameLine();
	ImGui::BeginChild("child2", ImVec2(ImGui::GetWindowWidth()-width-60,height), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::SetNextItemWidth(ptcnSpacing);
	ImGui::InputScalarN("##PTCN1", ImGuiDataType_S8, &seq->ptcn[0], 1, NULL, NULL, "%d");
	ImGui::SetNextItemWidth(ptcnSpacing); ImGui::SameLine();
	ImGui::InputScalarN("##PTCN2", ImGuiDataType_S8, &seq->ptcn[1], 1, NULL, NULL, "%d");
	ImGui::SetNextItemWidth(ptcnSpacing); ImGui::SameLine();
	ImGui::InputScalarN("##PTCN3", ImGuiDataType_S8, &seq->ptcn[2], 1, NULL, NULL, "%d");
	ImGui::SetNextItemWidth(ptcnSpacing); ImGui::SameLine();
	ImGui::InputScalarN("##PTCN4", ImGuiDataType_S8, &seq->ptcn[3], 1, NULL, NULL, "%d");
	ImGui::SetNextItemWidth(ptcnSpacing); ImGui::SameLine();
	ImGui::InputScalarN("PTCN", ImGuiDataType_S8, &seq->ptcn[4], 1, NULL, NULL, "%d");
	ImGui::SameLine();
	ImGui::Checkbox("##BPTCN", &seq->hasPtcn);
	
	ImGui::SetNextItemWidth(40.f);
	ImGui::InputInt("PSTS", &seq->psts, 0, 0);
	ImGui::SameLine();
	ImGui::Checkbox("##BPSTS", &seq->hasPsts);

	ImGui::SetNextItemWidth(40.f);
	ImGui::InputInt("Level", &seq->level, 0, 0);
	ImGui::SameLine();
	ImGui::Checkbox("##BPLVL", &seq->hasLevel);

	ImGui::SetNextItemWidth(80.f);
	ImGui::InputInt("Flag (PFLG)", &seq->flag, 0, 0);
	ImGui::SameLine();
	ImGui::Checkbox("##BPFLG", &seq->hasFlag);

	ImGui::EndChild();
}