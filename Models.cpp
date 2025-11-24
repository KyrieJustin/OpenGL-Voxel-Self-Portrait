#include "Models.h"
#include <cmath>

// 自画像模型 - 已增高并重构姿态
// 自画像模型 - 最终完美版：左手搭栏杆，右手下垂戴手表
// 自画像模型 - 最终精修版：下半身完美 + 精致五官（墨镜、耳、鼻、口、眉）
std::vector<Voxel> createSelfPortraitModel() {
    std::vector<Voxel> model;

    // --- 颜色库 ---
    const float SKIN_R = 0.9f, SKIN_G = 0.7f, SKIN_B = 0.5f;
    const float LIP_R = 0.95f, LIP_G = 0.6f, LIP_B = 0.5f; // 嘴唇颜色（深肤色）
    const float SHIRT_R = 0.1f, SHIRT_G = 0.1f, SHIRT_B = 0.1f;
    const float PANTS_R = 0.95f, PANTS_G = 0.95f, PANTS_B = 0.9f;
    const float SHOES_R = 1.0f, SHOES_G = 1.0f, SHOES_B = 1.0f;
    const float HAIR_R = 0.15f, HAIR_G = 0.15f, HAIR_B = 0.15f;
    const float SUNGLASSES_R = 0.1f, SUNGLASSES_G = 0.1f, SUNGLASSES_B = 0.1f; // 纯黑墨镜

    float bodyWidth = 3.0f;
    float bodyDepth = 2.0f;

    // ==================== 下半身 (保持完美状态) ====================

    // --- 鞋子 ---
    model.push_back({ -1.7f, 0.0f, 0.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ -1.3f, 0.0f, 0.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ -1.5f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ -1.7f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ -1.3f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ -1.5f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ 1.7f, 0.0f, 0.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ 1.3f, 0.0f, 0.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ 1.5f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ 1.7f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ 1.3f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });
    model.push_back({ 1.5f, 0.0f, 1.0f, SHOES_R, SHOES_G, SHOES_B });

    // --- 腿部 ---
    for (int y = 1; y <= 3; ++y) {
        model.push_back({ -1.5f, (float)y, 0.0f, SKIN_R, SKIN_G, SKIN_B });
        model.push_back({ 1.5f, (float)y, 0.0f, SKIN_R, SKIN_G, SKIN_B });
    }

    // --- 短裤 (精确缝隙版) ---
    // 裤腰
    for (float x = -bodyWidth / 2; x <= bodyWidth / 2; ++x) {
        for (float z = -bodyDepth / 2; z <= bodyDepth / 2; ++z) {
            model.push_back({ x, 5.0f, z, PANTS_R, PANTS_G, PANTS_B });
        }
    }
    // 裤管
    float innerPos = 0.35f + 0.5f;
    for (float z = -bodyDepth / 2; z <= bodyDepth / 2; ++z) {
        model.push_back({ -1.5f, 4.0f, z, PANTS_R, PANTS_G, PANTS_B });
        model.push_back({ -innerPos, 4.0f, z, PANTS_R, PANTS_G, PANTS_B });
        model.push_back({ 1.5f, 4.0f, z, PANTS_R, PANTS_G, PANTS_B });
        model.push_back({ innerPos, 4.0f, z, PANTS_R, PANTS_G, PANTS_B });
    }

    // --- 躯干 ---
    for (int y = 6; y <= 9; ++y) {
        for (float x = -bodyWidth / 2; x <= bodyWidth / 2; ++x) {
            for (float z = -bodyDepth / 2; z <= bodyDepth / 2; ++z) {
                model.push_back({ x, (float)y, z, SHIRT_R, SHIRT_G, SHIRT_B });
            }
        }
    }

    // --- 手臂 ---
    // 左臂 (搭栏杆)
    model.push_back({ 2.5f, 8.0f, 0.0f, SHIRT_R, SHIRT_G, SHIRT_B });
    for (float y = 4.7f; y <= 7.0f; y+=0.1f) {
        model.push_back({ 2.5f, (float)y, 0.0f, SKIN_R, SKIN_G, SKIN_B });
    }
    // 右臂 (下垂)
    model.push_back({ -2.5f, 8.0f, 0.0f, SHIRT_R, SHIRT_G, SHIRT_B });
    // --- 右臂上部 (平滑过渡版) ---
    // 原代码范围：Y 从 7.0 降到 6.2，Z 从 -0.1 降到 -0.9
    // 现在改用循环，密度提高10倍，实现丝滑效果
    for (int i = 0; i <= 800; ++i) {
        float offset = (float)i * 0.001f; // 每次增加 0.01 的偏移量

        model.push_back({
            -2.5f,              // X 保持不变
            7.0f - offset*0.9f,      // Y 逐渐减少 0.01 (从 7.0 到 6.2)
            -0.0f - offset*1.4f,     // Z 逐渐减少 0.01 (从 -0.1 到 -0.9)
            SKIN_R, SKIN_G, SKIN_B
            });
    }
    for (int i = 0; i <= 700; ++i) {
        float offset = (float)i * 0.001f;

        model.push_back({
            -2.5f,              // X 保持不变
            6.28f - offset*1.7f,      // Y 逐渐减少 (从 6.0 到 5.3)
            -1.12f + offset,     // Z 逐渐增加 (从 -0.7 回到 0.0)
            SKIN_R, SKIN_G, SKIN_B
            });
    }

    // ==================== 头部与五官 (全新升级) ====================

    // --- 头部 (作为画布，只保留皮肤和头发) ---
    // 脸部基底 (Y=10, 11, 12)
    for (int y = 10; y <= 12; ++y) {
        for (int x = -1; x <= 1; ++x) {
            for (int z = -1; z <= 1; ++z) {
                model.push_back({ (float)x, (float)y, (float)z, SKIN_R, SKIN_G, SKIN_B });
            }
        }
    }
    // 头发 (Y=13 及侧面)
    for (float x = -1.225f; x <= 1.225f; x+=0.025f) {
        for (float z = -1.05f; z <= 1.5f; z+=0.225f) {
            model.push_back({ (float)x, 13.0f, (float)z, HAIR_R, HAIR_G, HAIR_B });
        }
    }

    // *** 修正：后脑勺头发 ***
    // 头后部表面在 Z = -1.5。
    // 我们把头发放在 Z = -1.6，这样刚好贴在后面，不会闪烁。
    for (float x = -1.225f; x <= 1.225f; x += 0.025f) {
        model.push_back({ (float)x, 12.4f, -1.05f, HAIR_R, HAIR_G, HAIR_B });
        //model.push_back({ (float)x, 12.0f, -1.3f, HAIR_R, HAIR_G, HAIR_B }); // 加长后脑勺头发
    }


    return model;
}


