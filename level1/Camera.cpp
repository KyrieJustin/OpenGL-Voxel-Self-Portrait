#include "Camera.h"
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <iostream> // 用于打印操作提示

Camera::Camera() {
    eyeX = 0.0f; eyeY = 10.0f; eyeZ = 30.0f;
    centerX = 0.0f; centerY = 5.0f; centerZ = 0.0f;
    upX = 0.0f; upY = 1.0f; upZ = 0.0f;

    // 默认开启自动运镜
    isFreeMode = false;
}

float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

float smoothStep(float t) {
    if (t < 0.0f) return 0.0f;
    if (t > 1.0f) return 1.0f;
    return t * t * (3.0f - 2.0f * t);
}

void Camera::update(float elapsedTime) {
    // *** 关键修改：如果是自由模式，就不执行自动运镜逻辑 ***
    if (isFreeMode) return;

    // ... (以下保留您之前的自动运镜代码，完全不用变) ...
    // --- 第1幕：宏大开场 (0s - 5s) ---
    if (elapsedTime < 5.0f) {
        float t = smoothStep(elapsedTime / 5.0f);
        eyeX = lerp(0.0f, -15.0f, t);
        eyeY = lerp(30.0f, 5.0f, t);
        eyeZ = lerp(50.0f, 20.0f, t);
        centerX = 0.0f; centerY = 4.0f; centerZ = 0.0f;
    }
    // --- 第2幕：名字特写 (5s - 10s) ---
    else if (elapsedTime < 10.0f) {
        float t = smoothStep((elapsedTime - 5.0f) / 5.0f);
        eyeX = lerp(-15.0f, 0.0f, t);
        eyeY = lerp(5.0f, 6.0f, t);
        eyeZ = lerp(20.0f, 15.0f, t);
        centerX = lerp(-15.0f, -1.5f, t);
        centerY = lerp(4.0f, 8.0f, t);
        centerZ = lerp(8.0f, -2.0f, t);
    }
    // --- 第3幕：主角环绕展示 (10s - 17s) ---
    else if (elapsedTime < 17.0f) {
        float t = (elapsedTime - 10.0f) / 7.0f;
        float targetX = -1.5f; float targetY = 8.0f; float targetZ = -2.0f;
        float radius = 15.0f;
        float angle = 0.2f * 3.14159f - t * 3.14159f;
        eyeX = targetX + sin(angle) * radius;
        eyeZ = targetZ + cos(angle) * radius;
        eyeY = 8.0f + sin(t * 3.14f) * 2.0f;
        centerX = targetX; centerY = targetY; centerZ = targetZ;
    }
    // --- 第4幕：面部与细节特写 (17s - 22s) ---
    else if (elapsedTime < 22.0f) {
        float t = smoothStep((elapsedTime - 17.0f) / 5.0f);
        if (t < 0.5f) {
            float sub_t = t * 2.0f;
            eyeX = lerp(-5.0f, -0.5f, sub_t);
            eyeY = lerp(8.0f, 11.0f, sub_t);
            eyeZ = lerp(8.0f, 2.0f, sub_t);
        }
        else {
            float sub_t = (t - 0.5f) * 2.0f;
            eyeX = lerp(-0.5f, -4.0f, sub_t);
            eyeY = lerp(11.0f, 7.0f, sub_t);
            eyeZ = lerp(2.0f, 5.0f, sub_t);
        }
        centerX = -1.5f;
        centerY = lerp(11.0f, 7.0f, t);
        centerZ = -2.0f;
    }
    // --- 第5幕：结尾定格 (22s +) ---
    else {
        float drift = (elapsedTime - 22.0f) * 0.5f;
        eyeX = -10.0f - drift;
        eyeY = 8.0f + drift * 0.2f;
        eyeZ = 15.0f + drift;
        centerX = -1.5f; centerY = 6.0f; centerZ = -2.0f;
    }
}

// [新增] 键盘控制逻辑
void Camera::handleKey(unsigned char key) {
    // 切换模式 (C键)
    if (key == 'c' || key == 'C') {
        isFreeMode = !isFreeMode;
        std::cout << (isFreeMode ? "Free Mode ON" : "Auto Mode ON") << std::endl;
        return;
    }

    // 如果不是自由模式，不响应移动键
    if (!isFreeMode) return;

    // 计算当前的视线向量 (Forward Vector)
    float dx = centerX - eyeX;
    float dz = centerZ - eyeZ;
    // 归一化，保持速度一致
    float length = sqrt(dx * dx + dz * dz);
    if (length == 0) return; // 防止除以0
    dx /= length;
    dz /= length;

    // 计算右侧向量 (Right Vector) - 用于平移
    // 向量 (x, z) 旋转 90 度变成 (-z, x)
    float rx = -dz;
    float rz = dx;

    float moveSpeed = 1.0f;   // 移动速度
    float rotSpeed = 0.1f;    // 旋转速度

    switch (key) {
        // --- 移动 (WASD) ---
    case 'w': case 'W': // 前进
        eyeX += dx * moveSpeed; eyeZ += dz * moveSpeed;
        centerX += dx * moveSpeed; centerZ += dz * moveSpeed;
        break;
    case 's': case 'S': // 后退
        eyeX -= dx * moveSpeed; eyeZ -= dz * moveSpeed;
        centerX -= dx * moveSpeed; centerZ -= dz * moveSpeed;
        break;
    case 'a': case 'A': // 向左平移
        eyeX -= rx * moveSpeed; eyeZ -= rz * moveSpeed;
        centerX -= rx * moveSpeed; centerZ -= rz * moveSpeed;
        break;
    case 'd': case 'D': // 向右平移
        eyeX += rx * moveSpeed; eyeZ += rz * moveSpeed;
        centerX += rx * moveSpeed; centerZ += rz * moveSpeed;
        break;

        // --- 旋转视角 (Q/E) ---
    case 'q': case 'Q': // 向左转
        // 围绕 eye 旋转 center
    {
        float newDx = dx * cos(-rotSpeed) - dz * sin(-rotSpeed);
        float newDz = dx * sin(-rotSpeed) + dz * cos(-rotSpeed);
        centerX = eyeX + newDx * length;
        centerZ = eyeZ + newDz * length;
    }
    break;
    case 'e': case 'E': // 向右转
    {
        float newDx = dx * cos(rotSpeed) - dz * sin(rotSpeed);
        float newDz = dx * sin(rotSpeed) + dz * cos(rotSpeed);
        centerX = eyeX + newDx * length;
        centerZ = eyeZ + newDz * length;
    }
    break;

    // --- 升降 (R/F) ---
    case 'r': case 'R': // 升高
        eyeY += moveSpeed; centerX += moveSpeed;
        break;
    case 'f': case 'F': // 降低
        eyeY -= moveSpeed; centerX -= moveSpeed;
        break;
    }
}

void Camera::applyView() {
    gluLookAt(eyeX, eyeY, eyeZ,
        centerX, centerY, centerZ,
        upX, upY, upZ);
}