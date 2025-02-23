#include <GL/glut.h>
#include <math.h>
GLUquadricObj* cyl;

float speed = 5.0f;//����� ����Ǵ� �ӵ�

float t = 0; //����� �ð�
float am = 0; //a�� ������

bool contact = false; //A�� B�� �������� ����
float ready = 0; //B�� ������ �غ��ϴ� ������

bool swing = false;//������ �ϴ��� ����
float sw = 0;//������ �ϴ� ������

bool flying = false;//A�� ���ư����� ����
float f = 0;//A�� ���ư��� ������

bool secondhit = false;//A�� �¾翡 �¾Ҵ��� ����
float sechit = 0;//A�� �ٽ� ���ư��� ������

bool reset = false;//��� ������ ���� ����

//���� �Լ�. ��� ������ �ʱ�ȭ�ϸ� ����� �ٽ� ���۵Ǵ� �Ͱ� ���� ȿ��
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
	
		t = t + speed;//����� �����

		if (0.7 - t * 0.0001 < -0.25) {//A�� B�� �Ÿ��� ���� �Ÿ� �̻� ���������
			contact = true;//���� ���� ������ ���
		}
		else { am = t; } //�׷��� ���� ��쿡�� t��ŭ A�� ������

		//������ ���, B�� ���� �غ� ����
		if (contact) { ready += speed; }

		//������ �غ�Ǿ��� ���, ������ ����
		if (ready * 0.00007 > 0.5 && !swing) { swing = true; }
		if (swing) { sw += speed; }

		//������ �Ϸ�Ǿ��� ���, A�� ���ư��� ����
		if ((sw * 0.0037) > 1 && !flying) { flying = true; }
		if (flying && !secondhit) { f += speed; }

		//�¾�� ������ �ٸ� �������� ���ư��� ����
		if (1 - t * 0.000021 - f * 0.0002 < 0) { secondhit = true; }
		if (secondhit) { 
			sechit += speed; 
			//�̶� ���� ������ true�� �ٲٰ�
			if (!reset&&swing) { reset = true; swing = false; }
		}
		//Reset()�Լ��� 1.5�� �ڿ� 1���� ����
		if (reset) { glutTimerFunc(1500, Reset, 1); reset = false; }

	glutPostRedisplay();
	glutTimerFunc(1,MyTimer,1);
}


