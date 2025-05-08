module;
#include "imgui_nostatic.h"
#include <imgui_internal.h>

export module Easy.ImGui.ImGui;

import Easy.Core.Basic;

constexpr const char *ImGui_Version = IMGUI_VERSION;
constexpr int ImGui_Version_Num = IMGUI_VERSION_NUM;

#undef IMGUI_VERSION
#undef IMGUI_VERSION_NUM

export const char *IMGUI_VERSION = ImGui_Version;
export int IMGUI_VERSION_NUM = ImGui_Version_Num;

export {
    using ::ImGuiID;
    using ::ImS8;
    using ::ImU8;
    using ::ImS16;
    using ::ImU16;
    using ::ImS32;
    using ::ImU32;
    using ::ImS64;
    using ::ImU64;

    using ::ImDrawChannel;
    using ::ImDrawCmd;
    using ::ImDrawData;
    using ::ImDrawList;
    using ::ImDrawListSharedData;
    using ::ImDrawListSplitter;
    using ::ImDrawVert;
    using ::ImFont;
    using ::ImFontAtlas;
    using ::ImFontBuilderIO;
    using ::ImFontConfig;
    using ::ImFontGlyph;
    using ::ImFontGlyphRangesBuilder;
    using ::ImColor;

    using ::ImGuiContext;
    using ::ImGuiIO;
    using ::ImGuiInputTextCallbackData;
    using ::ImGuiKeyData;
    using ::ImGuiListClipper;
    using ::ImGuiMultiSelectIO;
    using ::ImGuiOnceUponAFrame;
    using ::ImGuiPayload;
    using ::ImGuiPlatformIO;
    using ::ImGuiPlatformImeData;
    using ::ImGuiPlatformMonitor;
    using ::ImGuiSelectionBasicStorage;
    using ::ImGuiSelectionExternalStorage;
    using ::ImGuiSelectionRequest;
    using ::ImGuiSizeCallbackData;
    using ::ImGuiStorage;
    using ::ImGuiStoragePair;
    using ::ImGuiStyle;
    using ::ImGuiTableSortSpecs;
    using ::ImGuiTableColumnSortSpecs;
    using ::ImGuiTextBuffer;
    using ::ImGuiTextFilter;
    using ::ImGuiViewport;
    using ::ImGuiWindowClass;

    using ::ImGuiDir;
    using ::ImGuiKey;
    using ::ImGuiMouseSource;
    using ::ImGuiSortDirection;

    using ::ImGuiCol;
    using ::ImGuiCond;
    using ::ImGuiDataType;
    using ::ImGuiMouseButton;
    using ::ImGuiMouseCursor;
    using ::ImGuiStyleVar;
    using ::ImGuiTableBgTarget;

    using ::ImDrawFlags;
    using ::ImDrawListFlags;
    using ::ImFontAtlasFlags;
    using ::ImGuiBackendFlags;
    using ::ImGuiButtonFlags;
    using ::ImGuiChildFlags;
    using ::ImGuiColorEditFlags;
    using ::ImGuiConfigFlags;
    using ::ImGuiComboFlags;
    using ::ImGuiDockNodeFlags;
    using ::ImGuiDragDropFlags;
    using ::ImGuiFocusedFlags;
    using ::ImGuiHoveredFlags;
    using ::ImGuiInputFlags;
    using ::ImGuiInputTextFlags;
    using ::ImGuiItemFlags;
    using ::ImGuiKeyChord;
    using ::ImGuiPopupFlags;
    using ::ImGuiMultiSelectFlags;
    using ::ImGuiSelectableFlags;
    using ::ImGuiSliderFlags;
    using ::ImGuiTabBarFlags;
    using ::ImGuiTabItemFlags;
    using ::ImGuiTableFlags;
    using ::ImGuiTableColumnFlags;
    using ::ImGuiTableRowFlags;
    using ::ImGuiTreeNodeFlags;
    using ::ImGuiViewportFlags;
    using ::ImGuiWindowFlags;

    using ::ImWchar32;
    using ::ImWchar16;

    using ::ImWchar;

    using ::ImGuiSelectionUserData;

    using ::ImGuiInputTextCallback;
    using ::ImGuiSizeCallback;
    using ::ImGuiMemAllocFunc;
    using ::ImGuiMemFreeFunc;

    using ::ImVec2;
    using ::ImVec4;
    using ::ImTextureID;
}

export namespace ImGui {
    using ImGui::CreateContext;
    using ImGui::DestroyContext;
    using ImGui::GetCurrentContext;
    using ImGui::SetCurrentContext;

    using ImGui::GetIO;
    using ImGui::GetPlatformIO;
    using ImGui::GetStyle;
    using ImGui::NewFrame;
    using ImGui::EndFrame;
    using ImGui::Render;
    using ImGui::GetDrawData;

    using ImGui::ShowDemoWindow;
    using ImGui::ShowMetricsWindow;
    using ImGui::ShowDebugLogWindow;
    using ImGui::ShowIDStackToolWindow;
    using ImGui::ShowAboutWindow;
    using ImGui::ShowStyleEditor;
    using ImGui::ShowStyleSelector;
    using ImGui::ShowFontSelector;
    using ImGui::ShowUserGuide;
    using ImGui::GetVersion;

    using ImGui::StyleColorsDark;
    using ImGui::StyleColorsLight;
    using ImGui::StyleColorsClassic;

    using ImGui::Begin;
    using ImGui::End;

    using ImGui::BeginChild;
    using ImGui::EndChild;

    using ImGui::IsWindowAppearing;
    using ImGui::IsWindowCollapsed;
    using ImGui::IsWindowFocused;
    using ImGui::IsWindowHovered;
    using ImGui::GetWindowDrawList;
    using ImGui::GetWindowDpiScale;
    using ImGui::GetWindowPos;
    using ImGui::GetWindowSize;
    using ImGui::GetWindowWidth;
    using ImGui::GetWindowHeight;
    using ImGui::GetWindowViewport;

    using ImGui::SetNextWindowPos;
    using ImGui::SetNextWindowSize;
    using ImGui::SetNextWindowSizeConstraints;
    using ImGui::SetNextWindowContentSize;
    using ImGui::SetNextWindowCollapsed;
    using ImGui::SetNextWindowFocus;
    using ImGui::SetNextWindowScroll;
    using ImGui::SetNextWindowBgAlpha;
    using ImGui::SetNextWindowViewport;
    using ImGui::SetWindowPos;
    using ImGui::SetWindowSize;
    using ImGui::SetWindowCollapsed;
    using ImGui::SetWindowFocus;
    using ImGui::SetWindowFontScale;
    using ImGui::SetWindowPos;
    using ImGui::SetWindowSize;
    using ImGui::SetWindowCollapsed;
    using ImGui::SetWindowFocus;

    using ImGui::GetScrollX;
    using ImGui::GetScrollY;
    using ImGui::SetScrollX;
    using ImGui::SetScrollY;
    using ImGui::GetScrollMaxX;
    using ImGui::GetScrollMaxY;
    using ImGui::SetScrollHereX;
    using ImGui::SetScrollHereY;
    using ImGui::SetScrollFromPosX;
    using ImGui::SetScrollFromPosY;

    using ImGui::PushFont;
    using ImGui::PopFont;
    using ImGui::PushStyleColor;
    using ImGui::PushStyleColor;
    using ImGui::PopStyleColor;
    using ImGui::PushStyleVar;
    using ImGui::PushStyleVar;
    using ImGui::PushStyleVarX;
    using ImGui::PushStyleVarY;
    using ImGui::PopStyleVar;
    using ImGui::PushItemFlag;
    using ImGui::PopItemFlag;

    using ImGui::PushItemWidth;
    using ImGui::PopItemWidth;
    using ImGui::SetNextItemWidth;
    using ImGui::CalcItemWidth;
    using ImGui::PushTextWrapPos;
    using ImGui::PopTextWrapPos;

    using ImGui::GetFont;
    using ImGui::GetFontSize;
    using ImGui::GetFontTexUvWhitePixel;
    using ImGui::GetColorU32;
    using ImGui::GetStyleColorVec4;

