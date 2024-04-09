#pragma once
//界面参数
#define WIDTH  800
#define HEIGHT 600
//帧间隔，单位ms
#define INTERVAL 100
//小车半径，单位px
#define RADIUS   5
//最多支持的小车数量
#define AGENTS_NUM  5
//虚线宽度
#define DASH_WIDTH  3
//字符长度
#define TEXT_LEN   15
class Position {
public:
	int x;
	int y;
};

class Agent {
public:
	Position curr;//当前位置
	Position dst;//终点
	int speed;
	double degree;//与正方向夹角,0-2Π
	COLORREF color;//颜色
	int id;//agentID
	Agent(Position curr, Position dst, COLORREF  color, int id) { this->curr = curr; this->dst = dst; this->color = color; speed = 0; degree = 0;  this->id = id; initPath(); };
	void initPath();
	void drawAgent();
};

//初始化路径
void Agent::initPath() {
	wchar_t idText[TEXT_LEN];
	wsprintf(idText, L"%d", id);
	setlinestyle(PS_DASH | PS_ENDCAP_FLAT, DASH_WIDTH);
	setlinecolor(color);
	line(curr.x,curr.y,dst.x,dst.y);
	settextcolor(color);
	outtextxy(curr.x, curr.y+5, idText);
}

//绘制小车
void Agent::drawAgent() {
	setlinestyle(PS_SOLID);
	setlinecolor(color);
	setfillcolor(color);
	fillcircle(curr.x,curr.y,RADIUS);
}
