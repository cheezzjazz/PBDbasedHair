#include "GL\glut.h"
#include "PBD_Hair.h"
#define LOAD_MESH
//#define HAIR_STRAND
//#define HAIR_WISP_CIRCLE
//#define HAIR_WISP_LINE
bool pause = true;
int mousePosition[2];
unsigned int mouseBtnState[3];
float matTrans[3];
float matRotat[2];
PBD_Hair *myHair;
//MeshLoader *mymeshLoader;
//PBD_Hairstrand *myHair;
//vector<PBD_Hairstrand*> myHairwisp;

void Lighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float light_pos[] = { 25.0f, 50.0f, 50.0f, 0.0f };
	float light_dir[] = { 0.0f, -1.0f, -1.0f };
	float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };		//Ambient Light
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };		//Diffuse Light
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };	//Specular Light
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	float noMat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float matShininess = 100.0f;
	float frontColor[] = { 0.8f, 0.8f, 0.8f };
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
}

void Init()
{
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	mousePosition[0] = mousePosition[1] = 0;
	mouseBtnState[0] = mouseBtnState[1] = mouseBtnState[2] = 0;
	matTrans[0] = matTrans[1] = 0.0f;
	matTrans[2] = -50.0f;
	matRotat[0] = matRotat[1] = 0.0f;
}

void Initialize()
{
	Init();


#ifdef LOAD_MESH
	//LoadMesh
	myHair = new PBD_Hair("./object/sphere_low.obj", 5.0f, 20);	//Hair mesh

//	mymeshLoader->LoadMeshfile("./object/Sphere_6300.obj");
//	mymeshLoader->LoadMeshfile("./object/Sphere_12000.obj");
#endif	
#ifdef HAIR_STRAND
	myHair = new PBD_Hair(10.0f, 20);	//Hair strand
#endif
#ifdef HAIR_WISP_CIRCLE
	myHair = new PBD_Hair(20.0f, 100, 50.0f);	//Hair wisp circle
#endif
#ifdef HAIR_WISP_LINE
	myHair = new PBD_Hair(5.0f, 10, 20);	//Hair wisp line
#endif	
}

void mouse(int btn, int state, int x, int y)
{
	mousePosition[0] = x;
	mousePosition[1] = y;

	switch (btn)
	{
	case GLUT_LEFT_BUTTON:
		mouseBtnState[0] = (state == GLUT_DOWN) ? 1 : 0;
		break;
	case GLUT_RIGHT_BUTTON:
		mouseBtnState[1] = (state == GLUT_DOWN) ? 1 : 0;
		break;
	case 3:
		matTrans[2] += 1.0f;
		break;
	case 4:
		matTrans[2] -= 1.0f;
		break;
	}
}

void motion(int x, int y)
{
	int diffX = x - mousePosition[0];
	int diffY = y - mousePosition[1];
	mousePosition[0] = x;
	mousePosition[1] = y;

	if (mouseBtnState[0])	//Left Btn
	{
		//Translation
		matTrans[0] += 0.01f * (float)diffX;
		matTrans[1] -= 0.01f * (float)diffY;
	}
	else if (mouseBtnState[1])	//Right Btn
	{
		//Rotation
		matRotat[0] += 0.5f * (float)diffY;
		matRotat[1] += 0.5f * (float)diffX;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
	case 'R':
		//Init();
		myHair->Init();
		break;
	case ' ':
		pause = !pause;
		break;
	case 'a':
		myHair->ApplyWindtoHair(Vec3<float>(-10.0f, 0.0f, 0.0f));
		break;
	case 'd':
		myHair->ApplyWindtoHair(Vec3<float>(10.0f, 0.0f, 0.0f));
		break;
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / (float)h, 0.1, 100);
	glutPostRedisplay();
}

void update(int value)
{
	if (!pause)
	{
		myHair->Simulation(0.02f);
	}
	glutPostRedisplay();
	glutTimerFunc(1, update, 0);
}

void RenderSphere()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	glutWireSphere(2.5f, 20.0f, 20.0f);
	glPopMatrix();
}

void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(matTrans[0], matTrans[1], matTrans[2]);
	glRotatef(matRotat[0], 1.0f, 0.0f, 0.0f);
	glRotatef(matRotat[1], 0.0f, 1.0f, 0.0f);
	Lighting();
	
	//RenderSphere()
	myHair->Draw();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("HAIR");
	
	Initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(1, update, 0);
	glutMainLoop();
	return 0;
}