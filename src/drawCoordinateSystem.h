#pragma once
#ifndef DrawCoordinateSystem 
#define DrawCoordinateSystem
#include <tchar.h>
#include <codecvt>
#include <string>
#include <locale>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <sstream>

#define _UNICODE
#define UNICODE

//轴结构体
struct Axis {
	int xlow, xhigh;
	int ylow, yhigh;
	int centerx, centery;
	double ax, ay;   //压缩
	int color;
};

//刻度结构体
struct Scale {
	int openflag;  //是否开启
	double scalex;     //刻度大小
	double scaley;
	int color;
};

struct Grid {
	int openflag;
	int color;
};

typedef struct coordinatesystem {
	struct Axis axis;
	struct Scale scale;
	int zerox, zeroy;  //坐标系原点
	struct Grid grid;
	int length, width;  //图幅长宽
	int color;     //背景颜色
	int borderx, brodery; //边界空出的范围

	HWND hwnd;
}CoordinateSystem;


void coordinateSystemInit(CoordinateSystem* cs);

int transX(CoordinateSystem* cs, double x);

int transY(CoordinateSystem* cs, double y);

void createCS(CoordinateSystem* cs);

template<typename T>
void setRange(CoordinateSystem* cs, T xlow, T xhigh, T ylow, T yhigh) {
	cs->axis.ylow = ylow;
	cs->axis.yhigh = yhigh;
	cs->axis.xlow = xlow;
	cs->axis.xhigh = xhigh;
	cs->axis.centerx = (cs->axis.xlow + cs->axis.xhigh) / 2;
	cs->axis.centery = (cs->axis.ylow + cs->axis.yhigh) / 2;
	int len1 = max(cs->axis.xhigh - cs->axis.xlow, 2);
	int len2 = max(cs->axis.yhigh - cs->axis.ylow, 2);
	cs->axis.ax = cs->length * 1.0 / len1;
	cs->axis.ay = cs->width * 1.0 / len2;
	cs->scale.scalex = (int)(100 / cs->axis.ax) / 10 * 10;
	cs->scale.scaley = (int)(100 / cs->axis.ay) / 10 * 10;
	if (cs->scale.scalex <= 0)
		cs->scale.scalex = (int)(100 / cs->axis.ax);
	if (cs->scale.scaley <= 0)
		cs->scale.scaley = (int)(100 / cs->axis.ay);
	if (cs->scale.scalex <= 0)
		cs->scale.scalex = 1;
	if (cs->scale.scaley <= 0)
		cs->scale.scaley = 1;
	cs->zerox = 0;
	cs->zeroy = cs->axis.centery;
}

template<typename T>
void plotType(CoordinateSystem* cs, T* data, int len, int color) {
	T tmpMin = data[0], tmpMax = data[0];
	for (int i = 1; i < len; ++i) {
		if (data[i] > tmpMax)
			tmpMax = data[i];
		if (data[i] < tmpMin)
			tmpMin = data[i];
	}
	T range = tmpMax - tmpMin;
	coordinateSystemInit(cs);

	setRange(cs, 0 - len * 0.1, len * 1.1, tmpMin - range * 0.1, tmpMax + range * 0.1);

	createCS(cs);
	setlinecolor(color);
	setaspectratio(1, -1);
	setlinestyle(PS_SOLID, 2);
	for (int i = 0; i < len - 1; ++i) {
		line(transX(cs, i + 1), transY(cs, data[i]), transX(cs, i + 2), transY(cs, data[i + 1]));
	}
	for (int i = 0; i < len; ++i) {
		putpixel(transX(cs, i + 1), transY(cs, data[i]), color);
	}
	return;
}

#endif // !DrawCoordinateSystem 

