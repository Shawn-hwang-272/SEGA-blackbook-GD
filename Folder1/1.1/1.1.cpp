// 1.1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;



const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object {
	OBJ_WALL,
	OBJ_MAN,
	OBJ_BLOCK,
	OBJ_SPACE,
	OBJ_GOAL,
	OBJ_BLOCK_ON_GOAL,
    OBJ_MAN_ON_GOAL,

    OBJ_UNKNOWN,
};
//定义对象

void initialize(Object* state, int w, int h ,const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);



int main()
{
	//创建状态数组
	Object* state = new Object[gStageWidth * gStageHeight];
	//初始化场景
	initialize(state, gStageWidth, gStageHeight, gStageData);
	while (true) {
		draw(state, gStageWidth, gStageHeight);
		//绘制场景
		if (checkClear(state, gStageHeight, gStageWidth)) {
			break;
		}
		//通关判定
		cout << "a:left,d:right,w:up,s:down command? " <<endl;

		char input;
		cin >> input;
		update(state, input,gStageWidth, gStageHeight);
	}

	cout << "congratulation's! you win." << endl;
	//通关
	delete[] state;
	state = 0;

	while (true) {
		;
	}
		return  0;
}

void initialize(Object* state, int width, int/*height*/, const char* stageData) {
	const char* d = stageData;
	int x = 0;
	int y = 0;
	while (*d != '\0') {
		Object t;//临时变量
		switch ( *d ) {
		case'#':t = OBJ_WALL; break;
		case'p':t = OBJ_MAN; break;
		case'o':t = OBJ_BLOCK; break;
		case' ':t = OBJ_SPACE; break;
		case'.':t = OBJ_GOAL; break;
		case'O':t = OBJ_BLOCK_ON_GOAL; break;
		case'P':t = OBJ_MAN_ON_GOAL; break;
		case'\n':x = 0; ++y; t = OBJ_UNKNOWN; break;

		default:t = OBJ_UNKNOWN; break;

		}
		++d;
		//生成对象

		if (t != OBJ_UNKNOWN) {
			state[y * width + x] = t;
			++x;
		}

	}
}

void draw(const Object* state, int width, int height) {
	const char font[] = { '#' ,'p' ,'o' ,' ' ,'O' ,'.' , 'P'};
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Object o = state[y*width + x];
				cout << font[ o ];
		}
		cout << endl;
	}

}

void update(Object* s, char input, int w, int h) {
	//移动量变换(变量名中的d可以理解为difference或者delta)
	int dx = 0;
	int dy = 0;
	switch (input) {
	case 'a': dx = -1; break; //向左
	case 'd': dx = 1; break; //右
	case 'w': dy = -1; break; //上。Y朝下为正
	case 's': dy = 1; break; //下。
	}
	//查询小人的坐标
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w; //x是对宽度的余数
	int y = i / w; //y是对宽度的商

	//移动
	//移动后的坐标（t并没有特定的代表含义）
	int tx = x + dx;
	int ty = y + dy;
	//判断坐标的极端值。不允许超出合理值范围
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//A.该方向上是空白或者终点。小人则移动
	int p = y * w + x; //人员位置
	int tp = ty * w + tx; //目标位置（TargetPosition）
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //如果该位置是终点，则将该位置值变为“终点上站着人”
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //如果该位置已经是“终点上站着人”，则变为“终点”
		//B.如果该方向上是箱子。并且该方向的下下个格子是空白或者终点，则允许移动
	}
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		//检测同方向上的下下个格子是否位于合理值范围
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //按键无效
			return;
		}

		int tp2 = (ty + dy) * w + (tx + dx); //下下个格子
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			//按顺序替换
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}
//没有方块则判定为通关
bool checkClear(const Object* s, int width, int height) {
	for (int i = 0; i < width * height; ++i) {
		if (s[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}