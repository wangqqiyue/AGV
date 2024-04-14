#include "AGV.h"
//agent对象数组
Agent *agents[AGENTS_NUM];
//agent总数量
int agentsNum=0;
//记录每个Agent是否结束的标志位数组
int finishArr[AGENTS_NUM] = { 0 };
//时间周期
int period = 0;

void update() {
	period++;
	cleardevice();
	BeginBatchDraw();
	for (int i = 0; i < agentsNum; i++) {
		agents[i]->drawInitPath();
		agents[i]->drawAgent();
		agents[i]->drawPath();
		if (finishArr[i]==0) {
			if (agents[i]->isFinish()) {
				finishArr[i] = 1;
			}
		}
		if (finishArr[i]==0) {
			agents[i]->avoidConflict(agents, agentsNum);
			agents[i]->run();
			agents[i]->recordLog(period);
		}
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
//结束任务并清除agent
void finishJobs() {
	for (int i = 0; i < agentsNum; i++) {
		free(agents[i]);
		agents[i] = NULL;
	}
}
//判断是否所有agent都结束了
bool isAllFinish() {
	for (int i = 0; i < agentsNum; i++) {
		if (finishArr[i] == 0) {
			return false;
		}
	}
	return true;
}

int main() {
	//初始化渲染界面
	initgraph(WIDTH, HEIGHT);
	setbkcolor(WHITE);
	// 用背景色清空屏幕
	cleardevice();

	initJobs();
	while (!isAllFinish()) {
		update();
		Sleep(INTERVAL);
	}
	finishJobs();

	getchar();
	closegraph();
	return 0;
}