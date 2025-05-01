module;

export import Easy.ImGui.ImGui;
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

export module Easy.Platform.Impl.OpenGL.ImGui;
export import Easy.ImGui.ImGui;

export {
    using ::ImGui_ImplGlfw_InitForOpenGL;
    using ::ImGui_ImplGlfw_InitForVulkan;
    using ::ImGui_ImplGlfw_InitForOther;
    using ::ImGui_ImplGlfw_Shutdown;
    using ::ImGui_ImplGlfw_NewFrame;

    using ::ImGui_ImplGlfw_InstallCallbacks;
    using ::ImGui_ImplGlfw_RestoreCallbacks;

    using ::ImGui_ImplGlfw_SetCallbacksChainForAllWindows;

    using ::ImGui_ImplGlfw_WindowFocusCallback;
    using ::ImGui_ImplGlfw_CursorEnterCallback;
    using ::ImGui_ImplGlfw_CursorPosCallback;
    using ::ImGui_ImplGlfw_MouseButtonCallback;
    using ::ImGui_ImplGlfw_ScrollCallback;
    using ::ImGui_ImplGlfw_KeyCallback;
    using ::ImGui_ImplGlfw_CharCallback;
    using ::ImGui_ImplGlfw_MonitorCallback;
    using ::ImGui_ImplGlfw_Sleep;
}

export  {
    using ::ImGui_ImplOpenGL3_Init;
    using ::ImGui_ImplOpenGL3_Shutdown;
    using ::ImGui_ImplOpenGL3_NewFrame;
    using ::ImGui_ImplOpenGL3_RenderDrawData;

    using ::ImGui_ImplOpenGL3_CreateFontsTexture;
    using ::ImGui_ImplOpenGL3_DestroyFontsTexture;
    using ::ImGui_ImplOpenGL3_CreateDeviceObjects;
    using ::ImGui_ImplOpenGL3_DestroyDeviceObjects;
}