// 修正后的 createWatchModel 函数
std::vector<VoxelWatch> createWatchModel() {
    std::vector<VoxelWatch> watchModel;

    // 手臂参数：中心 X=2.5, Y=6.0, Z=0.0。 尺寸 1.0。
    // 手表颜色
    float BAND_R = 0.2f, BAND_G = 0.2f, BAND_B = 0.2f; // 黑表带
    float FACE_R = 0.9f, FACE_G = 0.9f, FACE_B = 0.9f; // 白表盘

    // 1. 表带 - 正面 (Z > 0.5)
    // 我们在 Z=0.55 处画一排小方块
    for (float x = 2.1f; x <= 2.9f; x += 0.2f) {
        watchModel.push_back({ x, 5.1f, 0.55f, BAND_R, BAND_G, BAND_B });
    }

    // 2. 表带 - 背面 (Z < -0.5)
    // 在 Z=-0.55 处画一排
    for (float x = 2.1f; x <= 2.9f; x += 0.2f) {
        watchModel.push_back({ x, 5.1f, -0.55f, BAND_R, BAND_G, BAND_B });
    }

    // 3. 表带 - 外侧 (X > 3.0)
    // 在 X=3.05 处画一排
    for (float z = -0.5f; z <= 0.5f; z += 0.2f) {
        watchModel.push_back({ 3.05f, 5.1f, z, BAND_R, BAND_G, BAND_B });
    }

    // 4. 表带 - 内侧 (X < 2.0)
    // 在 X=1.95 处画一排
    for (float z = -0.5f; z <= 0.5f; z += 0.2f) {
        watchModel.push_back({ 1.95f, 5.1f, z, BAND_R, BAND_G, BAND_B });
    }

    // 5. 表盘 (在正面稍微突起一点，Z=0.65)
    // 放在手腕外侧偏正面的位置，或者正中心
    watchModel.push_back({ 2.5f, 5.1f, 0.65f, FACE_R, FACE_G, FACE_B });       // 表盘中心
    watchModel.push_back({ 2.5f + 0.2f, 5.1f, 0.65f, BAND_R, BAND_G, BAND_B }); // 装饰边缘
    watchModel.push_back({ 2.5f - 0.2f, 5.1f, 0.65f, BAND_R, BAND_G, BAND_B }); // 装饰边缘

    return watchModel;
}

