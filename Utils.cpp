#include "Utils.h"

// 绘制立方体的实现
// 使用glPushMatrix/glPopMatrix确保这个立方体的变换（如移动）不会影响到场景中的其他物体
void drawCube(float x, float y, float z, float size, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);    // 将立方体移动到指定位置
    glColor3f(r, g, b);       // 设置立方体的颜色
    glutSolidCube(size);      // 使用GLUT的内置函数绘制一个实心立方体
    // 这比手动用glBegin/glEnd画六个面要简单高效
    glPopMatrix();
}