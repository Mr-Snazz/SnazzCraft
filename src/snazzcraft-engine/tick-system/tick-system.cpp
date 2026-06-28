#include <thread>
#include <queue>

#include "snazzcraft-engine/tick-system/tick-system.hpp"
#include "snazzcraft-engine/core/mode.hpp"
#include "snazzcraft-engine/tick-system/notification/voxel-edit-next-to-liquid.hpp"
#include "snazzcraft-engine/voxel/liquid-type.hpp"

#include "snazzcraft-engine/world/world.hpp"
#include "external/glfw3.h"

std::queue<SnazzCraft::TickSystem::Notification*> Notifications;

void MainLoop();

void HandleNotifications();

void SnazzCraft::TickSystem::Initialize()
{
    std::thread MainLoopThread(MainLoop);
    MainLoopThread.detach();
}

void MainLoop()
{
    double PreviousTime = glfwGetTime();
    double Delta{};
    bool HandleRedstoneTick{};

    while (!SnazzCraft::ShouldCloseApplication())
    {
        while (SnazzCraft::GetUserMode() == SnazzCraft::UserMode::MainMenu) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10u));
            if (SnazzCraft::ShouldCloseApplication()) return;
        }

        double CurrentTime = glfwGetTime();
        Delta = CurrentTime - PreviousTime;

        if (Delta < 0.05) continue; // Execute onces every 0.05 seconds
        PreviousTime = CurrentTime;

        SnazzCraft::TickSystem::HandleNotifications();

        HandleRedstoneTick = !HandleRedstoneTick;
    }
}

void SnazzCraft::TickSystem::HandleNotifications()
{
    while (!Notifications.empty())
    {
        SnazzCraft::TickSystem::Notification* Notification = std::move(Notifications.front());
        Notifications.pop();

        switch (Notification->ID)
        {
            case SnazzCraft::TickSystem::Notification::NotificationID::VoxelEditNextToLiquid:
            {
                SnazzCraft::TickSystem::VoxelEditNextToLiquid* VoxelEditNextToLiquidNotification = static_cast<SnazzCraft::TickSystem::VoxelEditNextToLiquid*>(Notification);  

                if (!SnazzCraft::Overworld->ChunkWithinWorld(VoxelEditNextToLiquidNotification->ChunkX, VoxelEditNextToLiquidNotification->ChunkZ)) break;

                int32_t VoxelPosition[3u];
                SnazzCraft::Chunk::GetVoxelPosition(VoxelEditNextToLiquidNotification->LiquidVoxelIndex, VoxelPosition); // Gets local position

                uint64_t NotificationChunkHash = SnazzCraft::IntegerHash(VoxelEditNextToLiquidNotification->ChunkX, VoxelEditNextToLiquidNotification->ChunkZ);
                SnazzCraft::Voxel LiquidVoxel;
                {
                    std::lock_guard<std::recursive_mutex> ChunksLock(SnazzCraft::Overworld->ChunksMutex);
                    auto ChunkIterator = SnazzCraft::Overworld->Chunks.find(NotificationChunkHash);
                    if (ChunkIterator == SnazzCraft::Overworld->Chunks.end()) break;

                    // Check if voxel at index is a liquid
                    LiquidVoxel = ChunkIterator->second->Voxels[SnazzCraft::Chunk::LocalVoxelIndex(VoxelPosition[0], VoxelPosition[1], VoxelPosition[2])];
                    if (!LiquidVoxel.GetVoxelType().IsLiquid) break;
                }
                
                // Convert VoxelPosition to voxel space coordinates
                VoxelPosition[0u] += SnazzCraft::Chunk::Width * VoxelEditNextToLiquidNotification->ChunkX;
                VoxelPosition[2u] += SnazzCraft::Chunk::Width * VoxelEditNextToLiquidNotification->ChunkZ;

                for (uint8_t VoxelCheckPositionIndex : SnazzCraft::LiquidType::VoxelCheckPositionsIndexes) {
                    int32_t CheckPosition[3u] = {
                        VoxelPosition[0u] + SnazzCraft::VoxelCheckPositions[VoxelCheckPositionIndex][0u],
                        VoxelPosition[1u] + SnazzCraft::VoxelCheckPositions[VoxelCheckPositionIndex][1u],
                        VoxelPosition[2u] + SnazzCraft::VoxelCheckPositions[VoxelCheckPositionIndex][2u]
                    };

                    int32_t ChunkPosition[2];
                    SnazzCraft::Chunk::GetChunkPosition(CheckPosition[0], CheckPosition[2], ChunkPosition);
                    if (!SnazzCraft::Overworld->ChunkWithinWorld(ChunkPosition[0], ChunkPosition[1])) continue;
                    uint64_t ChunkHash = SnazzCraft::IntegerHash(ChunkPosition[0], ChunkPosition[1]);

                    {
                        std::lock_guard<std::recursive_mutex> ChunksLock(SnazzCraft::Overworld->ChunksMutex);
                        auto ChunkIterator = SnazzCraft::Overworld->Chunks.find(ChunkHash);
                        if (ChunkIterator == SnazzCraft::Overworld->Chunks.end()) break;

                        int32_t LocalVoxelPosition[3];
                        SnazzCraft::Chunk::GetLocalVoxelPosition(CheckPosition[0], CheckPosition[1], CheckPosition[2], LocalVoxelPosition);

                        uint32_t VoxelIndex = SnazzCraft::Chunk::LocalVoxelIndex(LocalVoxelPosition[0], LocalVoxelPosition[1], LocalVoxelPosition[2]);

                        SnazzCraft::Voxel& Voxel = ChunkIterator->second->Voxels[VoxelIndex];
                        const SnazzCraft::VoxelType& VoxelType = Voxel.GetVoxelType();
                        if (VoxelType.StopsLiquid) continue;

                        Voxel.ID = LiquidVoxel.ID;
                        ChunkIterator->second->CullVoxelFaces();
                        ChunkIterator->second->UpdateVerticesAndIndices();
                        
                        bool UpdatedChunk{};
                        SnazzCraft::Overworld->UpdateChunkLighting(ChunkIterator->second, &UpdatedChunk);
                        if (!UpdatedChunk) {
                            ChunkIterator->second->UpdateLightingOnVertices(SnazzCraft::Overworld);
                            ChunkIterator->second->ShouldUpdateMesh = true;
                        }
                    }
                }

                delete VoxelEditNextToLiquidNotification;
                break;
            }
            
            default:
                break;
        }
        Notification = nullptr;
    }
}

void SnazzCraft::TickSystem::AddNotification(SnazzCraft::TickSystem::Notification* Notification)
{
    Notifications.push(Notification);
}