    using ImGui::GetCursorScreenPos;
    using ImGui::GetContentRegionAvail;
    using ImGui::GetCursorPos;
    using ImGui::GetCursorPosX;
    using ImGui::GetCursorPosY;
    using ImGui::SetCursorPos;
    using ImGui::SetCursorPosX;
    using ImGui::SetCursorPosY;
    using ImGui::GetCursorStartPos;

    using ImGui::Separator;
    using ImGui::SameLine;
    using ImGui::NewLine;
    using ImGui::Spacing;
    using ImGui::Dummy;
    using ImGui::Indent;
    using ImGui::Unindent;
    using ImGui::BeginGroup;
    using ImGui::EndGroup;
    using ImGui::AlignTextToFramePadding;
    using ImGui::GetTextLineHeight;
    using ImGui::GetTextLineHeightWithSpacing;
    using ImGui::GetFrameHeight;
    using ImGui::GetFrameHeightWithSpacing;

    using ImGui::PushID;
    using ImGui::PopID;
    using ImGui::GetID;

    using ImGui::TextUnformatted;
    using ImGui::Text;
    using ImGui::TextV;
    using ImGui::TextColored;
    using ImGui::TextColoredV;
    using ImGui::TextDisabled;
    using ImGui::TextDisabledV;
    using ImGui::TextWrapped;
    using ImGui::TextWrappedV;
    using ImGui::LabelText;
    using ImGui::LabelTextV;
    using ImGui::BulletText;
    using ImGui::BulletTextV;
    using ImGui::SeparatorText;

    using ImGui::Button;
    using ImGui::SmallButton;
    using ImGui::InvisibleButton;
    using ImGui::ArrowButton;
    using ImGui::Checkbox;
    using ImGui::CheckboxFlags;
    using ImGui::RadioButton;
    using ImGui::ProgressBar;
    using ImGui::Bullet;
    using ImGui::TextLink;
    using ImGui::TextLinkOpenURL;

    using ImGui::Image;
    using ImGui::ImageWithBg;
    using ImGui::ImageButton;

    using ImGui::BeginCombo;
    using ImGui::EndCombo;
    using ImGui::Combo;

    using ImGui::DragFloat;
    using ImGui::DragFloat2;
    using ImGui::DragFloat3;
    using ImGui::DragFloat4;
    using ImGui::DragFloatRange2;
    using ImGui::DragInt;
    using ImGui::DragInt2;
    using ImGui::DragInt3;
    using ImGui::DragInt4;
    using ImGui::DragIntRange2;
    using ImGui::DragScalar;
    using ImGui::DragScalarN;

    using ImGui::SliderFloat;
    using ImGui::SliderFloat2;
    using ImGui::SliderFloat3;
    using ImGui::SliderFloat4;
    using ImGui::SliderAngle;
    using ImGui::SliderInt;
    using ImGui::SliderInt2;
    using ImGui::SliderInt3;
    using ImGui::SliderInt4;
    using ImGui::SliderScalar;
    using ImGui::VSliderFloat;
    using ImGui::VSliderInt;
    using ImGui::VSliderScalar;

    using ImGui::InputText;
    using ImGui::InputTextMultiline;
    using ImGui::InputTextWithHint;
    using ImGui::InputFloat;
    using ImGui::InputFloat2;
    using ImGui::InputFloat3;
    using ImGui::InputFloat4;
    using ImGui::InputInt;
    using ImGui::InputInt2;
    using ImGui::InputInt3;
    using ImGui::InputInt4;
    using ImGui::InputDouble;
    using ImGui::InputScalar;
    using ImGui::InputScalarN;

    using ImGui::ColorEdit3;
    using ImGui::ColorEdit4;
    using ImGui::ColorPicker3;
    using ImGui::ColorPicker4;
    using ImGui::ColorButton;
    using ImGui::SetColorEditOptions;

    using ImGui::TreeNode;
    using ImGui::TreeNodeEx;
    using ImGui::TreeNodeV;
    using ImGui::TreeNodeExV;
    using ImGui::TreePush;
    using ImGui::TreePop;

    using ImGui::GetTreeNodeToLabelSpacing;
    using ImGui::CollapsingHeader;
    using ImGui::CollapsingHeader;
    using ImGui::SetNextItemOpen;
    using ImGui::SetNextItemStorageID;

    using ImGui::Selectable;

    // multiselect
    using ImGui::BeginMultiSelect;
    using ImGui::EndMultiSelect;
    using ImGui::SetNextItemSelectionUserData;
    using ImGui::IsItemToggledSelection;

    using ImGui::BeginListBox;
    using ImGui::EndListBox;
    using ImGui::ListBox;

    using ImGui::PlotLines;
    using ImGui::PlotHistogram;

    using ImGui::Value;

    using ImGui::BeginMenuBar;
    using ImGui::EndMenuBar;
    using ImGui::BeginMainMenuBar;
    using ImGui::EndMainMenuBar;
    using ImGui::BeginMenu;
    using ImGui::EndMenu;
    using ImGui::MenuItem;

    using ImGui::BeginTooltip;
    using ImGui::EndTooltip;
    using ImGui::SetTooltip;
    using ImGui::SetTooltipV;

    using ImGui::BeginItemTooltip;
    using ImGui::SetItemTooltip;
    using ImGui::SetItemTooltipV;

    using ImGui::BeginPopup;
    using ImGui::BeginPopupModal;
    using ImGui::EndPopup;

    using ImGui::OpenPopup;
    using ImGui::OpenPopupOnItemClick;
    using ImGui::CloseCurrentPopup;

    using ImGui::BeginPopupContextItem;
    using ImGui::BeginPopupContextWindow;
    using ImGui::BeginPopupContextVoid;

    using ImGui::IsPopupOpen;

    using ImGui::BeginTable;
    using ImGui::EndTable;
    using ImGui::TableNextRow;
    using ImGui::TableNextColumn;
    using ImGui::TableSetColumnIndex;

    using ImGui::TableSetupColumn;
    using ImGui::TableSetupScrollFreeze;
    using ImGui::TableHeader;
    using ImGui::TableHeadersRow;
    using ImGui::TableAngledHeadersRow;

    using ImGui::TableGetSortSpecs;
    using ImGui::TableGetColumnCount;
    using ImGui::TableGetColumnIndex;
    using ImGui::TableGetRowIndex;
    using ImGui::TableGetColumnName;
    using ImGui::TableGetColumnFlags;
    using ImGui::TableSetColumnEnabled;
    using ImGui::TableGetHoveredColumn;
    using ImGui::TableSetBgColor;

    using ImGui::Columns;
    using ImGui::NextColumn;
    using ImGui::GetColumnIndex;
    using ImGui::GetColumnWidth;
    using ImGui::SetColumnWidth;
    using ImGui::GetColumnOffset;
    using ImGui::SetColumnOffset;
    using ImGui::GetColumnsCount;

    using ImGui::BeginTabBar;
    using ImGui::EndTabBar;
    using ImGui::BeginTabItem;
    using ImGui::EndTabItem;
    using ImGui::TabItemButton;
    using ImGui::SetTabItemClosed;

    using ImGui::DockSpace;
    using ImGui::DockSpaceOverViewport;
    using ImGui::SetNextWindowDockID;
    using ImGui::SetNextWindowClass;
    using ImGui::GetWindowDockID;
    using ImGui::IsWindowDocked;

    using ImGui::LogToTTY;
    using ImGui::LogToFile;
    using ImGui::LogToClipboard;
    using ImGui::LogFinish;
    using ImGui::LogButtons;
    using ImGui::LogText;
    using ImGui::LogTextV;

    using ImGui::BeginDragDropSource;
    using ImGui::SetDragDropPayload;
    using ImGui::EndDragDropSource;

    using ImGui::BeginDragDropTarget;
    using ImGui::AcceptDragDropPayload;
    using ImGui::EndDragDropTarget;
    using ImGui::GetDragDropPayload;

    using ImGui::BeginDisabled;
    using ImGui::EndDisabled;

    using ImGui::PushClipRect;
    using ImGui::PopClipRect;

    using ImGui::SetItemDefaultFocus;
    using ImGui::SetKeyboardFocusHere;

    using ImGui::SetNavCursorVisible;

    using ImGui::SetNextItemAllowOverlap;

