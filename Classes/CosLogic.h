﻿/*

	This header contains game's data and logic.

*/
#pragma once

#ifndef __COSLOGIC_H__
#define __COSLOGIC_H__

#include "cocos2d.h"
#include <vector>
#include <queue>

#define LAYER_COUNT 3 //一共多少个关卡

namespace cosmos{

enum Game_Level{
    Capricorn = 0, //魔羯座 (12/22 - 01/19)
    Aquarius, //水瓶座  (01/ 20- 02/18)
    Pisces, //双鱼座 (02/19- 03/20)
    Aries, //白羊座 (03/21 - 04/20)
    Taurus, //金牛座  (04/21 - 05/20)
    Gemini, //双子座 (05/21 - 6/21)
    Cancer, //巨蟹座 (06/22- 07/22
    Leo, //狮子座 (07/23 - 08/22)
    Virgo, //处女座 (08/23 - 09/22)
    Libra, //天秤座 (09/23 - 10/23)
    Scorpio, //天蝎座 (10/24 - 11/21)
    Sagittarius//射手座 (11/22 - 12/21)
};

enum Game_State{
	NOTSTART = 0,
	RUN,
	WIN,
	LOSE
};

enum Game_Difficulty{
	EASY = 0,
	NORMAL,
	HARD
};

typedef struct Game_Config{
		bool isSFXOn;
		bool isMusicOn;
}COSCONFIG;

typedef struct{
	int maxScoreNormal;
	double minTimeNormal;
	int maxScoreHard;
	double minTimeHard;
}GALLERYTYPE;

static const int LINK_NULL = 100;
//static const int LINK_TYPE_COUNT = 10;

class CosGame
{
public:
	typedef unsigned int link_t;
	typedef std::vector<std::vector<link_t> > board_t;

	virtual ~CosGame();

	static CosGame *getInstance();
	static void releaseInstance(){
		if(pGame)
			delete pGame;
		pGame = nullptr;
	}

	void createBoard(int x, int y, int ltc, int lmc);//创建一个长度为x,宽度为y的连连看面板,ltc为连连看元素的种类,lmc为每个元素最多有几个
	void startGame(){reset();CurrentState = RUN;}//开始游戏
	void endGame(){CurrentState = NOTSTART;}//结束游戏
	void winGame(){CurrentState = WIN;}//游戏胜利
	void loseGame(){CurrentState = LOSE;}//游戏失败
	void chooseLevel(Game_Level level){CurrentLevel = level;}//选择关卡
	void chooseDifficulty(Game_Difficulty diff){CurrentDiff = diff;};//选择难度
	void addGameScore(int nAddScore){GameScore += nAddScore;}
	void incGameTime(double inc){GameTime += inc;};
	
	bool isInLinkArea(int x, int y);//检测这个点是否在连连看图片区域内
	bool isContinuable();//检测游戏是否能继续
	bool isLinkable(int x1, int y1, int x2, int y2);//检测两个连连看元素是否能够相消
	bool isAllClear();//检测是否所有的连连看方块都被消去

	double getGameTime(){return GameTime;}
	int getGameScore(){return GameScore;}
	Game_State getGameState(){return CurrentState;}
	Game_Level getGameLevel(){return CurrentLevel;}
	Game_Difficulty getDiffculty(){return CurrentDiff;}
	link_t getBoardPosValue(int i, int j){return Board[i][j];}//获得面板对应点的坐标
	void setLinkNull(int x, int y){Board[y][x] = LINK_NULL;}

	const COSCONFIG &getConfig(){return config;}
	void saveConfig(const COSCONFIG &config);
	void loadConfig();

	void loadGallery();
	bool updateGallery();
	const GALLERYTYPE &getGallery(){return gallery;}
private:
	typedef struct Search_Point{
		int x,y;
		int cross;
	}SP;

	

	CosGame();
	void randArr();
	void reset();
	void expand_search_point(std::vector<SP> &searched, std::queue<SP> &opcl, std::vector<SP> &link_set,const SP &point_to_search);
	bool check_succeed(std::vector<SP> &link_set, const SP &target, const SP &origin);
	bool not_searched(std::vector<SP> &searched, const SP &point_to_add);

	static CosGame *pGame;

	Game_Level CurrentLevel;
	Game_State CurrentState;
	Game_Difficulty CurrentDiff;
	int GameScore;//游戏分数
	double GameTime;//游戏时间
	int BoardHor;//板上横向连连看个数（带隐藏位）
	int BoardVer;//板上纵向连连看个数（带隐藏位）
	int HorNum;	//横向连连看个数
	int VerNum;	//纵向连连看个数
	int link_type_count;	//连连看元素的种类
	int link_max_couple;	//每个元素最多有几个
	board_t Board;//连连看主面板

	COSCONFIG config; //设置文件
	GALLERYTYPE gallery;
};

}

#endif /* __COSLOGIC_H__ */
