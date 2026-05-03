#include "snazzcraft-engine/core/core.hpp"
#include "snazzcraft-engine/mesh/voxel-vertice.hpp"
#include "snazzcraft-engine/core/window.hpp"
#include "snazzcraft-engine/texture/atlas.hpp"
#include "snazzcraft-engine/texture/texture.hpp"
#include "snazzcraft-engine/entity/entity-type.hpp"
#include "snazzcraft-engine/mesh/mesh.hpp"
#include "snazzcraft-engine/fps-tracker/fps-tracker.hpp"
#include "snazzcraft-engine/entity/user.hpp"
#include "snazzcraft-engine/world/world.hpp"
#include "snazzcraft-engine/world/voxel-ids.h"
#include "snazzcraft-engine/shader/voxel-shader.hpp"

glm::mat4 SnazzCraft::ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
glm::mat4 SnazzCraft::ModelMatrix = glm::mat4(1.0f);
glm::mat4 SnazzCraft::ViewMatrix = glm::mat4(1.0f);

bool SnazzCraft::CloseApplication = false;

const std::vector<SnazzCraft::VoxelVertice> VoxelMeshVertices = {
    //   Position             // Normal             // Texture Coords

    // Front
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.2f } },
    { { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.2f, 0.0f } },
    { {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.2f, 0.2f } },

    // Left 
    { { -1.0f, -1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.2f } },
    { { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.2f, 0.0f } },
    { { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.2f, 0.2f } },

    // Right 
    { { 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.2f } },
    { { 1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { 1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.2f, 0.0f } },
    { { 1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.2f, 0.2f } },
    
    // Back
    { {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.2f } },
    { {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.2f, 0.0f } },
    { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.2f, 0.2f } },

    // Top
    { { -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.2f } },
    { { -1.0f, 1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  1.0f, 1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.2f, 0.0f } },
    { {  1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.2f, 0.2f } },

    // Bottom
    { { -1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.2f } },
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.2f, 0.0f } },
    { {  1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.2f, 0.2f } },
};

const std::vector<uint32_t> VoxelMeshIndices = {
    0,  1,  2,    2,  3,  0,    // Front  (verts 0  .. 3 )
    4,  5,  6,    6,  7,  4,    // Left   (verts 4  .. 7 )
    8,  9,  10,   10, 11, 8,    // Right  (verts 8  .. 11)
    12, 13, 14,   14, 15, 12,   // Back   (verts 12 .. 15)
    16, 17, 18,   18, 19, 16,   // Top    (verts 16 .. 19)
    20, 21, 22,   22, 23, 20,   // Bottom (verts 20 .. 23)
};

void MainMenuInputCallback(SnazzCraft::Event* Event);

void WorldInputCallback(SnazzCraft::Event* Event);

bool SnazzCraft::Initiate()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SnazzCraft::Window = glfwCreateWindow(900, 900, "SnazzCraft", NULL, NULL);
    if (SnazzCraft::Window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();

        return false;
    }

    glfwMakeContextCurrent(SnazzCraft::Window);
    glfwSetFramebufferSizeCallback(SnazzCraft::Window, &FrameBufferSizeCallBack);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

        return false;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    const SnazzCraft::VoxelShader& VoxelShaderInstance = SnazzCraft::VoxelShader::GetInstance();
    VoxelShaderInstance.SetProjectionMatrix(SnazzCraft::ProjectionMatrix, true);
    VoxelShaderInstance.SetModelMatrix(SnazzCraft::ModelMatrix, false);
    VoxelShaderInstance.SetViewMatrix(SnazzCraft::ViewMatrix, false);
    VoxelShaderInstance.SetLightPosition(glm::vec3(0.0f, 50.0f, 0.0f), false);
    VoxelShaderInstance.SetViewPosition(SnazzCraft::Player->Position, false);
    VoxelShaderInstance.SetAmbient(0.6f, false);
    VoxelShaderInstance.SetComplexLighting(true, false);
    
    SnazzCraft::VoxelTextureAtlas = new SnazzCraft::Texture("textures/voxel/atlas.png");
    stbi_set_flip_vertically_on_load(true);

    glfwSetMouseButtonCallback(SnazzCraft::Window, MouseButtonCallback);

    SnazzCraft::EntityType::Initialize();
    
    SnazzCraft::VoxelMesh = new Mesh(VoxelMeshVertices, VoxelMeshIndices);

    SnazzCraft::VoxelMesh->ScaleVector = { (float)(SnazzCraft::Voxel::Size) / 2, (float)(SnazzCraft::Voxel::Size) / 2, (float)(SnazzCraft::Voxel::Size) / 2 };

    SnazzCraft::MainMenuGUI::Initialize(MainMenuInputCallback);
    SnazzCraft::WorldGUI::Initialize(WorldInputCallback);

    return true;
}

