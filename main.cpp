#include <GL/glut.h>
#include <vector>
#include <cmath>

#include "Voxel.h"
#include "Utils.h"
#include "Models.h"
#include "Camera.h"

// --- 全局变量 ---
Camera g_camera;
std::vector<Voxel> selfPortraitModel;
std::vector<Voxel> landscapeModel;
std::vector<VoxelWatch> watchModel;
std::vector<VoxelFace> faceModel;
std::vector<VoxelName> nameModel;
float g_nameYOffset = 0.0f; // 用于名字上下浮动的动画

// --- 函数声明 ---
void init();
void display();
void reshape(int w, int h);
void timer(int value);
void keyboard(unsigned char key, int x, int y);

// --- 主函数 ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("EBU6231 - OpenGL Project");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // [新增] 注册键盘事件
    glutKeyboardFunc(keyboard);

    glutTimerFunc(0, timer, 0); // 启动动画循环

    glutMainLoop();
    return 0;
}

// --- 初始化函数 ---
void init() {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // 天蓝色背景
    glEnable(GL_DEPTH_TEST); // 开启深度测试，让物体有正确的遮挡关系

    // --- 升级后的光照和材质 (Stretch Feature) ---
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH); // 使用平滑着色
    // --- 雾效 (Fog) - 增加景深和氛围 ---
    glEnable(GL_FOG); // 开启雾效
    {
        // 雾的颜色：设为天蓝色，与背景融合
        GLfloat fogColor[] = { 0.5f, 0.7f, 1.0f, 1.0f };
        glFogfv(GL_FOG_COLOR, fogColor);

        // 雾的模式：EXP2 (指数平方) 比较自然，越远越浓
        glFogi(GL_FOG_MODE, GL_EXP2);

        // 雾的密度：0.015 是一个比较舒服的值，既能看清人，又能模糊远山
        // 如果觉得雾太浓，改小一点 (如 0.01)；太淡，改大一点 (如 0.02)
        glFogf(GL_FOG_DENSITY, 0.015f);

        // 提示：开启雾效后，最好把背景色 (glClearColor) 也设为同样的颜色
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    }

    // 设置光源属性
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // 环境光，让暗部不至于全黑
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 漫反射光，主光源
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 镜面反射光，高光
    GLfloat light_position[] = { 10.0f, 10.0f, 10.0f, 0.0f }; // 光源位置（定向光）

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 启用颜色追踪材质，这样glColor仍然可以影响物体颜色，但更受光照影响
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // 设置材质的镜面反射属性，让物体有一点高光
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // 加载所有模型数据
    selfPortraitModel = createSelfPortraitModel();
    watchModel = createWatchModel(); // 加载手表模型
    nameModel = createDetailedNameModel();
    landscapeModel = createLandscapeModel();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    g_camera.applyView();

    // 获取时间
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // 1. 绘制静态景观
    for (const auto& voxel : landscapeModel) {
        drawCube(voxel.x, voxel.y, voxel.z, 1.0f, voxel.r, voxel.g, voxel.b);
    }

    // 2. 绘制动态水面
    drawAnimatedWater(time);

    // [新增]  绘制下雪效果
    drawSnow(time);

    // --- 人物 ---
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, -2.0f);

    // *** 动画 3: 呼吸感 (Breathing) ***
    // 让整个身体在 Y 轴方向极其微小地伸缩 (1.0 ~ 1.02)
    // 模拟呼吸的起伏，非常细腻
    float breathScale = 1.0f + sin(time * 2.0f) * 0.01f;
    glScalef(1.0f, breathScale, 1.0f);

    // 3. 自画像 (身体)
    for (const auto& voxel : selfPortraitModel) {
        drawCube(voxel.x, voxel.y, voxel.z, 1.0f, voxel.r, voxel.g, voxel.b);
    }
    // 4. 手表
    for (const auto& watch : watchModel) {
        drawCube(watch.x, watch.y, watch.z, 0.2f, watch.r, watch.g, watch.b);
    }

    // *** 关键修改：每一帧实时生成脸部细节 ***
    // 传入 time，获取当前这一帧眉毛和流光应该在的位置
    std::vector<VoxelFace> currentFace = createFaceDetails(time);

    // 5. 脸部细节 (绘制新生成的 currentFace)
    for (const auto& f : currentFace) {
        drawCube(f.x, f.y, f.z, 0.25f, f.r, f.g, f.b);
    }
    glPopMatrix();

    // 6. 名字
    glPushMatrix();
    glTranslatef(0.0f, g_nameYOffset, 0.0f);
    for (const auto& voxel : nameModel) {
        drawCube(voxel.x, voxel.y, voxel.z, 0.25f, voxel.r, voxel.g, voxel.b);
    }
    glPopMatrix();

    glutSwapBuffers();
}
// 键盘回调函数
void keyboard(unsigned char key, int x, int y) {
    g_camera.handleKey(key);
    glutPostRedisplay(); // 按键后立即重绘，保证反应灵敏
}

// --- 窗口重塑函数 ---
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// --- 动画与更新循环 ---
void timer(int value) {
    float elapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // 获取总时间（秒）

    // 更新摄像机位置
    g_camera.update(elapsedTime);

    // 更新名字浮动动画
    g_nameYOffset = sin(elapsedTime * 2.0f) * 0.5f; // 调整速度和幅度

    glutPostRedisplay(); // 请求重绘窗口
    glutTimerFunc(16, timer, 0); // 大约60 FPS
}