#include <GL/glut.h>
#include <math.h>
GLUquadricObj* cyl;

float speed = 5.0f;//장면이 진행되는 속도

float t = 0; //경과된 시간
float am = 0; //a의 움직임

bool contact = false; //A와 B가 만났는지 여부
float ready = 0; //B가 스윙을 준비하는 움직임

bool swing = false;//스윙을 하는지 여부
float sw = 0;//스윙을 하는 움직임

bool flying = false;//A가 날아가는지 여부
float f = 0;//A가 날아가는 움직임

bool secondhit = false;//A가 태양에 맞았는지 여부
float sechit = 0;//A가 다시 날아가는 움직임

bool reset = false;//장면 리셋을 위한 변수

//리셋 함수. 모든 변수를 초기화하면 장면이 다시 시작되는 것과 같은 효과
void Reset(int n) { 
	f = 0;
	t = 0;
	am = 0;
	ready = 0;
	sw = 0;
	sechit = 0;
	contact = false;
	swing = false;
	flying = false;
	secondhit = false;
}

void MyTimer(int n)
{
	
		t = t + speed;//장면이 진행됨

		if (0.7 - t * 0.0001 < -0.25) {//A와 B의 거리가 일정 거리 이상 가까워지면
			contact = true;//둘이 만난 것으로 취급
		}
		else { am = t; } //그렇지 않은 경우에는 t만큼 A가 움직임

		//만났을 경우, B가 스윙 준비를 시작
		if (contact) { ready += speed; }

		//스윙이 준비되었을 경우, 스윙을 시작
		if (ready * 0.00007 > 0.5 && !swing) { swing = true; }
		if (swing) { sw += speed; }

		//스윙이 완료되었을 경우, A가 날아가기 시작
		if ((sw * 0.0037) > 1 && !flying) { flying = true; }
		if (flying && !secondhit) { f += speed; }

		//태양과 만나면 다른 방향으로 날아가기 시작
		if (1 - t * 0.000021 - f * 0.0002 < 0) { secondhit = true; }
		if (secondhit) { 
			sechit += speed; 
			//이때 리셋 변수를 true로 바꾸고
			if (!reset&&swing) { reset = true; swing = false; }
		}
		//Reset()함수를 1.5초 뒤에 1번만 실행
		if (reset) { glutTimerFunc(1500, Reset, 1); reset = false; }

	glutPostRedisplay();
	glutTimerFunc(1,MyTimer,1);
}


