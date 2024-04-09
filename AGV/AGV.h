#pragma once
//�������
#define WIDTH  800
#define HEIGHT 600
//֡�������λms
#define INTERVAL 100
//С���뾶����λpx
#define RADIUS   5
//���֧�ֵ�С������
#define AGENTS_NUM  5
//���߿��
#define DASH_WIDTH  3
//�ַ�����
#define TEXT_LEN   15
class Position {
public:
	int x;
	int y;
};

class Agent {
public:
	Position curr;//��ǰλ��
	Position dst;//�յ�
	int speed;
	double degree;//��������н�,0-2��
	COLORREF color;//��ɫ
	int id;//agentID
	Agent(Position curr, Position dst, COLORREF  color, int id) { this->curr = curr; this->dst = dst; this->color = color; speed = 0; degree = 0;  this->id = id; initPath(); };
	void initPath();
	void drawAgent();
};

//��ʼ��·��
void Agent::initPath() {
	wchar_t idText[TEXT_LEN];
	wsprintf(idText, L"%d", id);
	setlinestyle(PS_DASH | PS_ENDCAP_FLAT, DASH_WIDTH);
	setlinecolor(color);
	line(curr.x,curr.y,dst.x,dst.y);
	settextcolor(color);
	outtextxy(curr.x, curr.y+5, idText);
}

//����С��
void Agent::drawAgent() {
	setlinestyle(PS_SOLID);
	setlinecolor(color);
	setfillcolor(color);
	fillcircle(curr.x,curr.y,RADIUS);
}
