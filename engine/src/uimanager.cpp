#include "uimanager.hpp"
// imgui includes for oprngl and glfw
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>



void UIMANAGER::uiInit(GLFWwindow* window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void UIMANAGER::render(Camera& camera,
    glm::mat4& model,
    bool& shouldRotate,
    glm::vec3& sunPosition,
    glm::vec3& lightColor
) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("ArchRenderer Inspector");
    ImGui::Text("FPS: %.1f (%.2f ms)", io.Framerate, 1000.0f / io.Framerate);
    ImGui::Text("Press [TAB] to toggle Cursor / Camera Mode");
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Transform & Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Auto-Rotate (F)", &shouldRotate);
        ImGui::SliderFloat("Camera FOV", &camera.Zoom, 15.0f, 90.0f);
        ImGui::Text("Camera Pos: (%.1f, %.1f, %.1f)", camera.Position.x, camera.Position.y, camera.Position.z);
    }

    if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Sun Position", glm::value_ptr(sunPosition), 1.0f, -500.0f, 500.0f);
        // ImGui::Text("Camera Pos: (%.1f, %.1f, %.1f)", sunPosition.x, sunPosition.y, sunPosition.z);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIMANAGER::shutdown(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}