void SnazzCraft::MainLoop()
{
    while (!glfwWindowShouldClose(SnazzCraft::Window) && !SnazzCraft::CloseApplication)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        SnazzCraft::GlobalFPSTracker->UpdateFPS();
        //system(CLEAR_COMMAND);
        //std::cout << "FPS| " << SnazzCraft::GlobalFPSTracker->FPS << "\n";

        switch (SnazzCraft::UserMode)
        {
            case SNAZZCRAFT_USER_MODE_WORLD:
            {
                if (SnazzCraft::CurrentWorld == nullptr) break;

                SnazzCraft::WorldGUI& WorldGUIInstance = SnazzCraft::WorldGUI::GetInstance();

                WorldGUIInstance.PollEvents();
                WorldGUIInstance.HandleEvents();

                SnazzCraft::CurrentWorld->ApplyGravityToAllEntities();

                SnazzCraft::CurrentWorld->Render();
                WorldGUIInstance.Draw();

                break;
            }
                
            case SNAZZCRAFT_USER_MODE_MAIN_MENU:
            {
                SnazzCraft::MainMenuGUI& MainMenuGUIInstance = SnazzCraft::MainMenuGUI::GetInstance();

                MainMenuGUIInstance.PollEvents();
                MainMenuGUIInstance.HandleEvents();
                
                MainMenuGUIInstance.Draw();

                break;
            }
        }
      
        glfwSwapBuffers(SnazzCraft::Window);
        glfwPollEvents(); 
    }

    SnazzCraft::CloseApplication = true;
}

void SnazzCraft::FreeResources()
{
    SnazzCraft::CurrentWorld->SaveWorldToFile(true);

    SnazzCraft::EntityType::FreeResources();

    delete SnazzCraft::Player;
    delete SnazzCraft::VoxelMesh;
    delete SnazzCraft::EngineVoxelTextureApplier;
    delete SnazzCraft::CurrentWorld;
    delete SnazzCraft::VoxelTextureAtlas;
    delete SnazzCraft::GlobalFPSTracker;
    
    glfwTerminate();
}