// 优化版：更自然、扎根于水底的雪山 + 延伸的栈道
std::vector<Voxel> createLandscapeModel() {
    std::vector<Voxel> model;

    // 颜色定义
    const float WOOD_R = 0.6f, WOOD_G = 0.4f, WOOD_B = 0.2f;      // 木头
    const float GRASS_R = 0.1f, GRASS_G = 0.5f, GRASS_B = 0.1f;   // 深绿森林
    const float ROCK_R = 0.5f, ROCK_G = 0.5f, ROCK_B = 0.5f;      // 岩石灰
    const float SNOW_R = 1.0f, SNOW_G = 1.0f, SNOW_B = 1.0f;      // 山顶雪

    // 1. --- 木栈道 ---
    // 地面部分
    for (int x = -50; x <= 50; ++x) {
        for (int z = -5; z <= 15; ++z) {
            model.push_back({ (float)x, -1.0f, (float)z, WOOD_R, WOOD_G, WOOD_B });
        }
    }

    // 2. --- 栏杆 (全新升级：长排栏杆) ---
    // 我们用循环来生成一排柱子，而不是只画两个

    // (1) 横向扶手：贯穿整个栈道 (从 X=-20 到 X=20)
    for (int x = -50; x <= 50; ++x) {
        model.push_back({ (float)x, 5.0f, -4.0f, WOOD_R, WOOD_G, WOOD_B });
    }

    // (2) 垂直立柱：每隔 10 个单位放一根柱子，看起来更稳固
    for (int x = -50; x <= 50; x += 10) {
        // 每根柱子从 Y=0 画到 Y=4
        for (int y = 0; y <= 4; ++y) {
            model.push_back({ (float)x, (float)y, -4.0f, WOOD_R, WOOD_G, WOOD_B });
        }
    }

    // 3. --- 修正后的雪山群 (The Fixed Mountains) ---

    // [修改点1] 推远：从 Z=-25 开始画，而不是 -15，留出更多水面
    // [修改点2] 范围：画到 -60，让山脉更深远
    for (int x = -50; x <= 50; ++x) {
        for (int z = -60; z <= -25; ++z) {

            // [修改点3] 压低高度：
            // 基础高度降低，正弦波系数从 8.0 降到 6.0，让山势不那么突兀
            float distanceFactor = (float)(abs(z) - 25) * 0.4f;
            float height = 2.0f + distanceFactor +
                6.0f * sin((float)x * 0.1f) +
                3.0f * cos((float)z * 0.15f);

            // 如果算出来的高度比水面还低，就完全不画（那是水底）
            // 假设水面在 -2.5 左右
            if (height < -3.0f) continue;

            // [修改点4] 扎根：
            // 强制从 Y=-10 (深水底) 开始画，一直画到山顶
            // 这样山体底部就被埋在水里，绝对不会飘在空中
            for (int y = -10; y < height; ++y) {
                float r, g, b;

                // 颜色分层逻辑
                if (y < 5) {
                    r = GRASS_R; g = GRASS_G; b = GRASS_B; // 森林（现在包括水下的山基）
                }
                else if (y < 12) {
                    r = ROCK_R; g = ROCK_G; b = ROCK_B;    // 岩石
                }
                else {
                    r = SNOW_R; g = SNOW_G; b = SNOW_B;    // 雪顶
                }

                // 性能优化：只画看得到的表面（山顶向下3层，或者水面以上的部分）
                // 必须保证水面处有方块，不然会看到水下面的空洞
                if (y > height - 3.0f || y > -4.0f) {
                    model.push_back({ (float)x, (float)y, (float)z, r, g, b });
                }
            }
        }
    }

    return model;
}

