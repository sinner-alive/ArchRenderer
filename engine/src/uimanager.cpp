#include "uimanager.hpp"
// imgui includes for oprngl and glfw
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>

#include <model.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>



void UIMANAGER::applyStyle(){
    ImGuiStyle& style = ImGui::GetStyle() ;
    ImVec4* colors = style.Colors ;

    style.WindowPadding = ImVec2(16.0f , 16.0f) ;
    style.FramePadding = ImVec2(10.0f , 6.0f) ;
    style.ItemSpacing = ImVec2(10.0f , 8.0f) ;
    style.ItemInnerSpacing = ImVec2(8.0f , 6.0f) ;
    style.WindowRounding = 0.0f ;
    style.ChildRounding = 8.0f ;
    style.FrameRounding = 6.0f ;
    style.PopupRounding = 6.0f ;
    style.ScrollbarRounding = 8.0f ;
    style.GrabRounding = 6.0f ;
    style.WindowBorderSize = 0.0f ;
    style.ChildBorderSize = 1.0f ;
    style.FrameBorderSize = 0.0f ;
    style.PopupBorderSize = 0.0f ;

    const ImVec4 accent(0.36f , 0.56f , 0.92f , 1.00f) ;
    const ImVec4 accentHover(0.43f , 0.63f , 0.98f , 1.00f) ;
    const ImVec4 accentActive(0.28f , 0.47f , 0.82f , 1.00f) ;

    colors[ImGuiCol_Text] = ImVec4(0.90f , 0.92f , 0.95f , 1.00f) ;
    colors[ImGuiCol_TextDisabled] = ImVec4(0.52f , 0.56f , 0.62f , 1.00f) ;
    colors[ImGuiCol_WindowBg] = ImVec4(0.075f , 0.085f , 0.105f , 1.00f) ;
    colors[ImGuiCol_ChildBg] = ImVec4(0.095f , 0.105f , 0.128f , 1.00f) ;
    colors[ImGuiCol_PopupBg] = ImVec4(0.095f , 0.105f , 0.128f , 0.98f) ;
    colors[ImGuiCol_Border] = ImVec4(0.20f , 0.23f , 0.28f , 0.80f) ;
    colors[ImGuiCol_FrameBg] = ImVec4(0.13f , 0.15f , 0.18f , 1.00f) ;
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f , 0.21f , 0.26f , 1.00f) ;
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.21f , 0.25f , 0.31f , 1.00f) ;
    colors[ImGuiCol_TitleBg] = ImVec4(0.075f , 0.085f , 0.105f , 1.00f) ;
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f , 0.12f , 0.15f , 1.00f) ;
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.095f , 0.105f , 0.128f , 1.00f) ;
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.075f , 0.085f , 0.105f , 1.00f) ;
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f , 0.28f , 0.34f , 1.00f) ;
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.31f , 0.36f , 0.43f , 1.00f) ;
    colors[ImGuiCol_ScrollbarGrabActive] = accent ;
    colors[ImGuiCol_CheckMark] = accent ;
    colors[ImGuiCol_SliderGrab] = accent ;
    colors[ImGuiCol_SliderGrabActive] = accentHover ;
    colors[ImGuiCol_Button] = ImVec4(0.18f , 0.21f , 0.26f , 1.00f) ;
    colors[ImGuiCol_ButtonHovered] = accentHover ;
    colors[ImGuiCol_ButtonActive] = accentActive ;
    colors[ImGuiCol_Header] = ImVec4(0.36f , 0.56f , 0.92f , 0.24f) ;
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f , 0.56f , 0.92f , 0.38f) ;
    colors[ImGuiCol_HeaderActive] = ImVec4(0.36f , 0.56f , 0.92f , 0.50f) ;
    colors[ImGuiCol_Separator] = ImVec4(0.20f , 0.23f , 0.28f , 0.80f) ;
    colors[ImGuiCol_SeparatorHovered] = accent ;
    colors[ImGuiCol_SeparatorActive] = accentHover ;
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f , 0.56f , 0.92f , 0.20f) ;
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.36f , 0.56f , 0.92f , 0.45f) ;
    colors[ImGuiCol_ResizeGripActive] = accent ;
    colors[ImGuiCol_Tab] = ImVec4(0.10f , 0.12f , 0.15f , 1.00f) ;
    colors[ImGuiCol_TabHovered] = ImVec4(0.36f , 0.56f , 0.92f , 0.45f) ;
    colors[ImGuiCol_TabSelected] = ImVec4(0.18f , 0.21f , 0.26f , 1.00f) ;
    colors[ImGuiCol_TabDimmed] = ImVec4(0.09f , 0.10f , 0.12f , 1.00f) ;
    colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.14f , 0.16f , 0.20f , 1.00f) ;
    colors[ImGuiCol_DockingPreview] = ImVec4(0.36f , 0.56f , 0.92f , 0.55f) ;
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.36f , 0.56f , 0.92f , 0.35f) ;
    colors[ImGuiCol_NavCursor] = accent ;
}

