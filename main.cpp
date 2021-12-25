#include "FundLibs/BarakWinHelper/Barak.h"
#include "FundLibs/BarakWinHelper/Win/Keys.h"
#include "FundLibs/ObjectsGL/Shader.h"
#include <chrono>
#include <vector>

//glad 4.6

float ff(int g) { return (float)g; }

void prepr(hWindow* g_Win){
	g_Win->rename(L"Graphical panel");
}

int main() {
	srand(time(0));

	program prog;
	prog.setShaderFVG("Shaders/main.vert.glsl", 0);
	prog.setShaderFVG("Shaders/main.frag.glsl", 1);
	prog.create();
	prog.use();

	size_t XX = 16, YY = 64, ZZ = 16;

	GLuint* voxels = new GLuint[XX*YY*ZZ];

	for (size_t i = 0; i < XX; i++)
		for (size_t j = 0; j < YY; j++)
			for (size_t k = 0; k < ZZ; k++)
				voxels[i*YY*ZZ+j*ZZ+k] = rand()%4;

	glUniform1ui(1, XX);
	glUniform1ui(2, YY);
	glUniform1ui(3, ZZ);

	GLuint voxels_buffer;
	glGenBuffers(1, &voxels_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxels_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint)*XX*YY*ZZ, voxels, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, voxels_buffer);

	float speed = 10;
	float angSpeed = 1;
	char vel[3];
	float pos[3] = { 0, 50, 0 };
	char angVel[2] = { 0, 0 };
	float ang[2] = { 0, 0 };
	float dt;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	while (threadIsLive()) {
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> dTime = tp2 - tp1;
		tp1 = tp2;
		float dt = dTime.count();
		
		KeyUpdate();
		if (GetKey(VK_ESCAPE).bHeld)
			return 1;
		angVel[0] = 0; angVel[1] = 0;
		if (GetKey(VK_LEFT).bHeld)
			angVel[0] -= 1;
		if (GetKey(VK_RIGHT).bHeld)
			angVel[0] += 1;
		ang[0] += ((float)angVel[0]) * angSpeed * dt;
		if (GetKey(VK_UP).bHeld && ang[1] < 1.57079632f)
			angVel[1] += 1;
		if (GetKey(VK_DOWN).bHeld && ang[1] > -1.57079632f)
			angVel[1] -= 1;
		ang[1] += ((float)angVel[1]) * angSpeed * dt;

		if (ang[0] > 3.14159265f)
			ang[0] -= 6.2832853f;
		if (ang[0] < -3.14159265f)
			ang[0] += 6.2832853f;
		vel[0] = 0;
		vel[1] = 0;
		vel[2] = 0;
		if (GetKey('W').bHeld)
			vel[2] += 1;
		if (GetKey('A').bHeld)
			vel[0] -= 1;
		if (GetKey('S').bHeld)
			vel[2] -= 1;
		if (GetKey('D').bHeld)
			vel[0] += 1;
		if (GetKey(VK_SHIFT).bHeld)
			vel[1] -= 1;
		if (GetKey(VK_SPACE).bHeld)
			vel[1] += 1;
		pos[0] += (((float)vel[0])*cos(ang[0]) + ((float)vel[2])*sin(ang[0]))*speed*dt;
		pos[1] += ((float)vel[1])*speed*dt;
		pos[2] += (((float)vel[2])*cos(ang[0]) - ((float)vel[0])*sin(ang[0]))*speed*dt;

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColorRGB(0, 0, 0);

		glUniform3f(4, pos[0], pos[1], pos[2]);
		glUniform1f(0, ff(getWind().getW())/ff(getWind().getH()));
		glUniform2f(5, ang[0], ang[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SwapBuffers();
	}
	return 99;
}