// [新增] 动态水面绘制函数
// 注意：这个函数不返回vector，而是直接调用Utils.h里的drawCube
// 这样每一帧都可以根据 time 改变位置，而不需要重新生成百万个方块
#include "Utils.h" // 确保包含drawCube
void drawAnimatedWater(float time) {
    const float WATER_R = 0.2f, WATER_G = 0.6f, WATER_B = 0.9f;

    // 水面范围
    for (int x = -50; x <= 50; ++x) {
        for (int z = -30; z <= -6; ++z) {

            // 核心波浪算法：位置 = sin(x + 时间) + cos(z + 时间)
            // 这样水面就会随时间起伏
            float waveHeight = sin((float)x * 0.5f + time * 2.0f) * 0.25f +
                cos((float)z * 0.3f + time * 1.5f) * 0.25f;

            // 基础高度 -2.0 (在栈道下方)，加上波浪高度
            float y = -2.5f + waveHeight;

            // 绘制方块
            drawCube((float)x, y, (float)z, 1.0f, WATER_R, WATER_G, WATER_B);
        }
    }
}

// [优化版] 精细名字模型 (柔和渐变 + 抖动去条纹)
std::vector<VoxelName> createDetailedNameModel() {
    std::vector<VoxelName> model;

    float z_base = 8.0f;
    float voxel_size = 0.25f;
    float thickness = 0.5f;

    // 引入随机数种子，用于生成噪点
    // 放在函数外面或者循环里都可以，这里简单处理

    auto addVoxel = [&](float x, float y, float z, float r, float g, float b, float height_max) {

        // --- 优化 1: 柔化渐变范围 ---
        // 之前是 0.3 -> 1.0 (跨度0.7，太大了，导致断层明显)
        // 现在改为 0.65 -> 1.05 (跨度0.4，更温和)
        // 这样相邻两层方块的颜色差异变小，线条感就会减弱
        float h_factor = 1.0f + 0.6f * (y / height_max);

        // --- 优化 2: 深度渐变微调 ---
        // 让侧面不要太黑，保持一点通透感
        float d_factor = 0.5f + 0.5f * ((z - z_base) / thickness);

        // --- 优化 3: 关键！添加“抖动” (Dithering) ---
        // 生成一个 -0.03 到 +0.03 之间的微小随机数
        // 这会打破整齐的横纹，让表面看起来像是有颗粒感的磨砂材质，而不是阶梯
        float noise = ((rand() % 100) / 100.0f - 0.5f) * 0.06f;

        // 将渐变和噪点混合
        float final_factor = h_factor * d_factor + noise;

        float final_r = r * final_factor;
        float final_g = g * final_factor;
        float final_b = b * final_factor;

        // 简单的色调映射，增加亮度
        final_r = sqrt(final_r);
        final_g = sqrt(final_g);
        final_b = sqrt(final_b);

        // 防止颜色溢出 (Clamp)
        if (final_r > 1.0f) final_r = 1.0f;
        if (final_g > 1.0f) final_g = 1.0f;
        if (final_b > 1.0f) final_b = 1.0f;
        // 防止颜色过暗
        if (final_r < 0.0f) final_r = 0.0f;
        if (final_g < 0.0f) final_g = 0.0f;
        if (final_b < 0.0f) final_b = 0.0f;

        model.push_back({ x, y, z, final_r, final_g, final_b });
        };

    // ... (后面 L, Y, T 的循环生成代码保持不变，不需要改动) ...
    // --- 1. 字母 L (基调：科技蓝 / Cyan Blue) ---
    float l_center_x = -17.0f;
    float l_r = 0.0f, l_g = 0.6f, l_b = 1.0f;

    // L 竖线
    for (float x = l_center_x - 0.5f; x <= l_center_x + 0.5f; x += voxel_size) {
        for (float y = 0.0f; y <= 5.0f; y += voxel_size) {
            for (float z = z_base; z <= z_base + thickness; z += voxel_size) {
                addVoxel(x, y, z, l_r, l_g, l_b, 5.0f);
            }
        }
    }
    // L 横线
    for (float x = l_center_x + 0.5f; x <= l_center_x + 2.5f; x += voxel_size) {
        for (float y = 0.0f; y <= 1.0f; y += voxel_size) {
            for (float z = z_base; z <= z_base + thickness; z += voxel_size) {
                addVoxel(x, y, z, l_r, l_g, l_b, 5.0f);
            }
        }
    }

    // --- 2. 字母 Y (基调：霓虹紫 / Neon Purple) ---
    float y_center_x = -12.0f;
    float y_r = 0.8f, y_g = 0.0f, y_b = 1.0f;

    // Y 下半部分
    for (float x = y_center_x - 0.5f; x <= y_center_x + 0.5f; x += voxel_size) {
        for (float y = 0.0f; y <= 2.5f; y += voxel_size) {
            for (float z = z_base; z <= z_base + thickness; z += voxel_size) {
                addVoxel(x, y, z, y_r, y_g, y_b, 5.0f);
            }
        }
    }
    // Y 上半部分 V 字
    for (float y = 2.5f; y <= 5.0f; y += voxel_size) {
        float offset = (y - 2.5f) * 0.6f;
        for (float x = y_center_x - 0.5f - offset; x <= y_center_x + 0.5f - offset; x += voxel_size) {
            for (float z = z_base; z <= z_base + thickness; z += voxel_size) {
                addVoxel(x, y, z, y_r, y_g, y_b, 5.0f);
            }
        }
        for (float x = y_center_x - 0.5f + offset; x <= y_center_x + 0.5f + offset; x += voxel_size) {
            for (float z = z_base; z <= z_base + thickness; z += voxel_size) {
                addVoxel(x, y, z, y_r, y_g, y_b, 5.0f);
            }
        }
    }

    // --- 3. 字母 T (基调：活力橙 / Hot Orange) ---
    float t_center_x = -7.0f;
    float t_r = 1.0f, t_g = 0.5f, t_b = 0.0f;

    // T 竖线
    for (float x = t_center_x - 0.5f; x <= t_center_x + 0.5f; x += voxel_size) {
        for (float y = 0.0f; y <= 4.0f; y += voxel_size) {
            for (float z = z_base; z <= z_base + thickness; z += voxel_size) {
                addVoxel(x, y, z, t_r, t_g, t_b, 5.0f);
            }
        }
    }
    // T 横梁
    for (float x = t_center_x - 2.0f; x <= t_center_x + 2.0f; x += voxel_size) {
        for (float y = 4.0f; y <= 5.0f; y += voxel_size) {
            for (float z = z_base; z <= z_base + thickness; z += voxel_size) {
                addVoxel(x, y, z, t_r, t_g, t_b, 5.0f);
            }
        }
    }

    return model;
}

