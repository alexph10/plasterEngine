#pragma once

#include <string>
#include <cstdint>

struct GLFWwindow;

namespace plaster {

class Window {
public:
  Window(uint32_t width, uint32_t height, const std::string& title);
  ~Window();
  
  bool shouldClose() const;
  void pollEvents();
  void waitEvents();

  GLFWwindow* getHandle() const { return m_window; }
  uint32_t getWidth() const { return m_width; }
  uint32_t getHeight() const { return m_height; }
private:
  GLFWwindow* m_window;
  uint32_t m_width;
  uint32_t m_height;
};

} // namespace plaster

