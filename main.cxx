#include <cstdint>
#include <exception>
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>

// #include <vulkan/vulkan.hpp>
// alternatively:
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <stdexcept>

class HelloTriangleApplication {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  VkInstance instance;

  void initWindow() {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    // initializes the GLFW library
    glfwInit();

    // tell GLFW to not create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // disable resizable windows
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // window creation call
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }

  // keep the application running until either an error occurs or the window is
  // closed
  void mainLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
    }
  }

  void initVulkan() { createInstance(); }

  void createInstance() {
    // A lot of information in Vulkan is passed through structs instead of
    // funcion parameters

    // optional struct to get useful information to the driver
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Hello Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    // not optional struct to tell the Vulkan driver which global extensions and
    // validation layers we wanto to use
    // Global = apply to the entire program and not a specific device.
    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    // GLFW has an handy built-in function that returns the extention(s) it
    // needs to do that which we can pass to the struct
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    create_info.enabledExtensionCount = glfwExtensionCount;
    create_info.ppEnabledExtensionNames = glfwExtensions;
    create_info.enabledLayerCount = 0;

    // At this point is specified everything Vulkan needs to create an instance

    // Issue the vkCreateInstance call
    // VkResult result = vkCreateInstance(&create_info, nullptr, &instance);

    // If everything went well, then the handle to the insatnce was stored in
    // the VkInstance class member.
    //
    // Nearly all Vulkan functions return a value of type VkResult that is
    // either VK_SUCCESS or and error code.

    // check if the instance was created successfully, we don't need to store
    // the result and can just use a check for the success value instead
    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
      throw std::runtime_error("failed to create instance!");

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                           extensions.data());
    std::cout << "available extensions:\n";
    for (const auto &extension : extensions) {
      std::cout << '\t' << extension.extensionName << '\n';
    }
  }

  void cleanup() {
    // The VkInstance should be only destroyed right before the program exits.
    vkDestroyInstance(instance, nullptr);

    // cleanup resources by destroying it and terminating GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};

int main(int argc, char *argv[]) {
  HelloTriangleApplication app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
