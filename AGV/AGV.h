#pragma once
#include <math.h>
#include <stdio.h>
#include <graphics.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


//�������
#define WIDTH  800
#define HEIGHT 600
//֡�������λms
#define INTERVAL 100
//С���뾶����λpx
#define RADIUS   10
//���֧�ֵ�С������
#define AGENTS_NUM  5
//���߿��
#define DASH_WIDTH  3
//�ַ�����
#define TEXT_LEN   15
//����ٶ�
#define SPEED_MAX  10


int distance(POINT a, POINT b) {
	int x = b.x - a.x;
	int y = b.y - a.y;
	return sqrt(x*x+y*y);
}

//�Ƕȣ���sin/cos����ʾ
class Angle {
public:
	double sin;
	double cos;
	//���������a,b���Ϳ��Ի�ô�a��b�ĽǶ�
	void calc(POINT a, POINT b);
};

//ˮƽ�����ڱ�adjacent,��ֱ����Ա�opposite,б��hypotenuse
void Angle::calc(POINT a, POINT b) {
	int adj = b.x - a.x;
	int oppo = b.y - a.y;
	double hypo = sqrt(adj * adj + oppo * oppo);
	sin = oppo / hypo;
	cos = adj / hypo; 
};

class Agent {
public:
	POINT last;//��һ��λ��
	POINT curr;//��ǰλ��
	vector<POINT> path;
	POINT src;//���
	POINT dst;//�յ�
	int speed;
	Angle ang;//��������н�
	COLORREF color;//��ɫ
	int id;//agentID
	fstream logFile;//��־�ļ�

	Agent(POINT curr, POINT dst, COLORREF color, int id);
	~Agent();
	//��ʼ������·��
	void drawInitPath();
	void drawAgent();
	void drawPath();
	void run();
	//����ϰ�
	void avoidConflict(Agent **agents, int agentsNum);
	Agent* getNearest(Agent** agents, int agentsNum);
	int getPriority();
	//�ж��Ƿ����
	bool isFinish();
	//��¼��־
	void recordLog(int period);
	//������־�ļ�
	void createLog();
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
	createLog();
};
//��������
Agent::~Agent() {
	//�ر���־�ļ�
	logFile.close();
}

//��ʼ��·��
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

//����С��
void Agent::drawAgent() {
	setlinestyle(PS_SOLID);
	setlinecolor(color);
	setfillcolor(color);
	fillcircle(curr.x,curr.y,RADIUS);
}

//����ʵʱ·��
void Agent::drawPath() {
	setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, DASH_WIDTH);
	setlinecolor(color);
	for (auto i = path.begin(); i != path.end()-1;i++) {
		vector<POINT>::iterator  next = std::next(i,1);
		line(i->x, i->y, next->x, next->y);
	}
	
}

//С����ʻ
void Agent::run() {
	if (distance(curr, dst) >= RADIUS) {
		last.x = curr.x;
		last.y = curr.y;
		curr.x += speed * ang.cos;
		curr.y += speed * ang.sin;

		path.push_back(curr);
	}
}

//��ȡ���Լ������С��
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
//���ȼ�,�򵥼������㵽��ǰλ�õľ���
int Agent::getPriority() {
	return distance(src, curr);
}
//����ϰ�
//�㷨���ҵ����Լ������Agent��������߾���С��һ��ֵ�����򷴷����˶�
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

	if (dist < RADIUS * 5 || distNext < RADIUS * 3) {
		if (getPriority() <= nearest->getPriority()) {
			speed -= 1;
			if (speed < 0) {
				speed = 0;
			}
			//�����Ƕ�,��Զ������ھӵķ�����ʻ
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
		//�����Ƕ�
		ang.calc(curr, dst);
	}
}

//��¼��־
void Agent::recordLog(int period) {
	logFile << "\t" << period << "\t\t" << curr.x << "\t\t" << curr.y << "\t\t" << speed << endl;
}

//������־�ļ�
void Agent::createLog() {
	char logFileName[TEXT_LEN]={0};
	sprintf_s(logFileName,"Agent_%d.txt",id);
	logFile.open(logFileName,ios::out);

    if (!logFile) {
        cerr << "�޷������ļ�" << logFileName << endl;
        return ;
    }

	logFile << "\t����" << "\t����x" << "\t����y" << "\t�ٶ�" << endl;
}

bool Agent::isFinish() {
	return distance(curr, dst) <= RADIUS;
}