    using ImGui::IsItemHovered;
    using ImGui::IsItemActive;
    using ImGui::IsItemFocused;
    using ImGui::IsItemClicked;
    using ImGui::IsItemVisible;
    using ImGui::IsItemEdited;
    using ImGui::IsItemActivated;
    using ImGui::IsItemDeactivated;
    using ImGui::IsItemDeactivatedAfterEdit;
    using ImGui::IsItemToggledOpen;
    using ImGui::IsAnyItemHovered;
    using ImGui::IsAnyItemActive;
    using ImGui::IsAnyItemFocused;
    using ImGui::GetItemID;
    using ImGui::GetItemRectMin;
    using ImGui::GetItemRectMax;
    using ImGui::GetItemRectSize;

    using ImGui::GetMainViewport;
    using ImGui::GetBackgroundDrawList;

    using ImGui::IsRectVisible;
    using ImGui::GetTime;
    using ImGui::GetFrameCount;
    using ImGui::GetDrawListSharedData;
    using ImGui::GetStyleColorName;
    using ImGui::SetStateStorage;

    using ImGui::CalcTextSize;

    using ImGui::ColorConvertU32ToFloat4;
    using ImGui::ColorConvertFloat4ToU32;
    using ImGui::ColorConvertRGBtoHSV;
    using ImGui::ColorConvertHSVtoRGB;

    using ImGui::IsKeyDown;
    using ImGui::IsKeyPressed;
    using ImGui::IsKeyReleased;
    using ImGui::IsKeyChordPressed;
    using ImGui::GetKeyPressedAmount;
    using ImGui::GetKeyName;
    using ImGui::SetNextFrameWantCaptureKeyboard;

    using ImGui::Shortcut;
    using ImGui::SetNextItemShortcut;

    using ImGui::SetItemKeyOwner;

    using ImGui::IsMouseDown;
    using ImGui::IsMouseClicked;
    using ImGui::IsMouseReleased;
    using ImGui::IsMouseDoubleClicked;
    using ImGui::IsMouseReleasedWithDelay;
    using ImGui::GetMouseClickedCount;
    using ImGui::IsMouseHoveringRect;
    using ImGui::IsMousePosValid;
    using ImGui::IsAnyMouseDown;
    using ImGui::GetMousePos;
    using ImGui::GetMousePosOnOpeningCurrentPopup;
    using ImGui::IsMouseDragging;
    using ImGui::GetMouseDragDelta;
    using ImGui::ResetMouseDragDelta;
    using ImGui::GetMouseCursor;
    using ImGui::SetMouseCursor;
    using ImGui::SetNextFrameWantCaptureMouse;

    using ImGui::GetClipboardText;
    using ImGui::SetClipboardText;

    using ImGui::LoadIniSettingsFromDisk;
    using ImGui::LoadIniSettingsFromMemory;
    using ImGui::SaveIniSettingsToDisk;
    using ImGui::SaveIniSettingsToMemory;

    using ImGui::DebugTextEncoding;
    using ImGui::DebugFlashStyleColor;
    using ImGui::DebugStartItemPicker;
    using ImGui::DebugCheckVersionAndDataLayout;
    using ImGui::DebugLog;
    using ImGui::DebugLogV;

    using ImGui::SetAllocatorFunctions;
    using ImGui::MemAlloc;
    using ImGui::MemFree;

    using ImGui::UpdatePlatformWindows;
    using ImGui::RenderPlatformWindowsDefault;
    using ImGui::DestroyPlatformWindows;
    using ImGui::FindViewportByID;
    using ImGui::FindViewportByPlatformHandle;
}

export {
    using ::ImGuiDir;
    using ::ImGuiSortDirection;
    using ::ImGuiKey;

    using ::ImGuiTableSortSpecs;
    using ::ImGuiTableColumnSortSpecs;
    using ::ImNewWrapper;
    using ::operator new;
    using ::operator delete;
    using ::IM_DELETE;

    using ::ImVector;
    using ::ImGuiStyle;
    using ::ImGuiKeyData;
    using ::ImGuiIO;
    using ::ImGuiInputTextCallbackData;
    using ::ImGuiSizeCallbackData;
    using ::ImGuiWindowClass;
    using ::ImGuiPayload;
}

constexpr uint32_t Im_Unicode_Codepoint_Invalid = IM_UNICODE_CODEPOINT_INVALID;
constexpr uint32_t Im_Unicode_Codepoint_Max = IM_UNICODE_CODEPOINT_MAX;

#undef IM_UNICODE_CODEPOINT_INVALID
#undef IM_UNICODE_CODEPOINT_MAX

export {
    constexpr uint32_t IM_UNICODE_CODEPOINT_INVALID = Im_Unicode_Codepoint_Invalid;
    constexpr uint32_t IM_UNICODE_CODEPOINT_MAX = Im_Unicode_Codepoint_Max;
}

