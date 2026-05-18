#include "pch.h"
#include "MelonImGui.h"

#include <ranges>

#include "Device.h"
#include "RendUtil.h"
#include "RenderDefines.h"
#include "TilemapEditorUi.h"
#include "UIConfig.h"

ui::window::window(const char *title, bool* p_open, const ImGuiWindowFlags flags)
    : open(ImGui::Begin(title, p_open, flags))
{
}

ui::window::~window() {
    ImGui::End();
}

bool ui::window::is_open() const {
    return open;
}

MelonImGui::MelonImGui(Device* device, GLFWwindow* window, Pipelines* pipelines)
    : _device(device), _window(window), _pipelines(pipelines)
{
    _uiSyncObjects = &_device->_uiSyncObjects;
}

void MelonImGui::Init() const {
    const VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
    poolInfo.pPoolSizes = poolSizes;

    VkDescriptorPool imguiPool;
    VK_CHECK(vkCreateDescriptorPool(_device->GetDevice(), &poolInfo, nullptr, &imguiPool));

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForVulkan(_window, true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.ApiVersion = VK_API_VERSION_1_3;
    initInfo.Instance = _device->GetInstance();
    initInfo.PhysicalDevice = _device->_physicalDevice;
    initInfo.Device = _device->GetDevice();
    initInfo.QueueFamily = _device->GetGraphicsQueueFamily();
    initInfo.Queue = _device->GetGraphicsQueue();
    initInfo.DescriptorPool = imguiPool;
    const auto swapchainImageCount = static_cast<uint32_t>(_device->GetSwapchainImages().size());
    initInfo.MinImageCount = swapchainImageCount;
    initInfo.ImageCount = swapchainImageCount;
    initInfo.UseDynamicRendering = true;

    initInfo.PipelineInfoMain.PipelineRenderingCreateInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR};
    initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
    initInfo.PipelineInfoMain.PipelineRenderingCreateInfo.pColorAttachmentFormats = &_device->_drawImage.imageFormat;
    initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&initInfo);

    _device->_mainDeletionQueue.PushFunction([this, imguiPool]() {
        ImGui_ImplVulkan_Shutdown();
        vkDestroyDescriptorPool(_device->GetDevice(), imguiPool, nullptr);
    });

    ui::init_imgui_styles(ui::styleDarkMode, alpha);

    add_window("Tilemap Editor", []() {
        ui::draw_tilemap_editor_ui("Tilemap Editor");
    });
    window_states["Tilemap Editor"] = true;
}

void MelonImGui::immediate_submit(const std::function<void(VkCommandBuffer cmd)> &func) const {
    _device->ImmediateSubmit(func);
}

void MelonImGui::Draw(VkCommandBuffer cmd, VkImageView targetImageView) const {
    VkRenderingAttachmentInfo colorAttachment = rutil::RenderingAttachmentInfo(targetImageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    const VkRenderingInfo renderInfo = rutil::RenderingInfo(_device->_swapchainExtent, &colorAttachment, nullptr);
    vkCmdBeginRendering(cmd, &renderInfo);
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    vkCmdEndRendering(cmd);
}

void MelonImGui::draw_debug_ui(const char* title) const {
    bool& isOpen = window_open(title);
    if (!isOpen) {
        return;
    }

    const int chrome_colors = ui::push_window_chrome_style();
    if (const ui::window debug(title, &isOpen); debug) {
            auto& effects = _pipelines->GetBackgroundEffects();
            int& index = _pipelines->GetCurrentBackgroundEffect();

            ComputeEffect& selected = effects[index];

            ImGui::TextUnformatted("Selected Effect:");
            ImGui::SameLine();
            ImGui::TextUnformatted(selected.name);

            ImGui::Separator();

            ImGui::SliderInt("Effect Index", &index, 0, static_cast<int>(effects.size()) - 1);

            ImGui::Spacing();
            ImGui::SeparatorText("Parameters");

            ImGui::DragFloat4("Data 1", reinterpret_cast<float *>(&selected.data.data1), 0.01f);
        }
    ui::pop_window_chrome_style(chrome_colors);
}

void MelonImGui::draw_top_menu() {

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open")) {

            }
            if (ImGui::MenuItem("Save")) {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Project Settings")) {

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools"))
        {
            for (const auto &key: editor_window_uis | std::views::keys)
            {
                if (bool& state = window_states[key]; ImGui::MenuItem(key.c_str(), nullptr, state))
                {
                    state = !state;
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void MelonImGui::add_window(const char* id, std::function<void()> func) {
    if (id == nullptr) {
        return;
    }
    editor_window_uis.try_emplace(id, func);
    window_states[id] = false;
}

bool& MelonImGui::window_open(const char* id) {
    return window_states[id];
}

void MelonImGui::set_window_state(const std::string& id, const bool value) const {
    if (const auto it = window_states.find(id); it != window_states.end()) {
        it->second = value;
    }
}

void MelonImGui::open_window(const char* id) {
    const auto& it = editor_window_uis.find(id);
    if (it == editor_window_uis.end()) {
        return;
    }
    it->second();
}
