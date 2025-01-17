#pragma once


class GameUIRener
{
public:
	static GameUIRener& Instance()
	{
		static GameUIRener ins;
		return ins;
	}



private:
	int MaxCost = 0;
	int NowCost = 0;
	int UseCost = 0;


};

