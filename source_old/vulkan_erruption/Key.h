///////////////////////////////////////////////////////////////////////////////
// File:		 Key.h
// Revision:	 1.0
// Date:		 11.11.2019
// Author:		 Christian Steinbrecher
// Description:  Mapping of keys to ints
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <ostream>

enum class Key {
	Null = 0,

	_0 = 48,
	_1 = 49,
	_2 = 50,
	_3 = 51,
	_4 = 52,
	_5 = 53,
	_6 = 54,
	_7 = 55,
	_8 = 56,
	_9 = 57,

	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	Up = 128,
	Down = 129,
	Left = 130,
	Right = 131,

	Num0 = 200,
	Num1 = 201,
	Num2 = 202,
	Num3 = 203,
	Num4 = 204,
	Num5 = 205,
	Num6 = 206,
	Num7 = 207,
	Num8 = 208,
	Num9 = 209
};

std::string KeyToString(Key const key);

std::ostream& operator<<(std::ostream& ost, Key const key);




// #######+++++++ Implementation +++++++#######

#include <string>

inline std::string KeyToString(Key const key)
{
	switch (key) {
	case Key::_0: return "0";
	case Key::_1: return "1";
	case Key::_2: return "2";
	case Key::_3: return "3";
	case Key::_4: return "4";
	case Key::_5: return "5";
	case Key::_6: return "6";
	case Key::_7: return "7";
	case Key::_8: return "8";
	case Key::_9: return "9";

	case Key::A: return "A";
	case Key::B: return "B";
	case Key::C: return "C";
	case Key::D: return "D";
	case Key::E: return "E";
	case Key::F: return "F";
	case Key::G: return "G";
	case Key::H: return "H";
	case Key::I: return "I";
	case Key::J: return "J";
	case Key::K: return "K";
	case Key::L: return "L";
	case Key::M: return "M";
	case Key::N: return "N";
	case Key::O: return "O";
	case Key::P: return "P";
	case Key::Q: return "Q";
	case Key::R: return "R";
	case Key::S: return "S";
	case Key::T: return "T";
	case Key::U: return "U";
	case Key::V: return "V";
	case Key::W: return "W";
	case Key::X: return "X";
	case Key::Y: return "Y";

	case Key::Up:	 return "Up";
	case Key::Down:  return "Down";
	case Key::Left:	 return "Left";
	case Key::Right: return "Right";

	case Key::Num0: return "Num0";
	case Key::Num1: return "Num1";
	case Key::Num2: return "Num2";
	case Key::Num3: return "Num3";
	case Key::Num4: return "Num4";
	case Key::Num5: return "Num5";
	case Key::Num6: return "Num6";
	case Key::Num7: return "Num7";
	case Key::Num8: return "Num8";
	case Key::Num9: return "Num9";

	default: return "*";
	};
}

inline std::ostream& operator<<(std::ostream& ost, Key const key)
{
	ost << KeyToString(key);
	return ost;
}
