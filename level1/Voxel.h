#ifndef VOXEL_H
#define VOXEL_H

// 普通体素 (1.0f)
struct Voxel {
    float x, y, z;
    float r, g, b;
};

// 手表体素 (0.2f) - 保留
struct VoxelWatch {
    float x, y, z;
    float r, g, b;
};

// [新增] 脸部细节体素 (我们将把它渲染为 0.25f 大小)
struct VoxelFace {
    float x, y, z;
    float r, g, b;
};

// [新增] 名字体素 (我们将把它渲染为 0.25f 大小)
struct VoxelName {
    float x, y, z;
    float r, g, b;
};

#endif // VOXEL_H