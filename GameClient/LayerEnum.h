#pragma once

enum class Layer
{
	Fade,	// 효과				 : 0
	EFFECT,	// 효과				 : 0
	POP_B,	// 팝업창 버튼, img	 : 1
	POP_P,	// 팝업창 패널		 : 2
	POP,	// 팝업창			 : 3
	SET_B,	// 세팅창 버튼, img	 : 4
	SET_P,	// 세팅창 패널		 : 5
	SET,	// 세팅창			 : 6
	COM_M,	// 일반 메세지창		 : 7
	COM_B,	// 일반창 버튼, img   : 8
	COM_P,	// 일반창 패널		 : 9
	COM,	// 일반창			 : 10
	BG_B,	// 백그라운드 버튼, img: 11
	BG_P,	// 백그라운드 패널	 : 12
	BG,		// 백그라운드		 : 13

	Last,
};