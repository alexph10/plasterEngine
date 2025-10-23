#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

class Window;
class VulkanContext;

class Renderer {
public:
  Renderer(Window* window, VulkanContext* vulkanContext);
  ~Renderer();
  
  void render();

private:
  VulkanContext* m_vulkanContext;
  Window* m_window;

  VkSwapchainKHR m_swapchain;
  std::vector<VkImage> m_swapchainImages;
  std::vector<VkImageView> m_swapchainImageViews;
  VkFormat m_swapchainImageFormat;
  VkExtent2D m_swapchainExtent;

  void createSwapchain();
  void createImageViews();
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};
