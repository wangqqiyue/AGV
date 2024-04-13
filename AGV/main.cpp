#include "AGV.h"

Agent *agents[AGENTS_NUM];
int agentsNum=0;

void update() {
	cleardevice();
	BeginBatchDraw();
	for (int i = 0; i < agentsNum; i++) {
		agents[i]->drawInitPath();
		agents[i]->avoidConflict(agents,agentsNum);
		agents[i]->run();
		agents[i]->drawAgent();
		agents[i]->drawPath();
	}
	FlushBatchDraw();
}

void addJob(POINT src, POINT dst,COLORREF color) {
	agents[agentsNum] = new Agent(src,dst,color, agentsNum);
	agentsNum++;
}

void initJobs() {
	addJob({ 100,100 }, { 700,400 }, RED);
	addJob({ 100,200 }, { 700,200 }, BROWN);
	addJob({ 100,400 }, { 700,100 }, GREEN);
	addJob({ 100,500 }, { 700,300 }, BLACK);
	addJob({ 50,300 }, { 600,300 }, BLUE);
}


int main() {
	//��ʼ����Ⱦ����
	initgraph(WIDTH, HEIGHT);
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	cleardevice();

	initJobs();
	while (1) {
		update();
		Sleep(INTERVAL);
	}
	getchar();
	closegraph();
	return 0;
}