export {
    using ::ImGuiOnceUponAFrame;
    using ::ImGuiTextFilter;
    using ::ImGuiTextBuffer;
    using ::ImGuiStoragePair;
    using ::ImGuiStorage;
    using ::ImGuiListClipper;

    inline ImVec2 operator*(const ImVec2 &lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
    inline ImVec2 operator/(const ImVec2 &lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
    inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
    inline ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
    inline ImVec2 operator*(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
    inline ImVec2 operator/(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
    inline ImVec2 operator-(const ImVec2 &lhs) { return ImVec2(-lhs.x, -lhs.y); }

    inline ImVec2 &operator*=(ImVec2 &lhs, const float rhs) {
        lhs.x *= rhs;
        lhs.y *= rhs;
        return lhs;
    }

    inline ImVec2 &operator/=(ImVec2 &lhs, const float rhs) {
        lhs.x /= rhs;
        lhs.y /= rhs;
        return lhs;
    }

    inline ImVec2 &operator+=(ImVec2 &lhs, const ImVec2 &rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    inline ImVec2 &operator-=(ImVec2 &lhs, const ImVec2 &rhs) {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    inline ImVec2 &operator*=(ImVec2 &lhs, const ImVec2 &rhs) {
        lhs.x *= rhs.x;
        lhs.y *= rhs.y;
        return lhs;
    }

    inline ImVec2 &operator/=(ImVec2 &lhs, const ImVec2 &rhs) {
        lhs.x /= rhs.x;
        lhs.y /= rhs.y;
        return lhs;
    }

    inline bool operator==(const ImVec2 &lhs, const ImVec2 &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    inline bool operator!=(const ImVec2 &lhs, const ImVec2 &rhs) { return lhs.x != rhs.x || lhs.y != rhs.y; }

    inline ImVec4 operator*(const ImVec4 &lhs, const float rhs) {
        return ImVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
    }

    inline ImVec4 operator/(const ImVec4 &lhs, const float rhs) {
        return ImVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
    }

    inline ImVec4 operator+(const ImVec4 &lhs, const ImVec4 &rhs) {
        return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }

    inline ImVec4 operator-(const ImVec4 &lhs, const ImVec4 &rhs) {
        return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }

    inline ImVec4 operator*(const ImVec4 &lhs, const ImVec4 &rhs) {
        return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
    }

    inline ImVec4 operator/(const ImVec4 &lhs, const ImVec4 &rhs) {
        return ImVec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
    }

    inline ImVec4 operator-(const ImVec4 &lhs) { return ImVec4(-lhs.x, -lhs.y, -lhs.z, -lhs.w); }

    inline bool operator==(const ImVec4 &lhs, const ImVec4 &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
    }

    inline bool operator!=(const ImVec4 &lhs, const ImVec4 &rhs) {
        return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
    }
}


constexpr uint32_t m_IM_COL32_R_SHIFT = IM_COL32_R_SHIFT;
constexpr uint32_t m_IM_COL32_G_SHIFT = IM_COL32_G_SHIFT;
constexpr uint32_t m_IM_COL32_B_SHIFT = IM_COL32_B_SHIFT;
constexpr uint32_t m_IM_COL32_A_SHIFT = IM_COL32_A_SHIFT;
constexpr uint32_t m_IM_COL32_A_MASK = IM_COL32_A_MASK;

#undef IM_COL32_R_SHIFT
#undef IM_COL32_G_SHIFT
#undef IM_COL32_B_SHIFT
#undef IM_COL32_A_SHIFT
#undef IM_COL32_A_MASK

#undef IM_COL32


export {
    constexpr uint32_t IM_COL32_R_SHIFT = m_IM_COL32_R_SHIFT;
    constexpr uint32_t IM_COL32_G_SHIFT = m_IM_COL32_G_SHIFT;
    constexpr uint32_t IM_COL32_B_SHIFT = m_IM_COL32_B_SHIFT;
    constexpr uint32_t IM_COL32_A_SHIFT = m_IM_COL32_A_SHIFT;
}

export constexpr uint32_t IM_COL32(uint32_t r, uint32_t g, uint32_t b, uint32_t a) {
    return (((ImU32) (a) << IM_COL32_A_SHIFT) | ((ImU32) (b) << IM_COL32_B_SHIFT) | ((ImU32) (g) << IM_COL32_G_SHIFT) |
            ((ImU32) (r) << IM_COL32_R_SHIFT));
}

constexpr uint32_t m_IM_COL32_WHITE = IM_COL32_WHITE;
constexpr uint32_t m_IM_COL32_BLACK = IM_COL32_BLACK;
constexpr uint32_t m_IM_COL32_BLACK_TRANS = IM_COL32_BLACK_TRANS;

#undef IM_COL32_WHITE
#undef IM_COL32_BLACK
#undef IM_COL32_BLACK_TRANS

export {
    constexpr uint32_t IM_COL32_WHITE = m_IM_COL32_WHITE;
    constexpr uint32_t IM_COL32_BLACK = m_IM_COL32_BLACK;
    constexpr uint32_t IM_COL32_BLACK_TRANS = m_IM_COL32_BLACK_TRANS;
}

export {
    using ::ImColor;
}

export {
    using ::ImGuiMultiSelectIO;
    using ::ImGuiSelectionRequestType;
    using ::ImGuiSelectionRequest;
    using ::ImGuiSelectionBasicStorage;
    using ::ImGuiSelectionExternalStorage;
}

constexpr uint32_t m_IM_DRAWLIST_TEX_LINES_WIDTH_MAX = IM_DRAWLIST_TEX_LINES_WIDTH_MAX;

#undef IM_DRAWLIST_TEX_LINES_WIDTH_MAX
export {
    constexpr uint32_t IM_DRAWLIST_TEX_LINES_WIDTH_MAX = m_IM_DRAWLIST_TEX_LINES_WIDTH_MAX;
}

export {
    using ::ImDrawIdx;
    using ::ImDrawCallback;
}

/*constexpr ImDrawCallback m_ImDrawCallback_ResetRenderState = reinterpret_cast<ImDrawCallback>(ImDrawCallback_ResetRenderState);
#undef ImDrawCallback_ResetRenderState
export {
    constexpr ImDrawCallback ImDrawCallback_ResetRenderState = m_ImDrawCallback_ResetRenderState;
}*/

export {
    using ::ImGuiWindowFlags_;
    using ::ImGuiWindowFlags_None ;
    using ::ImGuiWindowFlags_NoTitleBar ;
    using ::ImGuiWindowFlags_NoResize ;
    using ::ImGuiWindowFlags_NoMove ;
    using ::ImGuiWindowFlags_NoScrollbar ;
    using ::ImGuiWindowFlags_NoScrollWithMouse ;
    using ::ImGuiWindowFlags_NoCollapse ;
    using ::ImGuiWindowFlags_AlwaysAutoResize ;
    using ::ImGuiWindowFlags_NoBackground ;
    using ::ImGuiWindowFlags_NoSavedSettings ;
    using ::ImGuiWindowFlags_NoMouseInputs ;
    using ::ImGuiWindowFlags_MenuBar ;
    using ::ImGuiWindowFlags_HorizontalScrollbar ;
    using ::ImGuiWindowFlags_NoFocusOnAppearing ;
    using ::ImGuiWindowFlags_NoBringToFrontOnFocus ;
    using ::ImGuiWindowFlags_AlwaysVerticalScrollbar;
    using ::ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    using ::ImGuiWindowFlags_NoNavInputs ;
    using ::ImGuiWindowFlags_NoNavFocus ;
    using ::ImGuiWindowFlags_UnsavedDocument ;
    using ::ImGuiWindowFlags_NoDocking ;
    using ::ImGuiWindowFlags_NoNav ;
    using ::ImGuiWindowFlags_NoDecoration ;
    using ::ImGuiWindowFlags_NoInputs ;
    using ::ImGuiWindowFlags_DockNodeHost ;
    using ::ImGuiWindowFlags_ChildWindow ;
    using ::ImGuiWindowFlags_Tooltip ;
    using ::ImGuiWindowFlags_Popup ;
    using ::ImGuiWindowFlags_Modal ;
    using ::ImGuiWindowFlags_ChildMenu ;
    using ::ImGuiWindowFlags_NavFlattened ;
    using ::ImGuiWindowFlags_AlwaysUseWindowPadding ;

    using ::ImGuiChildFlags_;
    using ::ImGuiChildFlags_None ;
    using ::ImGuiChildFlags_Borders ;
    using ::ImGuiChildFlags_AlwaysUseWindowPadding ;
    using ::ImGuiChildFlags_ResizeX ;
    using ::ImGuiChildFlags_ResizeY ;
    using ::ImGuiChildFlags_AutoResizeX ;
    using ::ImGuiChildFlags_AutoResizeY ;
    using ::ImGuiChildFlags_AlwaysAutoResize ;
    using ::ImGuiChildFlags_FrameStyle ;
    using ::ImGuiChildFlags_NavFlattened ;

    using ::ImGuiItemFlags_;
    using ::ImGuiItemFlags_None ;
    using ::ImGuiItemFlags_NoTabStop ;
    using ::ImGuiItemFlags_NoNav ;
    using ::ImGuiItemFlags_NoNavDefaultFocus;
    using ::ImGuiItemFlags_ButtonRepeat ;
    using ::ImGuiItemFlags_AutoClosePopups ;
    using ::ImGuiItemFlags_AllowDuplicateId ;

    using ::ImGuiInputTextFlags_;
    using ::ImGuiInputTextFlags_None ;
    using ::ImGuiInputTextFlags_CharsDecimal ;
    using ::ImGuiInputTextFlags_CharsHexadecimal ;
    using ::ImGuiInputTextFlags_CharsScientific ;
    using ::ImGuiInputTextFlags_CharsUppercase ;
    using ::ImGuiInputTextFlags_CharsNoBlank ;
    using ::ImGuiInputTextFlags_AllowTabInput ;
    using ::ImGuiInputTextFlags_EnterReturnsTrue ;
    using ::ImGuiInputTextFlags_EscapeClearsAll ;
    using ::ImGuiInputTextFlags_CtrlEnterForNewLine ;
    using ::ImGuiInputTextFlags_ReadOnly ;
    using ::ImGuiInputTextFlags_Password ;
    using ::ImGuiInputTextFlags_AlwaysOverwrite ;
    using ::ImGuiInputTextFlags_AutoSelectAll ;
    using ::ImGuiInputTextFlags_ParseEmptyRefVal ;
    using ::ImGuiInputTextFlags_DisplayEmptyRefVal ;
    using ::ImGuiInputTextFlags_NoHorizontalScroll ;
    using ::ImGuiInputTextFlags_NoUndoRedo ;
    using ::ImGuiInputTextFlags_ElideLeft ;
    using ::ImGuiInputTextFlags_CallbackCompletion ;
    using ::ImGuiInputTextFlags_CallbackHistory ;
    using ::ImGuiInputTextFlags_CallbackAlways ;
    using ::ImGuiInputTextFlags_CallbackCharFilter ;
    using ::ImGuiInputTextFlags_CallbackResize ;
    using ::ImGuiInputTextFlags_CallbackEdit ;

    using ::ImGuiTreeNodeFlags_;

    using ::ImGuiTreeNodeFlags_None ;
    using ::ImGuiTreeNodeFlags_Selected ;
    using ::ImGuiTreeNodeFlags_Framed ;
    using ::ImGuiTreeNodeFlags_AllowOverlap ;
    using ::ImGuiTreeNodeFlags_NoTreePushOnOpen ;
    using ::ImGuiTreeNodeFlags_NoAutoOpenOnLog ;
    using ::ImGuiTreeNodeFlags_DefaultOpen ;
    using ::ImGuiTreeNodeFlags_OpenOnDoubleClick ;
    using ::ImGuiTreeNodeFlags_OpenOnArrow ;
    using ::ImGuiTreeNodeFlags_Leaf ;
    using ::ImGuiTreeNodeFlags_Bullet ;
    using ::ImGuiTreeNodeFlags_FramePadding ;
    using ::ImGuiTreeNodeFlags_SpanAvailWidth ;
    using ::ImGuiTreeNodeFlags_SpanFullWidth ;
    using ::ImGuiTreeNodeFlags_SpanLabelWidth ;
    using ::ImGuiTreeNodeFlags_SpanAllColumns ;
    using ::ImGuiTreeNodeFlags_LabelSpanAllColumns ;
    using ::ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
    using ::ImGuiTreeNodeFlags_CollapsingHeader ;
    using ::ImGuiTreeNodeFlags_DrawLinesNone ;
    using ::ImGuiTreeNodeFlags_DrawLinesFull ;
    using ::ImGuiTreeNodeFlags_DrawLinesToNodes ;
    using ::ImGuiTreeNodeFlags_AllowItemOverlap ;
    using ::ImGuiTreeNodeFlags_SpanTextWidth ;

    using ::ImGuiPopupFlags_;
    using ::ImGuiPopupFlags_None ;
    using ::ImGuiPopupFlags_MouseButtonLeft ;
    using ::ImGuiPopupFlags_MouseButtonRight ;
    using ::ImGuiPopupFlags_MouseButtonMiddle ;
    using ::ImGuiPopupFlags_MouseButtonMask_ ;
    using ::ImGuiPopupFlags_MouseButtonDefault_ ;
    using ::ImGuiPopupFlags_NoReopen ;
    using ::ImGuiPopupFlags_NoOpenOverExistingPopup;
    using ::ImGuiPopupFlags_NoOpenOverItems ;
    using ::ImGuiPopupFlags_AnyPopupId ;
    using ::ImGuiPopupFlags_AnyPopupLevel ;
    using ::ImGuiPopupFlags_AnyPopup ;

    using ::ImGuiSelectableFlags_;
    using ::ImGuiSelectableFlags_None ;
    using ::ImGuiSelectableFlags_NoAutoClosePopups;
    using ::ImGuiSelectableFlags_SpanAllColumns ;
    using ::ImGuiSelectableFlags_AllowDoubleClick ;
    using ::ImGuiSelectableFlags_Disabled ;
    using ::ImGuiSelectableFlags_AllowOverlap ;
    using ::ImGuiSelectableFlags_Highlight ;
    using ::ImGuiSelectableFlags_DontClosePopups ;
    using ::ImGuiSelectableFlags_AllowItemOverlap ;

    using ::ImGuiComboFlags_;
    using ::ImGuiComboFlags_None ;
    using ::ImGuiComboFlags_PopupAlignLeft ;
    using ::ImGuiComboFlags_HeightSmall ;
    using ::ImGuiComboFlags_HeightRegular ;
    using ::ImGuiComboFlags_HeightLarge ;
    using ::ImGuiComboFlags_HeightLargest ;
    using ::ImGuiComboFlags_NoArrowButton ;
    using ::ImGuiComboFlags_NoPreview ;
    using ::ImGuiComboFlags_WidthFitPreview ;
    using ::ImGuiComboFlags_HeightMask_ ;

    using ::ImGuiTabBarFlags_;
    using ::ImGuiTabBarFlags_None ;
    using ::ImGuiTabBarFlags_Reorderable ;
    using ::ImGuiTabBarFlags_AutoSelectNewTabs ;
    using ::ImGuiTabBarFlags_TabListPopupButton ;
    using ::ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
    using ::ImGuiTabBarFlags_NoTabListScrollingButtons ;
    using ::ImGuiTabBarFlags_NoTooltip ;
    using ::ImGuiTabBarFlags_DrawSelectedOverline ;
    using ::ImGuiTabBarFlags_FittingPolicyResizeDown ;
    using ::ImGuiTabBarFlags_FittingPolicyScroll ;
    using ::ImGuiTabBarFlags_FittingPolicyMask_ ;
    using ::ImGuiTabBarFlags_FittingPolicyDefault_ ;

    using ::ImGuiTabItemFlags_;

    using ::ImGuiTabItemFlags_None ;
    using ::ImGuiTabItemFlags_UnsavedDocument ;
    using ::ImGuiTabItemFlags_SetSelected ;
    using ::ImGuiTabItemFlags_NoCloseWithMiddleMouseButton ;
    using ::ImGuiTabItemFlags_NoPushId ;
    using ::ImGuiTabItemFlags_NoTooltip ;
    using ::ImGuiTabItemFlags_NoReorder ;
    using ::ImGuiTabItemFlags_Leading ;
    using ::ImGuiTabItemFlags_Trailing ;
    using ::ImGuiTabItemFlags_NoAssumedClosure ;

    using ::ImGuiFocusedFlags_;
    using ::ImGuiFocusedFlags_None ;
    using ::ImGuiFocusedFlags_ChildWindows ;
    using ::ImGuiFocusedFlags_RootWindow ;
    using ::ImGuiFocusedFlags_AnyWindow ;
    using ::ImGuiFocusedFlags_NoPopupHierarchy ;
    using ::ImGuiFocusedFlags_DockHierarchy ;
    using ::ImGuiFocusedFlags_RootAndChildWindows;

    using ::ImGuiHoveredFlags_;

    using ::ImGuiHoveredFlags_None ;
    using ::ImGuiHoveredFlags_ChildWindows ;
    using ::ImGuiHoveredFlags_RootWindow ;
    using ::ImGuiHoveredFlags_AnyWindow ;
    using ::ImGuiHoveredFlags_NoPopupHierarchy ;
    using ::ImGuiHoveredFlags_DockHierarchy ;
    using ::ImGuiHoveredFlags_AllowWhenBlockedByPopup ;
    using ::ImGuiHoveredFlags_AllowWhenBlockedByActiveItem;
    using ::ImGuiHoveredFlags_AllowWhenOverlappedByItem ;
    using ::ImGuiHoveredFlags_AllowWhenOverlappedByWindow ;
    using ::ImGuiHoveredFlags_AllowWhenDisabled ;
    using ::ImGuiHoveredFlags_NoNavOverride ;
    using ::ImGuiHoveredFlags_AllowWhenOverlapped ;
    using ::ImGuiHoveredFlags_RectOnly ;
    using ::ImGuiHoveredFlags_RootAndChildWindows ;
    using ::ImGuiHoveredFlags_ForTooltip ;
    using ::ImGuiHoveredFlags_Stationary ;
    using ::ImGuiHoveredFlags_DelayNone ;
    using ::ImGuiHoveredFlags_DelayShort ;
    using ::ImGuiHoveredFlags_DelayNormal ;
    using ::ImGuiHoveredFlags_NoSharedDelay;

    using ::ImGuiDockNodeFlags_;
    using ::ImGuiDockNodeFlags_None ;
    using ::ImGuiDockNodeFlags_KeepAliveOnly ;
    using ::ImGuiDockNodeFlags_NoDockingOverCentralNode;
    using ::ImGuiDockNodeFlags_PassthruCentralNode ;
    using ::ImGuiDockNodeFlags_NoDockingSplit ;
    using ::ImGuiDockNodeFlags_NoResize ;
    using ::ImGuiDockNodeFlags_AutoHideTabBar ;
    using ::ImGuiDockNodeFlags_NoUndocking ;
    using ::ImGuiDockNodeFlags_NoSplit ;
    using ::ImGuiDockNodeFlags_NoDockingInCentralNode ;

    using ::ImGuiDragDropFlags_;

    using ::ImGuiDragDropFlags_None ;
    using ::ImGuiDragDropFlags_SourceNoPreviewTooltip ;
    using ::ImGuiDragDropFlags_SourceNoDisableHover ;
    using ::ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
    using ::ImGuiDragDropFlags_SourceAllowNullID ;
    using ::ImGuiDragDropFlags_SourceExtern ;
    using ::ImGuiDragDropFlags_PayloadAutoExpire ;
    using ::ImGuiDragDropFlags_PayloadNoCrossContext ;
    using ::ImGuiDragDropFlags_PayloadNoCrossProcess ;
    using ::ImGuiDragDropFlags_AcceptBeforeDelivery ;
    using ::ImGuiDragDropFlags_AcceptNoDrawDefaultRect ;
    using ::ImGuiDragDropFlags_AcceptNoPreviewTooltip ;
    using ::ImGuiDragDropFlags_AcceptPeekOnly ;
    using ::ImGuiDragDropFlags_SourceAutoExpirePayload ;

    using ::ImGuiDataType_;
    using ::ImGuiDataType_S8;
    using ::ImGuiDataType_U8;
    using ::ImGuiDataType_S16;
    using ::ImGuiDataType_U16;
    using ::ImGuiDataType_S32;
    using ::ImGuiDataType_U32;
    using ::ImGuiDataType_S64;
    using ::ImGuiDataType_U64;
    using ::ImGuiDataType_Float;
    using ::ImGuiDataType_Double;
    using ::ImGuiDataType_Bool;
    using ::ImGuiDataType_String;
    using ::ImGuiDataType_COUNT;

    using ::ImGuiInputFlags_;
    using ::ImGuiInputFlags_None ;
    using ::ImGuiInputFlags_Repeat ;
    using ::ImGuiInputFlags_RouteActive ;
    using ::ImGuiInputFlags_RouteFocused ;
    using ::ImGuiInputFlags_RouteGlobal ;
    using ::ImGuiInputFlags_RouteAlways ;
    using ::ImGuiInputFlags_RouteOverFocused ;
    using ::ImGuiInputFlags_RouteOverActive ;
    using ::ImGuiInputFlags_RouteUnlessBgFocused;
    using ::ImGuiInputFlags_RouteFromRootWindow ;
    using ::ImGuiInputFlags_Tooltip ;

    using ::ImGuiConfigFlags_;
    using ::ImGuiConfigFlags_None ;
    using ::ImGuiConfigFlags_NavEnableKeyboard ;
    using ::ImGuiConfigFlags_NavEnableGamepad ;
    using ::ImGuiConfigFlags_NoMouse ;
    using ::ImGuiConfigFlags_NoMouseCursorChange ;
    using ::ImGuiConfigFlags_NoKeyboard ;
    using ::ImGuiConfigFlags_DockingEnable ;
    using ::ImGuiConfigFlags_ViewportsEnable ;
    using ::ImGuiConfigFlags_DpiEnableScaleViewports;
    using ::ImGuiConfigFlags_DpiEnableScaleFonts ;
    using ::ImGuiConfigFlags_IsSRGB ;
    using ::ImGuiConfigFlags_IsTouchScreen ;
    using ::ImGuiConfigFlags_NavEnableSetMousePos ;
    using ::ImGuiConfigFlags_NavNoCaptureKeyboard ;

    using ::ImGuiBackendFlags_;
    using ::ImGuiBackendFlags_None ;
    using ::ImGuiBackendFlags_HasGamepad ;
    using ::ImGuiBackendFlags_HasMouseCursors ;
    using ::ImGuiBackendFlags_HasSetMousePos ;
    using ::ImGuiBackendFlags_RendererHasVtxOffset ;
    using ::ImGuiBackendFlags_PlatformHasViewports ;
    using ::ImGuiBackendFlags_HasMouseHoveredViewport;
    using ::ImGuiBackendFlags_RendererHasViewports ;

    using ::ImGuiCol_;
    using ::ImGuiCol_Text;
    using ::ImGuiCol_TextDisabled;
    using ::ImGuiCol_WindowBg; // Background of normal wi
    using ::ImGuiCol_ChildBg; // Background of child win
    using ::ImGuiCol_PopupBg; // Background of popups; m
    using ::ImGuiCol_Border;
    using ::ImGuiCol_BorderShadow;
    using ::ImGuiCol_FrameBg; // Background of checkbox;
    using ::ImGuiCol_FrameBgHovered;
    using ::ImGuiCol_FrameBgActive;
    using ::ImGuiCol_TitleBg; // Title bar
    using ::ImGuiCol_TitleBgActive; // Title bar when focused
    using ::ImGuiCol_TitleBgCollapsed; // Title bar when collapse
    using ::ImGuiCol_MenuBarBg;
    using ::ImGuiCol_ScrollbarBg;
    using ::ImGuiCol_ScrollbarGrab;
    using ::ImGuiCol_ScrollbarGrabHovered;
    using ::ImGuiCol_ScrollbarGrabActive;
    using ::ImGuiCol_CheckMark; // Checkbox tick and Radio
    using ::ImGuiCol_SliderGrab;
    using ::ImGuiCol_SliderGrabActive;
    using ::ImGuiCol_Button;
    using ::ImGuiCol_ButtonHovered;
    using ::ImGuiCol_ButtonActive;
    using ::ImGuiCol_Header; // Header* colors are used
    using ::ImGuiCol_HeaderHovered;
    using ::ImGuiCol_HeaderActive;
    using ::ImGuiCol_Separator;
    using ::ImGuiCol_SeparatorHovered;
    using ::ImGuiCol_SeparatorActive;
    using ::ImGuiCol_ResizeGrip; // Resize grip in lower-ri
    using ::ImGuiCol_ResizeGripHovered;
    using ::ImGuiCol_ResizeGripActive;
    using ::ImGuiCol_InputTextCursor; // InputText cursor/caret
    using ::ImGuiCol_TabHovered; // Tab background; when ho
    using ::ImGuiCol_Tab; // Tab background; when ta
    using ::ImGuiCol_TabSelected; // Tab background; when ta
    using ::ImGuiCol_TabSelectedOverline; // Tab horizontal overline
    using ::ImGuiCol_TabDimmed; // Tab background; when ta
    using ::ImGuiCol_TabDimmedSelected; // Tab background; when ta
    using ::ImGuiCol_TabDimmedSelectedOverline; //..horizontal overline
    using ::ImGuiCol_DockingPreview; // Preview overlay color w
    using ::ImGuiCol_DockingEmptyBg; // Background color for em
    using ::ImGuiCol_PlotLines;
    using ::ImGuiCol_PlotLinesHovered;
    using ::ImGuiCol_PlotHistogram;
    using ::ImGuiCol_PlotHistogramHovered;
    using ::ImGuiCol_TableHeaderBg; // Table header background
    using ::ImGuiCol_TableBorderStrong; // Table outer and header 
    using ::ImGuiCol_TableBorderLight; // Table inner borders (pr
    using ::ImGuiCol_TableRowBg; // Table row background (e
    using ::ImGuiCol_TableRowBgAlt; // Table row background (o
    using ::ImGuiCol_TextLink; // Hyperlink color
    using ::ImGuiCol_TextSelectedBg; // Selected text inside an
    using ::ImGuiCol_TreeLines; // Tree node hierarchy out
    using ::ImGuiCol_DragDropTarget; // Rectangle highlighting 
    using ::ImGuiCol_NavCursor; // Color of keyboard/gamep
    using ::ImGuiCol_NavWindowingHighlight; // Highlight window when u
    using ::ImGuiCol_NavWindowingDimBg; // Darken/colorize entire 
    using ::ImGuiCol_ModalWindowDimBg; // Darken/colorize entire 
    using ::ImGuiCol_COUNT;
    using ::ImGuiCol_TabActive ;
    using ::ImGuiCol_TabUnfocused ;
    using ::ImGuiCol_TabUnfocusedActive ;
    using ::ImGuiCol_NavHighlight;

    using ::ImGuiStyleVar_;
    using ::ImGuiStyleVar_Alpha; //
    using ::ImGuiStyleVar_DisabledAlpha; //
    using ::ImGuiStyleVar_WindowPadding; //
    using ::ImGuiStyleVar_WindowRounding; //
    using ::ImGuiStyleVar_WindowBorderSize; //
    using ::ImGuiStyleVar_WindowMinSize; //
    using ::ImGuiStyleVar_WindowTitleAlign; //
    using ::ImGuiStyleVar_ChildRounding; //
    using ::ImGuiStyleVar_ChildBorderSize; //
    using ::ImGuiStyleVar_PopupRounding; //
    using ::ImGuiStyleVar_PopupBorderSize; //
    using ::ImGuiStyleVar_FramePadding; //
    using ::ImGuiStyleVar_FrameRounding; //
    using ::ImGuiStyleVar_FrameBorderSize; //
    using ::ImGuiStyleVar_ItemSpacing; //
    using ::ImGuiStyleVar_ItemInnerSpacing; //
    using ::ImGuiStyleVar_IndentSpacing; //
    using ::ImGuiStyleVar_CellPadding; //
    using ::ImGuiStyleVar_ScrollbarSize; //
    using ::ImGuiStyleVar_ScrollbarRounding; //
    using ::ImGuiStyleVar_GrabMinSize; //
    using ::ImGuiStyleVar_GrabRounding; //
    using ::ImGuiStyleVar_ImageBorderSize; //
    using ::ImGuiStyleVar_TabRounding; //
    using ::ImGuiStyleVar_TabBorderSize; //
    using ::ImGuiStyleVar_TabBarBorderSize; //
    using ::ImGuiStyleVar_TabBarOverlineSize; //
    using ::ImGuiStyleVar_TableAngledHeadersAngle; //
    using ::ImGuiStyleVar_TableAngledHeadersTextAlign;
    using ::ImGuiStyleVar_TreeLinesSize; //
    using ::ImGuiStyleVar_TreeLinesRounding; //
    using ::ImGuiStyleVar_ButtonTextAlign; //
    using ::ImGuiStyleVar_SelectableTextAlign; //
    using ::ImGuiStyleVar_SeparatorTextBorderSize; //
    using ::ImGuiStyleVar_SeparatorTextAlign; //
    using ::ImGuiStyleVar_SeparatorTextPadding; //
    using ::ImGuiStyleVar_DockingSeparatorSize; //
    using ::ImGuiStyleVar_COUNT;

    using ::ImGuiButtonFlags_;
    using ::ImGuiButtonFlags_None ;
    using ::ImGuiButtonFlags_MouseButtonLeft ;
    using ::ImGuiButtonFlags_MouseButtonRight ;
    using ::ImGuiButtonFlags_MouseButtonMiddle;
    using ::ImGuiButtonFlags_MouseButtonMask_ ;
    using ::ImGuiButtonFlags_EnableNav ;

    using ::ImGuiColorEditFlags_;

    using ::ImGuiColorEditFlags_None ;
    using ::ImGuiColorEditFlags_NoAlpha ;
    using ::ImGuiColorEditFlags_NoPicker ;
    using ::ImGuiColorEditFlags_NoOptions ;
    using ::ImGuiColorEditFlags_NoSmallPreview ;
    using ::ImGuiColorEditFlags_NoInputs ;
    using ::ImGuiColorEditFlags_NoTooltip ;
    using ::ImGuiColorEditFlags_NoLabel ;
    using ::ImGuiColorEditFlags_NoSidePreview ;
    using ::ImGuiColorEditFlags_NoDragDrop ;
    using ::ImGuiColorEditFlags_NoBorder ;
    using ::ImGuiColorEditFlags_AlphaOpaque ;
    using ::ImGuiColorEditFlags_AlphaNoBg ;
    using ::ImGuiColorEditFlags_AlphaPreviewHalf;
    using ::ImGuiColorEditFlags_AlphaBar ;
    using ::ImGuiColorEditFlags_HDR ;
    using ::ImGuiColorEditFlags_DisplayRGB ;
    using ::ImGuiColorEditFlags_DisplayHSV ;
    using ::ImGuiColorEditFlags_DisplayHex ;
    using ::ImGuiColorEditFlags_Uint8 ;
    using ::ImGuiColorEditFlags_Float ;
    using ::ImGuiColorEditFlags_PickerHueBar ;
    using ::ImGuiColorEditFlags_PickerHueWheel ;
    using ::ImGuiColorEditFlags_InputRGB ;
    using ::ImGuiColorEditFlags_InputHSV ;
    using ::ImGuiColorEditFlags_DefaultOptions_ ;
    using ::ImGuiColorEditFlags_AlphaMask_ ;
    using ::ImGuiColorEditFlags_DisplayMask_ ;
    using ::ImGuiColorEditFlags_DataTypeMask_ ;
    using ::ImGuiColorEditFlags_PickerMask_ ;
    using ::ImGuiColorEditFlags_InputMask_ ;
    using ::ImGuiColorEditFlags_AlphaPreview ;

    using ::ImGuiSliderFlags_;
    using ::ImGuiSliderFlags_None ;
    using ::ImGuiSliderFlags_Logarithmic ;
    using ::ImGuiSliderFlags_NoRoundToFormat;
    using ::ImGuiSliderFlags_NoInput ;
    using ::ImGuiSliderFlags_WrapAround ;
    using ::ImGuiSliderFlags_ClampOnInput ;
    using ::ImGuiSliderFlags_ClampZeroRange ;
    using ::ImGuiSliderFlags_NoSpeedTweaks ;
    using ::ImGuiSliderFlags_AlwaysClamp ;
    using ::ImGuiSliderFlags_InvalidMask_ ;

    using ::ImGuiMouseButton_;
    using ::ImGuiMouseButton_Left;
    using ::ImGuiMouseButton_Right ;
    using ::ImGuiMouseButton_Middle;
    using ::ImGuiMouseButton_COUNT ;

    using ::ImGuiMouseCursor_;
    using ::ImGuiMouseCursor_None ;
    using ::ImGuiMouseCursor_Arrow ;
    using ::ImGuiMouseCursor_TextInput;
    using ::ImGuiMouseCursor_ResizeAll;
    using ::ImGuiMouseCursor_ResizeNS;
    using ::ImGuiMouseCursor_ResizeEW;
    using ::ImGuiMouseCursor_ResizeNESW;
    using ::ImGuiMouseCursor_ResizeNWSE;
    using ::ImGuiMouseCursor_Hand;
    using ::ImGuiMouseCursor_Wait;
    using ::ImGuiMouseCursor_Progress;
    using ::ImGuiMouseCursor_NotAllowed;
    using ::ImGuiMouseCursor_COUNT;

    using ::ImGuiMouseSource;
    using ::ImGuiMouseSource_Mouse ;
    using ::ImGuiMouseSource_TouchScreen;
    using ::ImGuiMouseSource_Pen ;
    using ::ImGuiMouseSource_COUNT;

    using ::ImGuiCond_;
    using ::ImGuiCond_None ;
    using ::ImGuiCond_Always ;
    using ::ImGuiCond_Once ;
    using ::ImGuiCond_FirstUseEver;
    using ::ImGuiCond_Appearing ;

    using ::ImGuiTableFlags_;
    using ::ImGuiTableFlags_None ;
    using ::ImGuiTableFlags_Resizable ;
    using ::ImGuiTableFlags_Reorderable ;
    using ::ImGuiTableFlags_Hideable ;
    using ::ImGuiTableFlags_Sortable ;
    using ::ImGuiTableFlags_NoSavedSettings ;
    using ::ImGuiTableFlags_ContextMenuInBody ;
    using ::ImGuiTableFlags_RowBg ;
    using ::ImGuiTableFlags_BordersInnerH ;
    using ::ImGuiTableFlags_BordersOuterH ;
    using ::ImGuiTableFlags_BordersInnerV ;
    using ::ImGuiTableFlags_BordersOuterV ;
    using ::ImGuiTableFlags_BordersH ;
    using ::ImGuiTableFlags_BordersV ;
    using ::ImGuiTableFlags_BordersInner ;
    using ::ImGuiTableFlags_BordersOuter ;
    using ::ImGuiTableFlags_Borders ;
    using ::ImGuiTableFlags_NoBordersInBody ;
    using ::ImGuiTableFlags_NoBordersInBodyUntilResize ;
    using ::ImGuiTableFlags_SizingFixedFit ;
    using ::ImGuiTableFlags_SizingFixedSame ;
    using ::ImGuiTableFlags_SizingStretchProp ;
    using ::ImGuiTableFlags_SizingStretchSame ;
    using ::ImGuiTableFlags_NoHostExtendX ;
    using ::ImGuiTableFlags_NoHostExtendY ;
    using ::ImGuiTableFlags_NoKeepColumnsVisible ;
    using ::ImGuiTableFlags_PreciseWidths ;
    using ::ImGuiTableFlags_NoClip ;
    using ::ImGuiTableFlags_PadOuterX ;
    using ::ImGuiTableFlags_NoPadOuterX ;
    using ::ImGuiTableFlags_NoPadInnerX ;
    using ::ImGuiTableFlags_ScrollX ;
    using ::ImGuiTableFlags_ScrollY ;
    using ::ImGuiTableFlags_SortMulti ;
    using ::ImGuiTableFlags_SortTristate ;
    using ::ImGuiTableFlags_HighlightHoveredColumn ;
    using ::ImGuiTableFlags_SizingMask_ ;

    using ::ImGuiTableColumnFlags_;
    using ::ImGuiTableColumnFlags_None ;
    using ::ImGuiTableColumnFlags_Disabled ;
    using ::ImGuiTableColumnFlags_DefaultHide ;
    using ::ImGuiTableColumnFlags_DefaultSort ;
    using ::ImGuiTableColumnFlags_WidthStretch ;
    using ::ImGuiTableColumnFlags_WidthFixed ;
    using ::ImGuiTableColumnFlags_NoResize ;
    using ::ImGuiTableColumnFlags_NoReorder ;
    using ::ImGuiTableColumnFlags_NoHide ;
    using ::ImGuiTableColumnFlags_NoClip ;
    using ::ImGuiTableColumnFlags_NoSort ;
    using ::ImGuiTableColumnFlags_NoSortAscending ;
    using ::ImGuiTableColumnFlags_NoSortDescending ;
    using ::ImGuiTableColumnFlags_NoHeaderLabel ;
    using ::ImGuiTableColumnFlags_NoHeaderWidth ;
    using ::ImGuiTableColumnFlags_PreferSortAscending ;
    using ::ImGuiTableColumnFlags_PreferSortDescending ;
    using ::ImGuiTableColumnFlags_IndentEnable ;
    using ::ImGuiTableColumnFlags_IndentDisable ;
    using ::ImGuiTableColumnFlags_AngledHeader ;
    using ::ImGuiTableColumnFlags_IsEnabled ;
    using ::ImGuiTableColumnFlags_IsVisible ;
    using ::ImGuiTableColumnFlags_IsSorted ;
    using ::ImGuiTableColumnFlags_IsHovered ;
    using ::ImGuiTableColumnFlags_WidthMask_ ;
    using ::ImGuiTableColumnFlags_IndentMask_ ;
    using ::ImGuiTableColumnFlags_StatusMask_ ;
    using ::ImGuiTableColumnFlags_NoDirectResize_ ;

    using ::ImGuiTableRowFlags_;
    using ::ImGuiTableRowFlags_None ;
    using ::ImGuiTableRowFlags_Headers ;

    using ::ImGuiTableBgTarget_;
    using ::ImGuiTableBgTarget_None ;
    using ::ImGuiTableBgTarget_RowBg0 ;
    using ::ImGuiTableBgTarget_RowBg1 ;
    using ::ImGuiTableBgTarget_CellBg ;

    using ::ImGuiMultiSelectFlags_;
    using ::ImGuiMultiSelectFlags_None;
    using ::ImGuiMultiSelectFlags_SingleSelect;
    using ::ImGuiMultiSelectFlags_NoSelectAll;
    using ::ImGuiMultiSelectFlags_NoRangeSelect;
    using ::ImGuiMultiSelectFlags_NoAutoSelect;
    using ::ImGuiMultiSelectFlags_NoAutoClear;
    using ::ImGuiMultiSelectFlags_NoAutoClearOnReselect;
    using ::ImGuiMultiSelectFlags_BoxSelect1d;
    using ::ImGuiMultiSelectFlags_BoxSelect2d;
    using ::ImGuiMultiSelectFlags_BoxSelectNoScroll;
    using ::ImGuiMultiSelectFlags_ClearOnEscape;
    using ::ImGuiMultiSelectFlags_ClearOnClickVoid;
    using ::ImGuiMultiSelectFlags_ScopeWindow;
    using ::ImGuiMultiSelectFlags_ScopeRect;
    using ::ImGuiMultiSelectFlags_SelectOnClick;
    using ::ImGuiMultiSelectFlags_SelectOnClickRelease;
    using ::ImGuiMultiSelectFlags_NavWrapX;

    using ::ImDrawFlags_;
    using ::ImDrawFlags_None ;
    using ::ImDrawFlags_Closed ;
    using ::ImDrawFlags_RoundCornersTopLeft ;
    using ::ImDrawFlags_RoundCornersTopRight ;
    using ::ImDrawFlags_RoundCornersBottomLeft ;
    using ::ImDrawFlags_RoundCornersBottomRight ;
    using ::ImDrawFlags_RoundCornersNone ;
    using ::ImDrawFlags_RoundCornersTop ;
    using ::ImDrawFlags_RoundCornersBottom ;
    using ::ImDrawFlags_RoundCornersLeft ;
    using ::ImDrawFlags_RoundCornersRight ;
    using ::ImDrawFlags_RoundCornersAll ;
    using ::ImDrawFlags_RoundCornersDefault_ ;
    using ::ImDrawFlags_RoundCornersMask_ ;

    using ::ImDrawListFlags_;
    using ::ImDrawListFlags_None ;
    using ::ImDrawListFlags_AntiAliasedLines ;
    using ::ImDrawListFlags_AntiAliasedLinesUseTex;
    using ::ImDrawListFlags_AntiAliasedFill ;
    using ::ImDrawListFlags_AllowVtxOffset ;

    using ::ImFontAtlasFlags_;
    using ::ImFontAtlasFlags_None ;
    using ::ImFontAtlasFlags_NoPowerOfTwoHeight;
    using ::ImFontAtlasFlags_NoMouseCursors ;
    using ::ImFontAtlasFlags_NoBakedLines ;

    using ::ImGuiViewportFlags_;
    using ::ImGuiViewportFlags_None ;
    using ::ImGuiViewportFlags_IsPlatformWindow ;
    using ::ImGuiViewportFlags_IsPlatformMonitor ;
    using ::ImGuiViewportFlags_OwnedByApp ;
    using ::ImGuiViewportFlags_NoDecoration ;
    using ::ImGuiViewportFlags_NoTaskBarIcon ;
    using ::ImGuiViewportFlags_NoFocusOnAppearing ;
    using ::ImGuiViewportFlags_NoFocusOnClick ;
    using ::ImGuiViewportFlags_NoInputs ;
    using ::ImGuiViewportFlags_NoRendererClear ;
    using ::ImGuiViewportFlags_NoAutoMerge ;
    using ::ImGuiViewportFlags_TopMost ;
    using ::ImGuiViewportFlags_CanHostOtherWindows ;
    using ::ImGuiViewportFlags_IsMinimized ;
    using ::ImGuiViewportFlags_IsFocused ;
}

export {
    using ::ImDrawCmd;
    using ::ImDrawVert;
    using ::ImDrawCmdHeader;
    using ::ImDrawChannel;
    using ::ImDrawListSplitter;

    using ::ImDrawList;
    using ::ImDrawData;
    using ::ImFontConfig;
    using ::ImFontGlyph;
    using ::ImFontGlyphRangesBuilder;
    using ::ImFontAtlasCustomRect;

    using ::ImFontAtlas;
    using ::ImFont;

    using ::ImGuiViewport;
    using ::ImGuiPlatformIO;
    using ::ImGuiPlatformMonitor;
}

export namespace ImGui {
    using ImGui::Image;

    using ImGui::PushButtonRepeat;
    using ImGui::PopButtonRepeat;
    using ImGui::PushTabStop;
    using ImGui::PopTabStop;
    using ImGui::GetContentRegionMax;
    using ImGui::GetWindowContentRegionMin;
    using ImGui::GetWindowContentRegionMax;

    using ImGui::BeginChildFrame;
    using ImGui::EndChildFrame;

    using ImGui::ShowStackToolWindow;
    using ImGui::Combo;
    using ImGui::ListBox;

    using ImGui::SetItemAllowOverlap;

    using ImGui::PushAllowKeyboardFocus;
    using ImGui::PopAllowKeyboardFocus;
}


// misc
void m_IMGUI_CHECKVERSION() {
    IMGUI_CHECKVERSION();
}

#undef IMGUI_CHECKVERSION
export void IMGUI_CHECKVERSION() {
    m_IMGUI_CHECKVERSION();
}

// imgui_internal.h, only export used symbols

export {
    using ::GImGui;
}