// small gear icon drawn with primitives so no icon font is needed
static void drawGear(ImDrawList* draw , const ImVec2& center , float radius , ImU32 color){
    const int toothCount = 8 ;

    for(int index = 0 ; index < toothCount ; ++index){
        const float angle = static_cast<float>(index) / toothCount * 6.28318530718f ;
        const ImVec2 direction(std::cos(angle) , std::sin(angle)) ;
        const ImVec2 perpendicular(-direction.y , direction.x) ;

        const float inner = radius * 0.70f ;
        const float outer = radius * 1.14f ;
        const float halfWidth = radius * 0.19f ;

        draw->AddQuadFilled(
            ImVec2(center.x + direction.x * inner + perpendicular.x * halfWidth , center.y + direction.y * inner + perpendicular.y * halfWidth) ,
            ImVec2(center.x + direction.x * inner - perpendicular.x * halfWidth , center.y + direction.y * inner - perpendicular.y * halfWidth) ,
            ImVec2(center.x + direction.x * outer - perpendicular.x * halfWidth , center.y + direction.y * outer - perpendicular.y * halfWidth) ,
            ImVec2(center.x + direction.x * outer + perpendicular.x * halfWidth , center.y + direction.y * outer + perpendicular.y * halfWidth) ,
            color
        ) ;
    }

    draw->AddCircleFilled(center , radius * 0.82f , color) ;
    draw->AddCircleFilled(center , radius * 0.34f , IM_COL32(15 , 18 , 24 , 255)) ;
}

std::string UIMANAGER::browseForModel(){
#ifdef __APPLE__
    const char* command =
        "osascript -e 'POSIX path of (choose file with prompt \"Select a 3D model\" "
        "of type {\"obj\", \"glb\", \"gltf\", \"fbx\", \"dae\", \"stl\"})'" ;

    FILE* pipe = popen(command , "r") ;
    if(!pipe){
        return {} ;
    }

    std::string path ;
    char buffer[4096] ;

    while(fgets(buffer , sizeof(buffer) , pipe)){
        path += buffer ;
    }

    const int status = pclose(pipe) ;

    if(status != 0){
        return {} ;
    }

    while(!path.empty() && (path.back() == '\n' || path.back() == '\r')){
        path.pop_back() ;
    }

    return path ;
#else
    return {} ;
#endif
}

void UIMANAGER::loadModelFromPath(
    const std::string& path ,
    Model*& currentModel ,
    glm::mat4 &model
){
    if(path.empty()){
        statusMessage = "No model selected." ;
        statusIsError = true ;
        return ;
    }

    Model* nextModel = new Model(path) ;

    if(nextModel->isEmpty()){
        delete nextModel ;
        statusMessage = "Failed to load " + std::filesystem::path(path).filename().string() ;
        statusIsError = true ;
        return ;
    }

    delete currentModel ;
    currentModel = nextModel ;
    currentModelPath = path ;
    currentModelName = std::filesystem::path(path).filename().string() ;

    if(currentModelName.empty()){
        currentModelName = path ;
    }

    model = initialModel ;
    statusMessage = "Loaded " + currentModelName ;
    statusIsError = false ;
}

void UIMANAGER::uiInit(GLFWwindow* window){
    IMGUI_CHECKVERSION() ;
    ImGui::CreateContext() ;

    ImGuiIO& io = ImGui::GetIO() ;
    io.IniFilename = nullptr ;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard ;

    applyStyle() ;

    ImGui_ImplGlfw_InitForOpenGL(window , true) ;
    ImGui_ImplOpenGL3_Init("#version 410") ;
}

