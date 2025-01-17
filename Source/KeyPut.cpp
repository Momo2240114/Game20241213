#include "KeyPut.h"
#include "System/ShapeRenderer.h"
#include<unordered_map>


bool KeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000) != 0;
}

bool KeyPressedOut(int key)
{
	static std::unordered_map<int, bool> oldState;

	bool current = (GetAsyncKeyState(key) & 0x8000) != 0;
	bool RET = (!oldState[key] && current);

	oldState[key] = current;
	return RET;
}

bool KeyPressedIn(int key)
{
	static std::unordered_map<int, bool> oldState;

	bool current = (GetAsyncKeyState(key) & 0x8000) != 0;
	bool RET = (oldState[key] && current);

	oldState[key] = current;
	return RET;
}