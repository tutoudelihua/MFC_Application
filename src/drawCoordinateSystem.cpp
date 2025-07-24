#include "pch.h"

#include "drawCoordinateSystem.h"
#include <easyx.h>
#include <graphics.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

void coordinateSystemInit(CoordinateSystem* cs) {
	cs->length = 1280;
	cs->width = 780;

	cs->axis.xhigh = 100;
	cs->axis.xlow = -100;
	cs->axis.yhigh = 100;
	cs->axis.ylow = 0;
	cs->axis.centerx = (cs->axis.xhigh + cs->axis.xlow) / 2;
	cs->axis.centery = (cs->axis.yhigh + cs->axis.ylow) / 2;
	int len1 = cs->axis.xhigh - cs->axis.xlow;
	int len2 = cs->axis.yhigh - cs->axis.ylow;
	cs->axis.ax = cs->length * 1.0 / len1;
	cs->axis.ay = cs->width * 1.0 / len2;
	cs->axis.color = BLACK;

	cs->zerox = 0;
	cs->zeroy = 50;

	cs->scale.openflag = 1;
	cs->scale.scalex = 10;
	cs->scale.scaley = 10;
	cs->scale.color = BLACK;

	cs->grid.openflag = 1;
	cs->grid.color = BLACK;

	cs->borderx = 10;
	cs->brodery = 10;
	cs->color = WHITE;

	HWND hwnd = initgraph(cs->length + cs->borderx, cs->width + cs->brodery, EW_SHOWCONSOLE);
	setrop2(R2_MASKPEN);
}

int transX(CoordinateSystem* cs, double x) {
	x -= cs->axis.centerx;
	x *= cs->axis.ax;
	int k = x;
	if (x - k > 0.5)
		k++;
	return k;
}
int transY(CoordinateSystem* cs, double y) {
	y -= cs->axis.centery;
	y *= cs->axis.ay;
	int k = y;
	if (y - k > 0.5)
		k++;
	return k;
}

