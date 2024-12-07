#pragma once
#include "pch.h"

// 키 코드 열거형
enum class KEY
{
	NONE = 0,

	/// Mouse
	LBUTTON = 0x01,	// 왼쪽 마우스 버튼
	RBUTTON = 0x02, // 오른쪽 마우스 버튼
	MBUTTON = 0x04, // 마우스 가운데 버튼
	XBUTTON_ONE = 0x05, // X1 마우스 버튼
	XBUTTON_TWO = 0x06, // X2 마우스 버튼

	/// KeyBoard
	BACK = 0x08,	// 백스페이스 키
	TAB = 0x09,	// Tab 키
	RETURN = 0x0D,	// Enter 키

	SHIFT = 0x10, // Shift 키
	CTRL = 0x11, // Ctrl 키
	ALT = 0x12, // Alt 키

	CAPSLOCK = 0x14, // Caps Lock 키

	ESCAPE = 0x1B, // Esc 키

	SPACE = 0x20, // 스페이스바
	PGUP = 0x21, // Page Up 키
	PGDN = 0x22, // Page Down 키
	END = 0x23, // End 키
	HOME = 0x24, // Home 키
	LEFT = 0x25, // 왼쪽 화살표 키
	UP = 0x26, // 위쪽 화살표 키
	RIGHT = 0x27, // 오른쪽 화살표 키
	DOWN = 0x28, // 아래쪽 화살표 키

	INS = 0x2D, // Ins 키
	DEL = 0x2E, // DEL 키

	D0 = 0x30, // 0 키
	D1 = 0x31, // 1 키
	D2 = 0x32, // 2 키
	D3 = 0x33, // 3 키
	D4 = 0x34, // 4 키
	D5 = 0x35, // 5 키
	D6 = 0x36, // 6 키
	D7 = 0x37, // 7 키
	D8 = 0x38, // 8 키
	D9 = 0x39, // 9 키

	A = 0x41, // A 키
	B = 0x42, // B 키
	C = 0x43, // C 키
	D = 0x44, // D 키
	E = 0x45, // E 키
	F = 0x46, // F 키
	G = 0x47, // G 키
	H = 0x48, // H 키
	I = 0x49, // I 키
	J = 0x4A, // J 키
	K = 0x4B, // K 키
	L = 0x4C, // L 키
	M = 0x4D, // M 키
	N = 0x4E, // N 키
	O = 0x4F, // O 키
	P = 0x50, // P 키
	Q = 0x51, // Q 키
	R = 0x52, // R 키
	S = 0x53, // S 키
	T = 0x54, // T 키
	U = 0x55, // U 키
	V = 0x56, // V 키
	W = 0x57, // W 키
	X = 0x58, // X 키
	Y = 0x59, // Y 키
	Z = 0x5A, // Z 키

	WIN = 0x5B, // 왼쪽 Windows 키

	F1 = 0x70, // F1 키
	F2 = 0x71, // F2 키
	F3 = 0x72, // F3 키
	F4 = 0x73, // F4 키
	F5 = 0x74, // F5 키
	F6 = 0x75, // F6 키
	F7 = 0x76, // F7 키
	F8 = 0x77, // F8 키
	F9 = 0x78, // F9 키
	F10 = 0x79, // F10 키
	F11 = 0x7A, // F11 키
	F12 = 0x7B, // F12 키

	NUMLOCK = 0x90, // Num Lock 키
	SCROLLLOCK = 0x91, // Scroll Lock 키

	LSHIFT = 0xA0, // 왼쪽 Shift 키
	RSHIFT = 0xA1, // 오른쪽 Shift 키
	LCONTROL = 0xA2, // 왼쪽 Ctrl 키
	RCONTROL = 0xA3, // 오른쪽 Ctrl 키
	LMENU = 0xA4, // 왼쪽 Alt 키
	RMENU = 0xA5, // 오른쪽 Alt 키

	OEM_1 = 0xBA,		// ;: 키
	OEM_PLUS = 0xBB,	// + 키
	OEM_COMMA = 0xBC,	// , 키
	OEM_MINUS = 0xBD,	// - 키
	OEM_PERIOD = 0xBE,	// . 키
	OEM_2 = 0xBF,		// /?  키
	OEM_3 = 0xC0,		// `~  키
	OEM_4 = 0xDB,		// [{  키
	OEM_5 = 0xDC,		// \|  키
	OEM_6 = 0xDD,		// ]}  키
	OEM_7 = 0xDE,		// '"  키

	LAST = 0xFF
};

// 키 상태 열거형
enum class KEY_STATE
{
	NONE,
	AWAY,
	TAP,
	HOLD
};

// 키 정보 구조체
struct KeyInfo
{
	KeyInfo()
		: keyState(KEY_STATE::NONE), isPushed(false)
	{}
	KeyInfo(KEY_STATE keyState, bool isPushed)
		: keyState(keyState), isPushed(isPushed)
	{}
	~KeyInfo() = default;

	KEY_STATE keyState;	// 키 상태
	bool isPushed;		// 이전 프레임에 키가 눌렸었는지
};