void MyDisplay() {sss
	cyl = gluNewQuadric();
	gluQuadricDrawStyle(cyl, GLU_LINE);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//하늘
	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // -90
	glColor3f(0, 0.6, 1);
	glTranslatef(-0.5, -0.5, -0.5);
	glScalef(5, 0.3, 5);
	glutSolidCube(0.6);
	glPopMatrix();
	//땅
	glPushMatrix();
	glColor3f(1.0, 0.6, 0.3);
	glTranslatef(0, -0.5, 0);
	glScalef(5, 0.3, 5);
	glutSolidCube(0.6);
	glPopMatrix();
	//태양
	glPushMatrix();
	if (flying) { //A가 날아가기 시작하면
		if (secondhit) {//A와 만났을 경우에는 반대방향으로 빠르게 움직임
			glTranslatef(1 - t * 0.000021 - f * 0.0002+ sechit*0.003, 0.4 + t * 0.00001, 0);
		}
		else {//A와 만나기 전에는 조금 더 빠르게 움직임
			glTranslatef(1 - t * 0.000021-f*0.0002, 0.4 + t * 0.00001, 0);
		}
	}
	else {//처음에는 천천히 움직임
		glTranslatef(1 - t * 0.000021, 0.4 + t * 0.00001, 0);
	}
	
	glRotatef(-90, 1, 0, 0); // -90
	glColor3f(1, 0.5, 0);
	
	glutSolidSphere(0.07, 20, 20);
	//태양 팔들
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
		glRotatef((t * 0.01) + i*45, 1, 90, 0);
		gluCylinder(cyl, 0.01, 0.01, 0.11, 200, 1);
		glPopMatrix();
	}
	
	
	
	glPopMatrix();
	
	//A
	
	//(n)은 깊이를 나타냄
	
	//몸통(1)
	glPushMatrix();
	
	if(contact){ //B와 만났을 경우
		if (secondhit) {//태양과 충돌했을 경우에 왼쪽으로 빠르게 움직임
			glTranslatef(-0.25 + f * 0.00037 - sechit*0.003, -0.2 + f * 0.00035 - sechit*0.0003, 0.5 - f * 0.001);
		}
		else {//날아가는 도중의 이동
			glTranslatef(-0.25 + f * 0.00037, -0.2 + f * 0.00035, 0.5 - f * 0.001);
		}
	}
	//B와 만나기 전에는 B를 향해 이동함
	else { glTranslatef(0.7 - (am * 0.0001), -0.2, 0.5); }
	glColor3f(0, 0, 0);
	glRotatef(-90 + f, 1, 0, 0); // 날아가는 도중에는 매우 빠르게 회전함
	 // Cynliner는 +z방향으로 길어진다.
	 gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 5);
	
		// 팔1(2)
		glPushMatrix();
		 glTranslatef(0, 0.1, 0.2);
		 glutSolidSphere(0.025, 10, 10);
		 glRotatef(180 + sin(am * 0.001) * 40, 1, 90, 0);
		 gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
			//손1(3)
		 glPushMatrix();
		 glTranslatef(0, 0, 0.15);
		 glutSolidSphere(0.025, 10, 10);
		 glPopMatrix();
		glPopMatrix();
		// 팔2(2)
		glPushMatrix();
		glTranslatef(0, -0.1, 0.2);
		glutSolidSphere(0.025, 10, 10);
		glRotatef(180 - sin(am * 0.001) * 40, 1, 90, 0);
		gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
				//손1(3)
		glPushMatrix();
		glTranslatef(0, 0, 0.15);
		glutSolidSphere(0.025, 10, 10);
		glPopMatrix();
		glPopMatrix();
		//목(2)
		glPushMatrix();
		glTranslatef(0, 0, 0.2);
		gluCylinder(cyl, 0.025, 0.025, 0.1, 50, 1);
			//머리(3)
		glPushMatrix();
		glTranslatef(0, 0, 0.1);
		glutSolidSphere(0.05, 40, 40);
		glPopMatrix(); glPopMatrix();

		// 다리1(2)
		glPushMatrix();
		glTranslatef(0, 0.08, 0);
		glutSolidSphere(0.025, 10, 10);
		glRotatef(180 - sin(am * 0.001) * 25, 1, 90, 0);
		gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
		glPopMatrix();
		// 다리2(2)
		glPushMatrix();
		glTranslatef(0, -0.08, 0);
		glutSolidSphere(0.025, 10, 10);
		glRotatef(180 + sin(am * 0.001) * 25, 1, 90, 0);
		gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
		glPopMatrix();
	
	glPopMatrix(); // 몸통 끝


	//B

	 //몸통(1)
	glPushMatrix();
	glTranslatef(-0.6, -0.2, 0.5);
	glColor3f(1, 0, 0);
	if (contact) {//A와 만난 경우
		if (flying) {//A가 날아가고 있는 경우
			if (secondhit) {//A가 태양과 만난 경우
				if (-0.5 + sechit * 0.0003 > 0) {
					//원래 상태로 돌아온 경우 그 상태를 고정함
					glRotatef(-90, 1, 0, 0); 
				}
				else { //서서히 원래 상태로 돌아감
					glRotatef(-90, 1, -0.5 + sechit * 0.0003, -0.5 + sechit * 0.0003); // -90
				}
			}
			else {//스윙을 마친 경우 그 상태로 고정
				glRotatef(-90, 1, -0.5, -0.5); // -90
			}
		}
		
		else { //준비한 만큼 몸을 오른쪽으로 돌리고, 스윙할 때는 왼쪽으로 돌림
			glRotatef(-90, 1, 0 + ready * 0.00007 - sw * 0.0037, 0 + ready * 0.00007 - sw * 0.0037); // -90
		}
	}
	
	else { //만나지 못한 경우에는 대기
		glRotatef(-90, 1, 0, 0); // -90
	}
	// Cynliner는 +z방향으로 길어진다.
	gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 5);
	


		//망치 막대(2)
	glPushMatrix();
	glTranslatef(0.15, 0,0.15);
	glRotatef(-90, 1, 0, 0); // -90
	glTranslatef(0, 0.03, -0.3);
	gluCylinder(cyl, 0.015, 0.015, 0.4, 70, 5);
			//3망치 머리(3)
	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // -90
	glTranslatef(0, 0, -0.04);
	
	gluCylinder(cyl, 0.035, 0.035, 0.08, 170, 5);

	glPopMatrix(); glPopMatrix();

		// 팔1(2)
	glPushMatrix();
	glTranslatef(0, 0.1, 0.2);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(120, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
			//손1(3)
	glPushMatrix();
	glTranslatef(0, 0, 0.15);
	glutSolidSphere(0.025, 10, 10);
	glPopMatrix(); glPopMatrix();
		// 팔2(2)
	glPushMatrix();
	glTranslatef(0, -0.1, 0.2);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(120, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
			//손2(3)
	glPushMatrix();
	glTranslatef(0, 0, 0.15);
	glutSolidSphere(0.025, 10, 10);
	glPopMatrix();
	glPopMatrix();
		//목(2)
	glPushMatrix();
	glTranslatef(0, 0, 0.2);
	gluCylinder(cyl, 0.025, 0.025, 0.1, 5, 1);
			//머리(3)
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	glutSolidSphere(0.05, 40, 40);
	glPopMatrix(); glPopMatrix();

		// 다리1(2)
	glPushMatrix();
	glTranslatef(0, 0.08, 0);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(180, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
	glPopMatrix();
		// 다리2(2)
	glPushMatrix();
	glTranslatef(0, -0.08, 0);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(180, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
	glPopMatrix();

	glPopMatrix(); // 몸통 끝


	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("1971261 KimJeongSeok");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20, 1, 0.1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-1.5, 0.1, 4, 0, 0, 0, 0, 1, 0);

	glutDisplayFunc(MyDisplay);
	glutTimerFunc(1, MyTimer, 1);
	glutMainLoop();
	return 0;
}