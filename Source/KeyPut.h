#pragma once

//キーが押されているかの検知
extern bool KeyPressed(int key);
//キー離された時
extern bool KeyPressedOut(int key);
//キー押された時
extern bool KeyPressedIn(int key);


enum KeyCode {
	A = 0x41,//68
	B = 0x42,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J
};