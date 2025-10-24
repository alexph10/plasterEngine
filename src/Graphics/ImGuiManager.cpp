#include "Graphics/ImGuiManager.h"
#include "Graphics/VulkanContext.h"
#include "Core/Window.h"

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

#include <vector>

namespace plaster {

ImGuiManager::ImGuiManager(Window* window, VulkanContext* vulkanContext, VkRenderPass renderPass)
    : m_window(window), m_vulkanContext(vulkanContext), m_imguiDescriptorPool(VK_NULL_HANDLE) {
    
  // Setup ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup Dear ImGui style
  setTheme();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForVulkan(m_window->getHandle(), true);
    
  // Create descriptor pool for ImGui
  VkDescriptorPoolSize poolSizes[] = {
      {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
      {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
      {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
  };
    
  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  poolInfo.maxSets = 1000;
  poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
  poolInfo.pPoolSizes = poolSizes;

  vkCreateDescriptorPool(m_vulkanContext->getDevice(), &poolInfo, nullptr, &m_imguiDescriptorPool);

  // Initialize ImGui Vulkan backend
  ImGui_ImplVulkan_InitInfo initInfo{};
  initInfo.Instance = m_vulkanContext->getInstance();
  initInfo.PhysicalDevice = m_vulkanContext->getPhysicalDevice();
  initInfo.Device = m_vulkanContext->getDevice();
  initInfo.QueueFamily = m_vulkanContext->getGraphicsQueueFamily();
  initInfo.Queue = m_vulkanContext->getGraphicsQueue();
  initInfo.DescriptorPool = m_imguiDescriptorPool;
  initInfo.MinImageCount = 2;
  initInfo.ImageCount = 2;
  initInfo.CheckVkResultFn = nullptr;
  initInfo.PipelineInfoMain.RenderPass = renderPass;

  ImGui_ImplVulkan_Init(&initInfo);
}

ImGuiManager::~ImGuiManager() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_imguiDescriptorPool) {
        vkDestroyDescriptorPool(m_vulkanContext->getDevice(), m_imguiDescriptorPool, nullptr);
    }
}

void ImGuiManager::newFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::render(VkCommandBuffer commandBuffer) {
    ImGui::Render();
    ImDrawData* drawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
}

void ImGuiManager::setTheme() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  // Colors from your reference image
  ImVec4 radiantYellow = ImVec4(0.996f, 1.0f, 0.11f, 1.0f);      // #feff1d
  ImVec4 toxicYellow = ImVec4(1.0f, 1.0f, 0.07f, 1.0f);          // #ffff1b
  ImVec4 starkWhite = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);            // #ffffff
  ImVec4 deepBlack = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);          // #0a0a0a
  ImVec4 charcoal = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);           // #1a1a1a
  ImVec4 darkGray = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);           // #2a2a2a

  // Background & Surface colors
  colors[ImGuiCol_WindowBg] = charcoal;
  colors[ImGuiCol_ChildBg] = deepBlack;
  colors[ImGuiCol_PopupBg] = charcoal;
  colors[ImGuiCol_Border] = darkGray;
  colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

  // Text colors
  colors[ImGuiCol_Text] = starkWhite;
  colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

  // Button colors
  colors[ImGuiCol_Button] = darkGray;
  colors[ImGuiCol_ButtonHovered] = radiantYellow;
  colors[ImGuiCol_ButtonActive] = toxicYellow;

  // Header/Title colors
  colors[ImGuiCol_Header] = darkGray;
  colors[ImGuiCol_HeaderHovered] = radiantYellow;
  colors[ImGuiCol_HeaderActive] = toxicYellow;

  // Selection colors
  colors[ImGuiCol_FrameBg] = charcoal;
  colors[ImGuiCol_FrameBgHovered] = darkGray;
  colors[ImGuiCol_FrameBgActive] = radiantYellow;

  // Scrollbar colors
  colors[ImGuiCol_ScrollbarBg] = charcoal;
  colors[ImGuiCol_ScrollbarGrab] = darkGray;
  colors[ImGuiCol_ScrollbarGrabHovered] = radiantYellow;
  colors[ImGuiCol_ScrollbarGrabActive] = toxicYellow;

  // Slider colors
  colors[ImGuiCol_SliderGrab] = radiantYellow;
  colors[ImGuiCol_SliderGrabActive] = toxicYellow;

  // Checkbox/Radio colors
  colors[ImGuiCol_CheckMark] = radiantYellow;

  // Tab colors
  colors[ImGuiCol_Tab] = charcoal;
  colors[ImGuiCol_TabHovered] = radiantYellow;
  colors[ImGuiCol_TabActive] = toxicYellow;
  colors[ImGuiCol_TabUnfocused] = charcoal;
  colors[ImGuiCol_TabUnfocusedActive] = charcoal;

  // Separator/Divider colors
  colors[ImGuiCol_Separator] = darkGray;
  colors[ImGuiCol_SeparatorHovered] = radiantYellow;
  colors[ImGuiCol_SeparatorActive] = toxicYellow;

  // Plot/Graph colors
  colors[ImGuiCol_PlotLines] = radiantYellow;
  colors[ImGuiCol_PlotLinesHovered] = toxicYellow;
  colors[ImGuiCol_PlotHistogram] = radiantYellow;
  colors[ImGuiCol_PlotHistogramHovered] = toxicYellow;

  // Drag & Drop colors
  colors[ImGuiCol_DragDropTarget] = toxicYellow;

  // Navigation highlight
  colors[ImGuiCol_NavHighlight] = radiantYellow;

  // Window styling
  style.WindowPadding = ImVec2(8.0f, 8.0f);
  style.FramePadding = ImVec2(4.0f, 3.0f);
  style.ItemSpacing = ImVec2(8.0f, 4.0f);
  style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
  style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
  style.IndentSpacing = 21.0f;
  style.ScrollbarSize = 14.0f;
  style.GrabMinSize = 10.0f;
    
  // Rounding
  style.WindowRounding = 4.0f;
  style.ChildRounding = 3.0f;
  style.FrameRounding = 3.0f;
  style.PopupRounding = 3.0f;
  style.ScrollbarRounding = 3.0f;
  style.GrabRounding = 3.0f;
  style.TabRounding = 3.0f;
}

} // namespace plaster

