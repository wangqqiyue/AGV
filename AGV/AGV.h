#pragma once
#include <math.h>
#include <stdio.h>
#include <graphics.h>
#include <vector>

using namespace std;


//界面参数
#define WIDTH  800
#define HEIGHT 600
//帧间隔，单位ms
#define INTERVAL 100
//小车半径，单位px
#define RADIUS   10
//最多支持的小车数量
#define AGENTS_NUM  5
//虚线宽度
#define DASH_WIDTH  3
//字符长度
#define TEXT_LEN   15
//最快速度
#define SPEED_MAX  10


int distance(POINT a, POINT b) {
	int x = b.x - a.x;
	int y = b.y - a.y;
	return sqrt(x*x+y*y);
}

//角度，用sin/cos来表示
class Angle {
public:
	double sin;
	double cos;
	//传入坐标点a,b，就可以获得从a到b的角度
	void calc(POINT a, POINT b);
};

//水平方向邻边adjacent,垂直方向对边opposite,斜边hypotenuse
void Angle::calc(POINT a, POINT b) {
	int adj = b.x - a.x;
	int oppo = b.y - a.y;
	double hypo = sqrt(adj * adj + oppo * oppo);
	sin = oppo / hypo;
	cos = adj / hypo; 
};

class Agent {
public:
	POINT last;//上一次位置
	POINT curr;//当前位置
	vector<POINT> path;
	POINT src;//起点
	POINT dst;//终点
	int speed;
	Angle ang;//与正方向夹角
	COLORREF color;//颜色
	int id;//agentID


	Agent(POINT curr, POINT dst, COLORREF color, int id);
	void drawInitPath();
	void drawAgent();
	void drawPath();
	void run();
	void avoidConflict(Agent **agents, int agentsNum);
	Agent* getNearest(Agent** agents, int agentsNum);
	int getPriority();
};
Agent::Agent(POINT src, POINT dst, COLORREF  color, int id) {
	this->curr = src; 
	this->src = src;
	this->dst = dst;
	this->last = src; 
	this->color = color; 
	speed = SPEED_MAX; 
	ang.calc(src,dst);
	this->id = id; 
	path.push_back(src);
	drawInitPath(); 
};

//初始化路径
void Agent::drawInitPath() {
	wchar_t idText[TEXT_LEN];
	wsprintf(idText, L"%d", id);
	setlinestyle(PS_DASH | PS_ENDCAP_FLAT, DASH_WIDTH);
	setlinecolor(color);
	line(src.x,src.y,dst.x,dst.y);
	settextcolor(color);
	outtextxy(src.x, src.y+5, idText);
	drawAgent();
}

//绘制小车
void Agent::drawAgent() {
	setlinestyle(PS_SOLID);
	setlinecolor(color);
	setfillcolor(color);
	fillcircle(curr.x,curr.y,RADIUS);
}

//绘制实时路径
void Agent::drawPath() {
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, DASH_WIDTH);
	setlinecolor(color);
	for (auto i = path.begin(); i != path.end()-1;i++) {
		vector<POINT>::iterator  next = std::next(i,1);
		line(i->x, i->y, next->x, next->y);
	}
	
}

//小车行驶
void Agent::run() {
	if (distance(curr, dst) >= RADIUS) {
		last.x = curr.x;
		last.y = curr.y;
		curr.x += speed * ang.cos;
		curr.y += speed * ang.sin;

		path.push_back(curr);
	}
}

//获取离自己最近的小车
Agent* Agent::getNearest(Agent** agents, int agentsNum) {
	int minDist = WIDTH+HEIGHT;
	int dist = 0;
	int nearest = 0;
	for (int i = 0; i < agentsNum; i++) {
		if (i == id) {
			continue;
		}
		else {
			dist=distance(agents[i]->curr, curr);
			if (dist < minDist) {
				minDist = dist;
				nearest = i;
			}
		}
	}

	return agents[nearest];
}
//优先级,简单计算到终点的剩余
int Agent::getPriority() {
	return distance(src, dst);
}
void Agent::avoidConflict(Agent** agents, int agentsNum) {
	Agent* nearest = getNearest(agents, agentsNum);
	POINT nextPosition;
	POINT nextPositionNearest;
	int dist = 0;
	int distNext = 0;
	nextPosition.x = curr.x + speed * ang.cos;
	nextPosition.y = curr.y + speed * ang.sin;
	nextPositionNearest.x = nearest->curr.x + nearest->speed * nearest->ang.cos;
	nextPositionNearest.y = nearest->curr.y + nearest->speed * nearest->ang.sin;
	dist = distance(nearest->curr, curr);
	distNext = distance(nextPosition, nextPositionNearest);

	if (dist < RADIUS * 4 || distNext < RADIUS * 2) {
		if (getPriority() <= nearest->getPriority()) {
			speed -=1;
			//修正角度,向远离最近邻居的方向行驶
			ang.calc(nearest->curr, curr);
		}
		else {
			speed += 1;
			if (speed > SPEED_MAX) {
				speed = SPEED_MAX;
			}
		}
		
	}
	else {
		speed += 1;
		if (speed > SPEED_MAX) {
			speed = SPEED_MAX;
		}
		//修正角度
		ang.calc(curr, dst);
	}

	

}