void UIMANAGER::render(Camera& camera,
    glm::mat4& model,
    bool& shouldRotate,
    glm::vec3& sunPosition,
    glm::vec3& lightColor,
    Model*& currentModel
) {
    ImGui_ImplOpenGL3_NewFrame() ;
    ImGui_ImplGlfw_NewFrame() ;
    ImGui::NewFrame() ;

    if(!hasInitialModel){
        initialModel = model ;
        hasInitialModel = true ;
    }

    ImGuiIO& io = ImGui::GetIO() ;
    const ImVec2 display = io.DisplaySize ;

    const ImVec4 accent(0.36f , 0.56f , 0.92f , 1.00f) ;
    const ImVec4 accentHover(0.43f , 0.63f , 0.98f , 1.00f) ;
    const ImVec4 accentActive(0.28f , 0.47f , 0.82f , 1.00f) ;
    const ImVec4 headerBg(0.060f , 0.070f , 0.090f , 1.00f) ;
    const ImVec4 toolbarBg(0.080f , 0.092f , 0.115f , 1.00f) ;
    const ImVec4 panelBg(0.075f , 0.085f , 0.105f , 1.00f) ;

    const float headerHeight = 52.0f ;
    const float toolbarHeight = 46.0f ;
    const float leftPanelWidth = std::min(224.0f , display.x * 0.20f) ;
    const float inspectorWidth = std::min(300.0f , display.x * 0.26f) ;
    const float top = headerHeight + toolbarHeight ;
    const float bottom = display.y ;

    const ImGuiWindowFlags barFlags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse ;

    const ImGuiWindowFlags panelFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus ;

    auto placeWindow = [](const ImVec2& position , const ImVec2& size){
        ImGui::SetNextWindowPos(position , ImGuiCond_Always) ;
        ImGui::SetNextWindowSize(size , ImGuiCond_Always) ;
    } ;

    // header
    ImGui::PushStyleColor(ImGuiCol_WindowBg , headerBg) ;
    placeWindow(ImVec2(0.0f , 0.0f) , ImVec2(display.x , headerHeight)) ;
    if(ImGui::Begin("##header" , nullptr , barFlags)){
        const ImVec2 headerPos = ImGui::GetWindowPos() ;
        const ImVec2 headerSize = ImGui::GetWindowSize() ;

        ImGui::SetCursorPosY((headerHeight - ImGui::GetTextLineHeight()) * 0.5f) ;
        ImGui::TextColored(accent , "ArchRender") ;
        ImGui::SameLine() ;
        ImGui::TextDisabled("3D Architecture Renderer") ;

        const float gearSize = 30.0f ;
        const float gearX = headerPos.x + headerSize.x - 16.0f - gearSize ;
        const float gearY = headerPos.y + (headerHeight - gearSize) * 0.5f ;

        char fpsLabel[32] ;
        snprintf(fpsLabel , sizeof(fpsLabel) , "%.0f FPS" , io.Framerate) ;
        const float fpsWidth = ImGui::CalcTextSize(fpsLabel).x ;
        ImGui::SetCursorScreenPos(
            ImVec2(
                gearX - 16.0f - fpsWidth ,
                headerPos.y + (headerHeight - ImGui::GetTextLineHeight()) * 0.5f
            )
        ) ;
        ImGui::TextDisabled("%s" , fpsLabel) ;

        ImGui::SetCursorScreenPos(ImVec2(gearX , gearY)) ;
        ImGui::InvisibleButton("##settings-toggle" , ImVec2(gearSize , gearSize)) ;

        const bool gearHovered = ImGui::IsItemHovered() ;
        if(ImGui::IsItemClicked()){
            showInspector = !showInspector ;
        }

        drawGear(
            ImGui::GetWindowDrawList() ,
            ImVec2(gearX + gearSize * 0.5f , gearY + gearSize * 0.5f) ,
            8.5f ,
            (gearHovered || showInspector)
                ? IM_COL32(122 , 152 , 235 , 255)
                : IM_COL32(158 , 168 , 184 , 255)
        ) ;

        if(gearHovered){
            ImGui::SetTooltip(showInspector ? "Hide inspector" : "Show inspector") ;
        }
    }
    ImGui::End() ;
    ImGui::PopStyleColor() ;

    // toolbar
    ImGui::PushStyleColor(ImGuiCol_WindowBg , toolbarBg) ;
    placeWindow(ImVec2(0.0f , headerHeight) , ImVec2(display.x , toolbarHeight)) ;
    if(ImGui::Begin("##toolbar" , nullptr , barFlags)){
        ImGui::SetCursorPosY((toolbarHeight - ImGui::GetFrameHeight()) * 0.5f) ;

        ImGui::PushStyleColor(ImGuiCol_Button , accent) ;
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered , accentHover) ;
        ImGui::PushStyleColor(ImGuiCol_ButtonActive , accentActive) ;

        if(ImGui::Button("Load Model")){
            loadModelFromPath(browseForModel() , currentModel , model) ;
        }

        ImGui::PopStyleColor(3) ;
        ImGui::SameLine() ;

        ImGui::BeginDisabled(currentModelPath.empty()) ;
        if(ImGui::Button("Reload")){
            loadModelFromPath(currentModelPath , currentModel , model) ;
        }
        ImGui::EndDisabled() ;

        ImGui::SameLine() ;

        if(ImGui::Button("Reset Transform")){
            model = initialModel ;
            statusMessage = "Transform reset." ;
            statusIsError = false ;
        }

        ImGui::SameLine(std::max(520.0f , display.x - 240.0f)) ;
        ImGui::TextDisabled("Tab toggles mouse capture") ;
    }
    ImGui::End() ;
    ImGui::PopStyleColor() ;

    // left panel
    ImGui::PushStyleColor(ImGuiCol_WindowBg , panelBg) ;
    placeWindow(
        ImVec2(0.0f , top) ,
        ImVec2(leftPanelWidth , std::max(1.0f , bottom - top))
    ) ;
    if(ImGui::Begin("##left-panel" , nullptr , panelFlags)){
        ImGui::TextColored(accent , "MODEL") ;
        ImGui::Separator() ;

        ImGui::TextDisabled("Current file") ;
        ImGui::TextWrapped("%s" , currentModelName.c_str()) ;

        ImGui::Spacing() ;

        ImGui::PushStyleColor(ImGuiCol_Button , accent) ;
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered , accentHover) ;
        ImGui::PushStyleColor(ImGuiCol_ButtonActive , accentActive) ;

        if(ImGui::Button("Browse model..." , ImVec2(-1.0f , 0.0f))){
            loadModelFromPath(browseForModel() , currentModel , model) ;
        }

        ImGui::PopStyleColor(3) ;

        ImGui::BeginDisabled(currentModelPath.empty()) ;
        if(ImGui::Button("Reload current" , ImVec2(-1.0f , 0.0f))){
            loadModelFromPath(currentModelPath , currentModel , model) ;
        }
        ImGui::EndDisabled() ;

        ImGui::Spacing() ;
        ImGui::TextDisabled("Supported formats") ;
        ImGui::TextWrapped(".obj  .glb  .gltf  .fbx  .dae  .stl") ;

        ImGui::Spacing() ;

        const ImVec4 statusColor = statusIsError
            ? ImVec4(0.95f , 0.45f , 0.45f , 1.00f)
            : ImVec4(0.55f , 0.80f , 0.62f , 1.00f) ;

        ImGui::PushStyleColor(ImGuiCol_Text , statusColor) ;
        ImGui::TextWrapped("%s" , statusMessage.c_str()) ;
        ImGui::PopStyleColor() ;
    }
    ImGui::End() ;
    ImGui::PopStyleColor() ;

    // right inspector, toggled from the gear icon
    if(showInspector){
        ImGui::PushStyleColor(ImGuiCol_WindowBg , panelBg) ;
        placeWindow(
            ImVec2(display.x - inspectorWidth , top) ,
            ImVec2(inspectorWidth , std::max(1.0f , bottom - top))
        ) ;
        if(ImGui::Begin("##right-panel" , nullptr , panelFlags)){
            ImGui::TextColored(accent , "INSPECTOR") ;
            ImGui::Separator() ;

            ImGui::SeparatorText("Transform") ;
            ImGui::Checkbox("Auto rotate" , &shouldRotate) ;

            ImGui::SeparatorText("Scene") ;

            ImGui::TextDisabled("Sun position") ;
            ImGui::SetNextItemWidth(-1.0f) ;
            ImGui::DragFloat3(
                "##sun-position" ,
                glm::value_ptr(sunPosition) ,
                1.0f ,
                -500.0f ,
                500.0f ,
                "%.1f"
            ) ;

            ImGui::TextDisabled("Light color") ;
            ImGui::SetNextItemWidth(-1.0f) ;
            ImGui::ColorEdit3("##light-color" , glm::value_ptr(lightColor)) ;

            ImGui::SeparatorText("Camera") ;

            ImGui::TextDisabled("Move speed") ;
            ImGui::SetNextItemWidth(-1.0f) ;
            ImGui::SliderFloat("##move-speed" , &camera.MovementSpeed , 0.1f , 10.0f , "%.2f") ;

            ImGui::TextDisabled("Mouse sensitivity") ;
            ImGui::SetNextItemWidth(-1.0f) ;
            ImGui::SliderFloat(
                "##mouse-sensitivity" ,
                &camera.MouseSensitivity ,
                0.01f ,
                0.2f ,
                "%.3f"
            ) ;

            ImGui::Text(
                "Position  %.1f, %.1f, %.1f" ,
                camera.Position.x ,
                camera.Position.y ,
                camera.Position.z
            ) ;

            if(ImGui::CollapsingHeader("Controls")){
                ImGui::BulletText("WASD  Move camera") ;
                ImGui::BulletText("Q / E  Move up / down") ;
                ImGui::BulletText("Mouse  Look around") ;
                ImGui::BulletText("F  Toggle auto-rotate") ;
                ImGui::BulletText("I J K L  Move model") ;
                ImGui::BulletText("Space  Reset model") ;
                ImGui::BulletText("Tab  Cursor mode") ;
            }
        }
        ImGui::End() ;
        ImGui::PopStyleColor() ;
    }

    // viewport border and label
    const ImVec2 viewportMin(leftPanelWidth , top) ;
    const ImVec2 viewportMax(
        showInspector ? display.x - inspectorWidth : display.x ,
        bottom
    ) ;

    if(viewportMax.x > viewportMin.x && viewportMax.y > viewportMin.y){
        ImDrawList* draw = ImGui::GetForegroundDrawList() ;
        draw->AddRect(
            viewportMin ,
            viewportMax ,
            IM_COL32(92 , 112 , 145 , 255) ,
            0.0f ,
            0 ,
            1.0f
        ) ;

        char label[96] ;
        snprintf(label , sizeof(label) , "Viewport   %.0f FPS" , io.Framerate) ;

        const ImVec2 labelSize = ImGui::CalcTextSize(label) ;
        const ImVec2 labelMin(viewportMin.x + 14.0f , viewportMin.y + 14.0f) ;
        const ImVec2 labelMax(
            labelMin.x + labelSize.x + 20.0f ,
            labelMin.y + labelSize.y + 12.0f
        ) ;

        draw->AddRectFilled(
            labelMin ,
            labelMax ,
            IM_COL32(10 , 12 , 16 , 190) ,
            6.0f
        ) ;
        draw->AddText(
            ImVec2(labelMin.x + 10.0f , labelMin.y + 6.0f) ,
            IM_COL32(220 , 228 , 240 , 255) ,
            label
        ) ;
    }

    ImGui::Render() ;
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()) ;

    static int frameCount = 0 ;
    ++frameCount ;
    if(frameCount > 100 && frameCount % 60 == 0){
        GLFWwindow* currentWindow = glfwGetCurrentContext() ;
        int width = 0 ;
        int height = 0 ;
        glfwGetFramebufferSize(currentWindow , &width , &height) ;

        if(width > 0 && height > 0){
            std::vector<unsigned char> pixels(static_cast<size_t>(width) * height * 3) ;
            glPixelStorei(GL_PACK_ALIGNMENT , 1) ;
            glReadBuffer(GL_BACK) ;
            glReadPixels(0 , 0 , width , height , GL_RGB , GL_UNSIGNED_BYTE , pixels.data()) ;

            std::ofstream out(
                "/private/var/folders/wj/7l89zbkn4l57n1yblm11ysgm0000gn/T/ui_latest.ppm" ,
                std::ios::binary
            ) ;
            out << "P6\n" << width << " " << height << "\n255\n" ;
            for(int row = height - 1 ; row >= 0 ; --row){
                out.write(
                    reinterpret_cast<const char*>(pixels.data() + static_cast<size_t>(row) * width * 3) ,
                    width * 3
                ) ;
            }
        }
    }
}

void UIMANAGER::shutdown(){
    ImGui_ImplOpenGL3_Shutdown() ;
    ImGui_ImplGlfw_Shutdown() ;
    ImGui::DestroyContext() ;
}