// [动画升级版] 精细脸部细节 (挑眉 + 墨镜流光)
std::vector<VoxelFace> createFaceDetails(float time) {
    std::vector<VoxelFace> face;

    // 颜色定义
    float GLASS_R = 0.1f, GLASS_G = 0.1f, GLASS_B = 0.1f;
    float SKIN_R = 0.9f, SKIN_G = 0.7f, SKIN_B = 0.5f;
    float LIP_R = 0.8f, LIP_G = 0.4f, LIP_B = 0.4f;
    float HAIR_R = 0.15f, HAIR_G = 0.15f, HAIR_B = 0.15f;
    // [流光颜色] 亮白色
    float GLINT_R = 0.9f, GLINT_G = 0.9f, GLINT_B = 0.9f;

    float Z_SURFACE = 1.6f;
    float Z_POP = 1.7f;

    // 1. 精细墨镜
    // 镜片主体 (保持不变)
    for (float x = -1.3f; x <= -0.2f; x += 0.25f) {
        for (float y = 10.8f; y <= 11.3f; y += 0.25f) {
            face.push_back({ x, y, Z_POP, GLASS_R, GLASS_G, GLASS_B });
        }
    }
    for (float x = 0.2f; x <= 1.3f; x += 0.25f) {
        for (float y = 10.8f; y <= 11.3f; y += 0.25f) {
            face.push_back({ x, y, Z_POP, GLASS_R, GLASS_G, GLASS_B });
        }
    }
    face.push_back({ 0.0f, 11.2f, Z_POP, GLASS_R, GLASS_G, GLASS_B });
    for (float z = -0.8f; z <= 1.7f; z += 0.25f) {
        face.push_back({ -1.6f, 11.2f, z, GLASS_R, GLASS_G, GLASS_B });
        face.push_back({ 1.6f, 11.2f, z, GLASS_R, GLASS_G, GLASS_B });
    }

    // *** 动画 1: 墨镜流光 (Scanner Effect) ***
    // 让一个光点从左 (-1.3) 扫到右 (1.3)
    // fmod 用于循环，速度为 2.0
    float scanPos = -1.5f + fmod(time * 2.5f, 3.5f);

    // 如果光点在镜片范围内，就画出来
    // 左镜片范围
    if (scanPos >= -1.3f && scanPos <= -0.2f) {
        face.push_back({ scanPos, 11.1f, Z_POP + 0.05f, GLINT_R, GLINT_G, GLINT_B });
    }
    // 右镜片范围
    if (scanPos >= 0.2f && scanPos <= 1.3f) {
        face.push_back({ scanPos, 11.1f, Z_POP + 0.05f, GLINT_R, GLINT_G, GLINT_B });
    }


    // 2. 鼻子 & 3. 嘴巴 (保持不变)
    face.push_back({ -0.1f, 10.5f, Z_POP, SKIN_R, SKIN_G, SKIN_B });
    face.push_back({ 0.1f, 10.5f, Z_POP, SKIN_R, SKIN_G, SKIN_B });
    face.push_back({ -0.1f, 10.3f, Z_POP, SKIN_R, SKIN_G, SKIN_B });
    face.push_back({ 0.1f, 10.3f, Z_POP, SKIN_R, SKIN_G, SKIN_B });

    for (float x = -0.3f; x <= 0.3f; x += 0.2f) {
        face.push_back({ x, 9.8f, Z_SURFACE, LIP_R, LIP_G, LIP_B });
    }

    // *** 动画 2: 挑眉 (Eyebrow Raise) ***
    // 使用 sin 函数计算偏移量
    // sin(time * 5.0) 产生 -1 到 1 的波动
    // * 0.15f 限制波动幅度，避免眉毛飞出脸外
    // abs() 确保眉毛只向上挑，不向下压
    float browOffset = abs(sin(time * 4.0f)) * 0.2f;

    // 4. 眉毛 (Y坐标加上了动画偏移)
    for (float x = -0.9f; x <= -0.2f; x += 0.2f) {
        face.push_back({ x, 11.8f + browOffset, Z_SURFACE, HAIR_R, HAIR_G, HAIR_B });
    }
    for (float x = 0.2f; x <= 0.9f; x += 0.2f) {
        face.push_back({ x, 11.8f + browOffset, Z_SURFACE, HAIR_R, HAIR_G, HAIR_B });
    }

    // 5. 耳朵 & 鬓角 (保持不变)
    face.push_back({ -1.6f, 10.7f, 0.0f, SKIN_R, SKIN_G, SKIN_B });
    face.push_back({ -1.6f, 10.9f, 0.0f, SKIN_R, SKIN_G, SKIN_B });
    face.push_back({ 1.6f, 10.7f, 0.0f, SKIN_R, SKIN_G, SKIN_B });
    face.push_back({ 1.6f, 10.9f, 0.0f, SKIN_R, SKIN_G, SKIN_B });

    for (float y = 11.5f; y <= 12.5f; y += 0.25f) {
        for (float z = -0.5f; z <= 0.2f; z += 0.25f) {
            face.push_back({ -1.6f, y, z, HAIR_R, HAIR_G, HAIR_B });
        }
    }
    for (float y = 11.5f; y <= 12.5f; y += 0.25f) {
        for (float z = -0.5f; z <= 0.2f; z += 0.25f) {
            face.push_back({ 1.6f, y, z, HAIR_R, HAIR_G, HAIR_B });
        }
    }

    return face;
}


