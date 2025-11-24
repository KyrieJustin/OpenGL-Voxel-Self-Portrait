#ifndef MODELS_H
#define MODELS_H

#include <vector>
#include "Voxel.h"// 包含Voxel和VoxelWatch的定义

// 返回一个包含自画像所有Voxel的vector
std::vector<Voxel> createSelfPortraitModel();

// 声明手表模型函数（新增，必须与实现一致）
std::vector<VoxelWatch> createWatchModel();

// 返回脸部细节模型
std::vector<VoxelFace> createFaceDetails(float time);


// 返回一个包含场景景观所有Voxel的vector
std::vector<Voxel> createLandscapeModel();



// 动态绘制水面（直接调用绘制指令，不返回vector）
void drawAnimatedWater(float time);

// [新增] 绘制下雪粒子
void drawSnow(float time);


// 返回精细名字模型
std::vector<VoxelName> createDetailedNameModel();

// 增加 time 参数，用于制作挑眉和流光动画
std::vector<VoxelFace> createFaceDetails(float time);

#endif // MODELS_H