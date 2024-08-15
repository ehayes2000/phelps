#include "ui.hpp"
#include <imgui.h>

void ShowUi(RenderParameters &params,
  FluidParameters &fluidParams,
  bool * p_open) {
  const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(550, 6), ImGuiCond_FirstUseEver);
  ImGuiWindowFlags window_flags = 0;
  if (!ImGui::Begin("Debug", p_open, window_flags)){
    ImGui::End();
    return;
  }
  ImGui::Checkbox("Adjacent View", &params.isAdjacentView);
  ImGui::Checkbox("Density View", &params.isDensityView);
  ImGui::Separator();
  const float dragSpeed = .001;
  const float range = 100.f;
  ImGui::Text("Fluid Parameters");
  ImGui::DragFloat("Collision Damping", &fluidParams.collisionDamping, dragSpeed, -range, range);
  ImGui::DragFloat("Smoothing Radius", &fluidParams.smoothingRadius, dragSpeed, -range, range);
  ImGui::DragFloat("Target Density", &fluidParams.targetDensity, dragSpeed, -range, range);
  ImGui::DragFloat("Pressure", &fluidParams.pressureMultiplier, dragSpeed, -range, range);
  ImGui::DragFloat("Near Pressure", &fluidParams.nearPressureMultiplier, dragSpeed, -range, range);
  ImGui::DragFloat("Viscosity Beta", &fluidParams.viscosityBeta, dragSpeed, -range, range);
  ImGui::DragFloat("Viscosity Delta", &fluidParams.viscosityDelta, dragSpeed, -range, range);
  ImGui::DragFloat("Gravity", &fluidParams.gravity, dragSpeed, -range, range);
  ImGui::DragScalar("N Particles", ImGuiDataType_U32, &fluidParams.nParticles, dragSpeed);
  ImGui::End();
}