void MyDisplay() {sss
	cyl = gluNewQuadric();
	gluQuadricDrawStyle(cyl, GLU_LINE);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//�ϴ�
	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // -90
	glColor3f(0, 0.6, 1);
	glTranslatef(-0.5, -0.5, -0.5);
	glScalef(5, 0.3, 5);
	glutSolidCube(0.6);
	glPopMatrix();
	//��
	glPushMatrix();
	glColor3f(1.0, 0.6, 0.3);
	glTranslatef(0, -0.5, 0);
	glScalef(5, 0.3, 5);
	glutSolidCube(0.6);
	glPopMatrix();
	//�¾�
	glPushMatrix();
	if (flying) { //A�� ���ư��� �����ϸ�
		if (secondhit) {//A�� ������ ��쿡�� �ݴ�������� ������ ������
			glTranslatef(1 - t * 0.000021 - f * 0.0002+ sechit*0.003, 0.4 + t * 0.00001, 0);
		}
		else {//A�� ������ ������ ���� �� ������ ������
			glTranslatef(1 - t * 0.000021-f*0.0002, 0.4 + t * 0.00001, 0);
		}
	}
	else {//ó������ õõ�� ������
		glTranslatef(1 - t * 0.000021, 0.4 + t * 0.00001, 0);
	}
	
	glRotatef(-90, 1, 0, 0); // -90
	glColor3f(1, 0.5, 0);
	
	glutSolidSphere(0.07, 20, 20);
	//�¾� �ȵ�
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
		glRotatef((t * 0.01) + i*45, 1, 90, 0);
		gluCylinder(cyl, 0.01, 0.01, 0.11, 200, 1);
		glPopMatrix();
	}
	
	
	
	glPopMatrix();
	
	//A
	
	//(n)�� ���̸� ��Ÿ��
	
	//����(1)
	glPushMatrix();
	
	if(contact){ //B�� ������ ���
		if (secondhit) {//�¾�� �浹���� ��쿡 �������� ������ ������
			glTranslatef(-0.25 + f * 0.00037 - sechit*0.003, -0.2 + f * 0.00035 - sechit*0.0003, 0.5 - f * 0.001);
		}
		else {//���ư��� ������ �̵�
			glTranslatef(-0.25 + f * 0.00037, -0.2 + f * 0.00035, 0.5 - f * 0.001);
		}
	}
	//B�� ������ ������ B�� ���� �̵���
	else { glTranslatef(0.7 - (am * 0.0001), -0.2, 0.5); }
	glColor3f(0, 0, 0);
	glRotatef(-90 + f, 1, 0, 0); // ���ư��� ���߿��� �ſ� ������ ȸ����
	 // Cynliner�� +z�������� �������.
	 gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 5);
	
		// ��1(2)
		glPushMatrix();
		 glTranslatef(0, 0.1, 0.2);
		 glutSolidSphere(0.025, 10, 10);
		 glRotatef(180 + sin(am * 0.001) * 40, 1, 90, 0);
		 gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
			//��1(3)
		 glPushMatrix();
		 glTranslatef(0, 0, 0.15);
		 glutSolidSphere(0.025, 10, 10);
		 glPopMatrix();
		glPopMatrix();
		// ��2(2)
		glPushMatrix();
		glTranslatef(0, -0.1, 0.2);
		glutSolidSphere(0.025, 10, 10);
		glRotatef(180 - sin(am * 0.001) * 40, 1, 90, 0);
		gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
				//��1(3)
		glPushMatrix();
		glTranslatef(0, 0, 0.15);
		glutSolidSphere(0.025, 10, 10);
		glPopMatrix();
		glPopMatrix();
		//��(2)
		glPushMatrix();
		glTranslatef(0, 0, 0.2);
		gluCylinder(cyl, 0.025, 0.025, 0.1, 50, 1);
			//�Ӹ�(3)
		glPushMatrix();
		glTranslatef(0, 0, 0.1);
		glutSolidSphere(0.05, 40, 40);
		glPopMatrix(); glPopMatrix();

		// �ٸ�1(2)
		glPushMatrix();
		glTranslatef(0, 0.08, 0);
		glutSolidSphere(0.025, 10, 10);
		glRotatef(180 - sin(am * 0.001) * 25, 1, 90, 0);
		gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
		glPopMatrix();
		// �ٸ�2(2)
		glPushMatrix();
		glTranslatef(0, -0.08, 0);
		glutSolidSphere(0.025, 10, 10);
		glRotatef(180 + sin(am * 0.001) * 25, 1, 90, 0);
		gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
		glPopMatrix();
	
	glPopMatrix(); // ���� ��


	//B

	 //����(1)
	glPushMatrix();
	glTranslatef(-0.6, -0.2, 0.5);
	glColor3f(1, 0, 0);
	if (contact) {//A�� ���� ���
		if (flying) {//A�� ���ư��� �ִ� ���
			if (secondhit) {//A�� �¾�� ���� ���
				if (-0.5 + sechit * 0.0003 > 0) {
					//���� ���·� ���ƿ� ��� �� ���¸� ������
					glRotatef(-90, 1, 0, 0); 
				}
				else { //������ ���� ���·� ���ư�
					glRotatef(-90, 1, -0.5 + sechit * 0.0003, -0.5 + sechit * 0.0003); // -90
				}
			}
			else {//������ ��ģ ��� �� ���·� ����
				glRotatef(-90, 1, -0.5, -0.5); // -90
			}
		}
		
		else { //�غ��� ��ŭ ���� ���������� ������, ������ ���� �������� ����
			glRotatef(-90, 1, 0 + ready * 0.00007 - sw * 0.0037, 0 + ready * 0.00007 - sw * 0.0037); // -90
		}
	}
	
	else { //������ ���� ��쿡�� ���
		glRotatef(-90, 1, 0, 0); // -90
	}
	// Cynliner�� +z�������� �������.
	gluCylinder(cyl, 0.05, 0.05, 0.2, 20, 5);
	


		//��ġ ����(2)
	glPushMatrix();
	glTranslatef(0.15, 0,0.15);
	glRotatef(-90, 1, 0, 0); // -90
	glTranslatef(0, 0.03, -0.3);
	gluCylinder(cyl, 0.015, 0.015, 0.4, 70, 5);
			//3��ġ �Ӹ�(3)
	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // -90
	glTranslatef(0, 0, -0.04);
	
	gluCylinder(cyl, 0.035, 0.035, 0.08, 170, 5);

	glPopMatrix(); glPopMatrix();

		// ��1(2)
	glPushMatrix();
	glTranslatef(0, 0.1, 0.2);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(120, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
			//��1(3)
	glPushMatrix();
	glTranslatef(0, 0, 0.15);
	glutSolidSphere(0.025, 10, 10);
	glPopMatrix(); glPopMatrix();
		// ��2(2)
	glPushMatrix();
	glTranslatef(0, -0.1, 0.2);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(120, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
			//��2(3)
	glPushMatrix();
	glTranslatef(0, 0, 0.15);
	glutSolidSphere(0.025, 10, 10);
	glPopMatrix();
	glPopMatrix();
		//��(2)
	glPushMatrix();
	glTranslatef(0, 0, 0.2);
	gluCylinder(cyl, 0.025, 0.025, 0.1, 5, 1);
			//�Ӹ�(3)
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	glutSolidSphere(0.05, 40, 40);
	glPopMatrix(); glPopMatrix();

		// �ٸ�1(2)
	glPushMatrix();
	glTranslatef(0, 0.08, 0);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(180, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
	glPopMatrix();
		// �ٸ�2(2)
	glPushMatrix();
	glTranslatef(0, -0.08, 0);
	glutSolidSphere(0.025, 10, 10);
	glRotatef(180, 1, 90, 0);
	gluCylinder(cyl, 0.025, 0.025, 0.15, 20, 1);
	glPopMatrix();

	glPopMatrix(); // ���� ��


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