void WorldInputCallback(SnazzCraft::Event* Event)
{
    static uint8_t VoxelIDToPlace = 0;

    SnazzCraft::WorldGUI& WorldGUIInstance = SnazzCraft::WorldGUI::GetInstance();

    if (SnazzCraft::CurrentWorld == nullptr) return;
    
    if (!WorldGUIInstance.InInventory) { // Not in inventory
        switch (Event->Type)
        {
            case SNAZZCRAFT_EVENT_KEY_DOWN:
            {
                unsigned char* Key = static_cast<unsigned char*>(Event->EventData->AccessDataType(SNAZZCRAFT_DATA_TYPE_KEY));
                if (Key == nullptr) return;
                
                switch (*Key)
                {
                    case SNAZZCRAFT_KEY_0:
                        VoxelIDToPlace = 0;
                        break;
                    
                    case SNAZZCRAFT_KEY_1:
                        VoxelIDToPlace = 1;
                        break;
                    
                    case SNAZZCRAFT_KEY_2:
                        VoxelIDToPlace = 2;
                        break;
                    
                    case SNAZZCRAFT_KEY_3:
                        VoxelIDToPlace = 3;
                        break;

                    case SNAZZCRAFT_KEY_4:
                        VoxelIDToPlace = 4;
                        break;
                    
                    case SNAZZCRAFT_KEY_5:
                        VoxelIDToPlace = 5;
                        break;

                    case SNAZZCRAFT_KEY_6:
                        VoxelIDToPlace = 6;
                        break;
                    
                    case SNAZZCRAFT_KEY_7:
                        VoxelIDToPlace = 7;
                        break;

                    case SNAZZCRAFT_KEY_8:
                        VoxelIDToPlace = 8;
                        break;
                    
                    case SNAZZCRAFT_KEY_9:
                        VoxelIDToPlace = 9;
                        break;

                    case SNAZZCRAFT_KEY_T:
                        VoxelIDToPlace = ID_VOXEL_TORCH;
                        break;

                    case SNAZZCRAFT_KEY_ESCAPE:
                        SnazzCraft::CloseApplication = true;
                        break;

                    case SNAZZCRAFT_KEY_W:
                        SnazzCraft::CurrentWorld->MoveEntity(SnazzCraft::Player, glm::vec3(0.0f), 1.0f);
                        break;

                    case SNAZZCRAFT_KEY_A:
                        SnazzCraft::CurrentWorld->MoveEntity(SnazzCraft::Player, glm::vec3(0.0f, -90.0f, 0.0f), 1.0f);
                        break;

                    case SNAZZCRAFT_KEY_S:
                        SnazzCraft::CurrentWorld->MoveEntity(SnazzCraft::Player, glm::vec3(0.0f, 180.0f, 0.0f), 1.0f);
                        break;

                    case SNAZZCRAFT_KEY_D:
                        SnazzCraft::CurrentWorld->MoveEntity(SnazzCraft::Player, glm::vec3(0.0f, 90.0f, 0.0f), 1.0f);
                        break;

                    case SNAZZCRAFT_KEY_SPACE:
                        SnazzCraft::CurrentWorld->MoveEntity(glm::vec3(0.0f, 1.0f, 0.0f), SnazzCraft::Player);
                        break;

                    case SNAZZCRAFT_KEY_LEFT_SHIFT:
                        SnazzCraft::CurrentWorld->MoveEntity(glm::vec3(0.0f, -1.0f, 0.0f), SnazzCraft::Player);
                        break;

                    case SNAZZCRAFT_KEY_Q:
                        SnazzCraft::Player->Rotate({ 0.0f, -2.0f, 0.0f });
                        break;

                    case SNAZZCRAFT_KEY_E:
                        SnazzCraft::Player->Rotate({ 0.0f, 2.0f, 0.0f });
                        break;

                    case SNAZZCRAFT_KEY_X:
                        SnazzCraft::Player->Rotate({ 0.0f, 0.0f, -2.0f });
                        break;

                    case SNAZZCRAFT_KEY_C:
                        SnazzCraft::Player->Rotate({ 0.0f, 0.0f, 2.0f });
                        break;

                    default:
                        break;
                }

                break;
            }

            case SNAZZCRAFT_EVENT_MOUSE_CLICK_RIGHT_PRESS:
            {
                if (SnazzCraft::CurrentWorld == nullptr) break;

                SnazzCraft::CurrentWorld->PlaceVoxel(SnazzCraft::Player->Position, SnazzCraft::Player->Rotation, VoxelIDToPlace);

                break;
            }

            case SNAZZCRAFT_EVENT_MOUSE_CLICK_LEFT_PRESS:
            {
                WorldGUIInstance.SendEventToPanels(Event);
                SnazzCraft::CurrentWorld->DestroyVoxel(SnazzCraft::Player->Position, SnazzCraft::Player->Rotation);
            
                break;
            }

            default:
                break;
        }
    } else { // In inventory
        switch (Event->Type)
        {
            case SNAZZCRAFT_EVENT_KEY_DOWN:
            {
                unsigned char* Key = static_cast<unsigned char*>(Event->EventData->AccessDataType(SNAZZCRAFT_DATA_TYPE_KEY));
                if (Key == nullptr) return;

                switch (*Key)
                {

                    default:
                        break;
                }

                break;
            }

            case SNAZZCRAFT_EVENT_MOUSE_CLICK_LEFT_PRESS:
            {
                WorldGUIInstance.SendEventToPanels(Event);
                
                break;
            }

            default:
                break;
        }
    }
}

void MainMenuInputCallback(SnazzCraft::Event* Event)
{
    const SnazzCraft::MainMenuGUI& MainMenuGUIInstance = SnazzCraft::MainMenuGUI::GetInstance();

    switch (Event->Type)
    {
        case SNAZZCRAFT_EVENT_MOUSE_CLICK_LEFT_PRESS:
        {
            MainMenuGUIInstance.SendEventToPanels(Event);
            break;
        }
    }
}





