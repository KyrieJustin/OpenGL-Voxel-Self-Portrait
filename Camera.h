#ifndef CAMERA_H
#define CAMERA_H

// 摄像机类，负责管理视图变换
class Camera {
public:
    // 摄像机的位置
    float eyeX, eyeY, eyeZ;
    // 摄像机看向的目标点
    float centerX, centerY, centerZ;
    // 摄像机的“上”方向
    float upX, upY, upZ;
    // [新增] 自由模式开关
    bool isFreeMode;

public:
    Camera(); // 构造函数，初始化摄像机

    // 根据当前时间更新摄像机的位置和朝向，以实现动态运镜
    void update(float elapsedTime);

    // 将摄像机的视图应用到场景中
    void applyView();

    // [新增] 处理键盘输入
    void handleKey(unsigned char key);
};

#endif // CAMERA_H