// [新增] 简单的下雪粒子系统
// 不用存储粒子状态，直接用哈希函数根据索引和时间计算位置
void drawSnow(float time) {
    // 关闭光照和纹理，确保雪花是纯白的亮色
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    // 设置点的大小
    glPointSize(3.0f);

    glBegin(GL_POINTS);
    // 生成 2000 片雪花
    for (int i = 0; i < 2000; ++i) {
        // 利用 i 制造随机种子
        float seed = (float)i;

        // X: 分布在 -50 到 50 之间
        // sin(seed * 12.9898) 是一个伪随机技巧
        float x = -50.0f + 100.0f * abs(sin(seed * 12.9898f));

        // Z: 分布在 -50 到 20 之间
        float z = -50.0f + 70.0f * abs(cos(seed * 78.233f));

        // Y: 让雪花不断下落
        // 初始高度 30，下落速度 2.0
        // fmod 让它循环，落到 -5 就回到 30
        float dropSpeed = 2.0f + sin(seed) * 0.5f; // 每个雪花速度稍微不同
        float y = 30.0f - fmod(time * dropSpeed + seed * 10.0f, 35.0f);

        // 如果雪花在视野范围内，就画出来
        if (y > -5.0f) {
            // 简单的摇摆效果 (Wind)
            float wind = sin(time * 0.5f + y * 0.1f) * 2.0f;
            glVertex3f(x + wind, y, z);
        }
    }
    glEnd();

    // 恢复光照，以免影响其他物体
    glEnable(GL_LIGHTING);
}