void createCS(CoordinateSystem* cs) {

	int avex = (cs->length + cs->borderx) / 2;
	int avey = (cs->width + cs->brodery) / 2;
	setorigin(avex, avey);

	setaspectratio(1, -1); //��y�ᷭתΪ ����Ϊ��

	setbkcolor(cs->color);
	cleardevice();

	setlinecolor(cs->axis.color); //������ɫ
	setlinestyle(PS_SOLID, 3);     //�ߵ���ʽ��ʵ�ߣ����
	line(-cs->length / 2, 0, cs->length / 2, 0);
	line(transX(cs, cs->zerox), cs->width / 2, transX(cs, cs->zerox), -cs->width / 2);
	//���Ƽ�ͷ
	double angle = 15 / 180.0 * 3.14159; //��ͷ�Ƕ�
	double len3 = 20;
	double x11 = cs->zerox * 1.0;
	double x12 = cs->zerox * 1.0 + len3 * tan(angle) / cs->axis.ax;
	double x13 = cs->zerox * 1.0 - len3 * tan(angle) / cs->axis.ax;
	double y11 = cs->axis.yhigh * 1.0;
	double y12 = cs->axis.yhigh * 1.0 - len3 / cs->axis.ay;
	double y13 = cs->axis.yhigh * 1.0 - len3 / cs->axis.ay;
	line(transX(cs, x11), cs->width / 2, transX(cs, x12), cs->width / 2 - len3);
	line(transX(cs, x11), cs->width / 2, transX(cs, x13), cs->width / 2 - len3);

	double x21 = cs->axis.xhigh;
	double x22 = cs->axis.xhigh - len3 / cs->axis.ax;
	double x23 = cs->axis.xhigh - len3 / cs->axis.ax;
	double y21 = cs->zeroy;
	double y22 = cs->zeroy + len3 * tan(angle) / cs->axis.ay;
	double y23 = cs->zeroy - len3 * tan(angle) / cs->axis.ay;
	line(transX(cs, x21), transY(cs, y21), transX(cs, x22), transY(cs, y22));
	line(transX(cs, x21), transY(cs, y21), transX(cs, x23), transY(cs, y23));
	
	if (cs->scale.openflag) {
		int len4 = 5;  //�̶ȵĸ߶�
		double rlen = 50 / cs->axis.ax, rwid = 30 / cs->axis.ay; //�̶�ֵ����
		int bias = 2; //�̶�ֵ��������ľ���
		wchar_t s[1024];
		int i = 0;
		setaspectratio(1, 1); //��y�ᷭתΪ ����Ϊ��
		settextcolor(cs->scale.color); //����������ɫ
		setbkmode(TRANSPARENT); //����͸��
		while (cs->zerox - i * cs->scale.scalex > cs->axis.xlow) {
			setlinecolor(cs->axis.color); //���ÿ̶���ɫ
			setlinestyle(PS_SOLID, 3);     //�ߵ���ʽ��ʵ�ߣ����

			line(transX(cs, cs->zerox - i * cs->scale.scalex), transY(cs, cs->zeroy), 
				transX(cs, cs->zerox - i * cs->scale.scalex), transY(cs, cs->zeroy) - len4);
			RECT r = { transX(cs, cs->zerox - i * cs->scale.scalex - rlen / 2), transY(cs, cs->zeroy) + bias,
				transX(cs, cs->zerox - i * cs->scale.scalex + rlen / 2),transY(cs, cs->zeroy + rwid) + bias };
			if (cs->scale.scalex >= 1) {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%d", cs->zerox + i * (int)cs->scale.scalex);
			}
			else {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%lf", cs->zerox + i * cs->scale.scalex);
			}
			//drawtext(s, &r, DT_CENTER | DT_TOP | DT_SINGLELINE);
			//HDC hdc = GetDC(cs->hwnd); // ��ȡ�豸�����ģ�cs->hwnd�Ǵ��ھ��
			//DrawTextW(hdc, s, -1, &r, DT_CENTER | DT_TOP | DT_SINGLELINE); // ʹ�� DrawTextW ����
			//ReleaseDC(cs->hwnd, hdc); // �ͷ��豸������


			//��������
			if (cs->grid.openflag && i != 0) {
				setlinecolor(cs->grid.color); //����������ɫ
				setlinestyle(PS_DASH, 1);     //�ߵ���ʽ�����ߣ����
				line(transX(cs, cs->zerox - i * cs->scale.scalex), transY(cs, cs->axis.ylow), 
					transX(cs, cs->zerox - i * cs->scale.scalex), transY(cs, cs->axis.yhigh));
			}
			i++;
		}
		i = 1;
		while (cs->zerox + i * cs->scale.scalex < cs->axis.xhigh) {
			setlinecolor(cs->axis.color); //���ÿ̶���ɫ
			setlinestyle(PS_SOLID, 3);     //�ߵ���ʽ��ʵ�ߣ����

			line(transX(cs, cs->zerox + i * cs->scale.scalex), transY(cs, cs->zeroy), 
				transX(cs, cs->zerox + i * cs->scale.scalex), transY(cs, cs->zeroy) - len4);
			RECT r = { transX(cs, cs->zerox + i * cs->scale.scalex - rlen / 2), transY(cs, cs->zeroy) + bias,
				transX(cs, cs->zerox + i * cs->scale.scalex + rlen / 2),transY(cs, cs->zeroy + rwid) + bias };
			if (cs->scale.scalex >= 1) {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%d", cs->zerox + i * (int)cs->scale.scalex);
			}
			else {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%lf", cs->zerox + i * cs->scale.scalex);
			}
			//drawtext(s, &r, DT_CENTER | DT_TOP | DT_SINGLELINE);
			//HDC hdc = GetDC(cs->hwnd); // ��ȡ�豸�����ģ�cs->hwnd�Ǵ��ھ��
			//DrawTextW(hdc, s, -1, &r, DT_CENTER | DT_TOP | DT_SINGLELINE); // ʹ�� DrawTextW ����
			//ReleaseDC(cs->hwnd, hdc); // �ͷ��豸������

			//��������
			if (cs->grid.openflag && i != 0) {
				setlinecolor(cs->grid.color); //����������ɫ
				setlinestyle(PS_DASH, 1);     //�ߵ���ʽ�����ߣ����
				line(transX(cs, cs->zerox + i * cs->scale.scalex), transY(cs, cs->axis.ylow), 
					transX(cs, cs->zerox + i * cs->scale.scalex), transY(cs, cs->axis.yhigh));
			}
			i++;
		}
		i = 0;
		while (cs->zeroy - i * cs->scale.scaley > cs->axis.ylow) {
			setlinecolor(cs->axis.color); //���ÿ̶���ɫ
			setlinestyle(PS_SOLID, 3);     //�ߵ���ʽ��ʵ�ߣ����

			line(transX(cs, cs->zerox), transY(cs, cs->zeroy + i * cs->scale.scaley), 
				transX(cs, cs->zerox) + len4, transY(cs, cs->zeroy + i * cs->scale.scaley));
			RECT r = { transX(cs, cs->zerox - rlen) - bias, transY(cs,cs->zeroy + i * cs->scale.scaley - rwid / 2),
				transX(cs, cs->zerox) - bias, transY(cs,cs->zeroy + i * cs->scale.scaley + rwid / 2) };
			if (cs->scale.scaley >= 1) {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%d", cs->zerox + i * (int)cs->scale.scaley);
			}
			else {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%lf", cs->zerox + i * cs->scale.scaley);
			}
			//drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			//HDC hdc = GetDC(cs->hwnd); // ��ȡ�豸�����ģ�cs->hwnd�Ǵ��ھ��
			//DrawTextW(hdc, s, -1, &r, DT_CENTER | DT_TOP | DT_SINGLELINE); // ʹ�� DrawTextW ����
			//ReleaseDC(cs->hwnd, hdc); // �ͷ��豸������
			//��������
			if (cs->grid.openflag && i != 0) {
				setlinecolor(cs->grid.color); //����������ɫ
				setlinestyle(PS_DASH, 1);     //�ߵ���ʽ�����ߣ����
				line(transX(cs, cs->axis.xlow), transY(cs, cs->zeroy + i * cs->scale.scaley), transX(cs, cs->axis.xhigh), transY(cs, cs->zeroy + i * cs->scale.scaley));
			}
			i++;
		}
		i = 1;
		while (cs->zeroy + i * cs->scale.scaley < cs->axis.yhigh) {
			setlinecolor(cs->axis.color); //���ÿ̶���ɫ
			setlinestyle(PS_SOLID, 3);     //�ߵ���ʽ��ʵ�ߣ����

			line(transX(cs, cs->zerox), transY(cs, cs->zeroy - i * cs->scale.scaley), 
				transX(cs, cs->zerox) + len4, transY(cs, cs->zeroy - i * cs->scale.scaley));
			RECT r = { transX(cs, cs->zerox - rlen) - bias, transY(cs,cs->zeroy - i * cs->scale.scaley - rwid / 2),
				transX(cs, cs->zerox) - bias, transY(cs,cs->zeroy - i * cs->scale.scaley + rwid / 2) };
			//sprintf(s, "%d", cs->zeroy + i * cs->scale.scaley);
			if (cs->scale.scaley >= 1) {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%d", cs->zeroy + i * (int)cs->scale.scaley);
			}
			else {
				swprintf(s, sizeof(s) / sizeof(wchar_t), L"%lf", cs->zeroy + i * cs->scale.scaley);
			}
			//			printf("%s\n", s);
			//drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			// HDC hdc = GetDC(cs->hwnd); // ��ȡ�豸�����ģ�cs->hwnd�Ǵ��ھ��
			//DrawTextW(hdc, s, -1, &r, DT_CENTER | DT_TOP | DT_SINGLELINE); // ʹ�� DrawTextW ����
			//ReleaseDC(cs->hwnd, hdc); // �ͷ��豸������
			//��������
			if (cs->grid.openflag && i != 0) {
				setlinecolor(cs->grid.color); //����������ɫ
				setlinestyle(PS_DASH, 1);     //�ߵ���ʽ�����ߣ����
				line(transX(cs, cs->axis.xlow), transY(cs, cs->zeroy - i * cs->scale.scaley), 
					transX(cs, cs->axis.xhigh), transY(cs, cs->zeroy - i * cs->scale.scaley));
			}
			i++;
		}
	}
}
