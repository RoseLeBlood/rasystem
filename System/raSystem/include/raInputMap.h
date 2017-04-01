#pragma once

#define RKEY_ESCAPE			0x1B
#define RKEY_F1				0x70
#define RKEY_F2				0x71
#define RKEY_F3				0x72
#define RKEY_F4				0x73
#define RKEY_F5				0x74
#define RKEY_F6				0x75
#define RKEY_F7				0x76
#define RKEY_F8				0x77
#define RKEY_F9				0x78
#define RKEY_F10			0x79
#define RKEY_F11			0x7A
#define RKEY_F12			0x7B
#define RKEY_F13			0x7C
#define RKEY_F14			0x7D
#define RKEY_F15			0x7E
#define RKEY_F16			0x7F
#define RKEY_F17			0x80H
#define RKEY_F18			0x81H
#define RKEY_F19			0x82H
#define RKEY_F20			0x83H
#define RKEY_F21			0x84H
#define RKEY_F22			0x85H
#define RKEY_F23			0x86H
#define RKEY_F24			0x87H
#define RKEY_SYSRQ			0x2A
#define RKEY_SCROLL			0x91
#define RKEY_PAUSE			0x13
#define RKEY_1				0x31
#define RKEY_2				0x32
#define RKEY_3				0x33
#define RKEY_4				0x34
#define RKEY_5				0x35
#define RKEY_6				0x36
#define RKEY_7				0x37
#define RKEY_8				0x38
#define RKEY_9				0x39
#define RKEY_0				0x30
#define RKEY_MINUS			0xBD
#define RKEY_BACK			0x08
#define RKEY_INSERT			0x2D
#define RKEY_HOME			0x24
#define RKEY_PRIOR			0x21
#define RKEY_NUMLOCK		0x90
#define RKEY_DIVIDE			0x6F
#define RKEY_MULTIPLY		0x6A
#define RKEY_SUBTRACT		0x6D
#define RKEY_TAB			0x09
#define RKEY_CLEAR			0x0C
#define RKEY_A				0x41
#define RKEY_B				0x42
#define RKEY_C				0x43
#define RKEY_D				0x44
#define RKEY_E				0x45
#define RKEY_F				0x46
#define RKEY_G				0x47
#define RKEY_H				0x48
#define RKEY_I				0x49
#define RKEY_J				0x4A
#define RKEY_K				0x4B
#define RKEY_L				0x4C
#define RKEY_M				0x4D
#define RKEY_N				0x4E
#define RKEY_O				0x4F
#define RKEY_P				0x50
#define RKEY_Q				0x51
#define RKEY_R				0x52
#define RKEY_S				0x53
#define RKEY_T				0x54
#define RKEY_U				0x55
#define RKEY_V				0x56
#define RKEY_W				0x57
#define RKEY_X				0x58
#define RKEY_Y				0x59
#define RKEY_Z				0x5A
#define RKEY_RETURN			0x0D
#define RKEY_DELETE			0x2E
#define RKEY_END			0x23
#define RKEY_NEXT			0x22
#define RKEY_NUMPAD0		0x60
#define RKEY_NUMPAD1		0x61
#define RKEY_NUMPAD2		0x62
#define RKEY_NUMPAD3		0x63
#define RKEY_NUMPAD4		0x64
#define RKEY_NUMPAD5		0x65
#define RKEY_NUMPAD6		0x66
#define RKEY_NUMPAD7		0x67
#define RKEY_NUMPAD8		0x68
#define RKEY_NUMPAD9		0x69
#define RKEY_ADD			0x6B
#define RKEY_CAPITAL		0x14
#define RKEY_OEM_102		0xE2
#define RKEY_COMMA			0x6E
#define RKEY_PERIOD			0xBE
#define RKEY_LSHIFT			0xA0
#define RKEY_RSHIFT			0xA1
#define RKEY_UP				0x26
#define RKEY_LCONTROL		0xA2
#define RKEY_RCONTROL		0xA3
#define RKEY_LWIN			0x5B
#define RKEY_SPACE			0x20
#define RKEY_ALT			0x12
#define RKEY_RWIN			0x5C
#define RKEY_APPS			0x5D
#define RKEY_LEFT			0x25
#define RKEY_DOWN			0x28
#define RKEY_RIGHT			0x27
#define RKEY_DECIMAL		0x6E

// Mausknopfdefinitionen
#define RMOUSE_X_NEG		(108 + 0)		// x-Achse negativ
#define RMOUSE_X_POS		(108 + 1)		// x-Achse positiv
#define RMOUSE_Y_NEG		(108 + 2)		// ...
#define RMOUSE_Y_POS		(108 + 3)		// ...
#define RMOUSE_Z_NEG		(108 + 4)		// ...
#define RMOUSE_Z_POS		(108 + 5)		// ...
#define RMOUSE_LBUTTON		0x01
#define RMOUSE_RBUTTON		0x02
#define RMOUSE_MBUTTON		0x04
#define RMOUSE_X1BUTTON		0x05
#define RMOUSE_X2BUTTON		0x06

// Joystickknopfdefinitionen
#define RJOY_X_NEG(j)			(122 + (j) * 66 + 0)			// x-Achse negativ
#define RJOY_X_POS(j)			(122 + (j) * 66 + 1)			// x-Achse positiv
#define RJOY_Y_NEG(j)			(122 + (j) * 66 + 2)			// ...
#define RJOY_Y_POS(j)			(122 + (j) * 66 + 3)			// ...
#define RJOY_Z_NEG(j)			(122 + (j) * 66 + 4)			// ...
#define RJOY_Z_POS(j)			(122 + (j) * 66 + 5)			// ...
#define RJOY_RX_NEG(j)			(122 + (j) * 66 + 6)			// Rx-Achse negativ
#define RJOY_RX_POS(j)			(122 + (j) * 66 + 7)			// Rx-Achse positiv
#define RJOY_RY_NEG(j)			(122 + (j) * 66 + 8)			// ...
#define RJOY_RY_POS(j)			(122 + (j) * 66 + 9)			// ...
#define RJOY_RZ_NEG(j)			(122 + (j) * 66 + 10)			// ...
#define RJOY_RZ_POS(j)			(122 + (j) * 66 + 11)			// ...
#define RJOY_SLIDER_NEG(j, i)	(122 + (j) * 66 + 12 + (i * 3))	// Schieberegler Nr. i negativ (0 bis 1)
#define RJOY_SLIDER_POS(j, i)	(122 + (j) * 66 + 13 + (i * 3))	// Schieberegler Nr. i positiv (0 bis 1)
#define RJOY_SLIDER_ABS(j, i)	(122 + (j) * 66 + 14 + (i * 3))	// Schieberegler Nr. i positiv (0 bis 1)
#define RJOY_POV_UP(j, i)		(122 + (j) * 66 + 18 + (i * 4))	// POV-Controller Nr. i hoch (0 bis 3)
#define RJOY_POV_RIGHT(j, i)	(122 + (j) * 66 + 19 + (i * 4))	// ...
#define RJOY_POV_DOWN(j, i)		(122 + (j) * 66 + 20 + (i * 4))	// ...
#define RJOY_POV_LEFT(j, i)		(122 + (j) * 66 + 21 + (i * 4))	// ...
#define RJOY_BUTTON(j, i)		(122 + (j) * 66 + 34 + (i))     // Joystickknopf Nr. i (0 bis 31)