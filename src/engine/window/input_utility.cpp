#include "input_utility.h"
#include <Windows.h>

#undef DELETE

namespace Input
{
  Key ConvertWinApiKeyToManagerKey(const unsigned int scanCode, const bool isExtended)
  {
    switch (scanCode)
    {
    case VK_LBUTTON: //Left mouse button
      return Key::LMB;

    case VK_RBUTTON: //Right mouse button
      return Key::RMB;

    case VK_MBUTTON: //Middle mouse button(three - button mouse)
      return Key::MMB;

    case VK_XBUTTON1: //X1 mouse button
      return Key::X1MB;

    case VK_XBUTTON2: //X2 mouse button
      return Key::X2MB;

    case VK_BACK: //BACKSPACE key
      return Key::BACKSPACE;

    case VK_TAB: //TAB key
      return Key::TAB;

    case VK_RETURN: //ENTER key
      return Key::ENTER;

    case VK_SHIFT: //SHIFT key
      return isExtended ? Key::LEFT_SHIFT : Key::RIGHT_SHIFT;

    case VK_CONTROL: //CTRL key
      return isExtended ? Key::LEFT_CONTROL : Key::RIGHT_CONTROL;

    case VK_MENU: //ALT key
      return isExtended ? Key::LEFT_ALT : Key::RIGHT_ALT;

    case VK_CAPITAL: //CAPS LOCK key
      return Key::CAPS_LOCK;

    case VK_ESCAPE: //ESC key
      return Key::ESCAPE;

    case VK_SPACE: //SPACEBAR
      return Key::SPACE;

    case VK_PRIOR: //PAGE UP key
      return Key::PAGE_UP;

    case VK_NEXT: //PAGE DOWN key
      return Key::PAGE_DOWN;

    case VK_END: //END key
      return Key::END;

    case VK_HOME: //HOME key
      return Key::HOME;

    case VK_LEFT: //LEFT ARROW key
      return Key::LEFT;

    case VK_UP: //UP ARROW key
      return Key::UP;

    case VK_RIGHT: //RIGHT ARROW key
      return Key::RIGHT;

    case VK_DOWN: //DOWN ARROW key
      return Key::DOWN;

    case VK_INSERT: //INS key
      return Key::INSERT;

    case VK_DELETE: //DEL key
      return Key::DELETE;

    case 0x30: //0
      return Key::NUMBER_0;

    case 0x31: //1
      return Key::NUMBER_1;

    case 0x32: //2
      return Key::NUMBER_2;

    case 0x33: //3
      return Key::NUMBER_3;

    case 0x34: //4
      return Key::NUMBER_4;

    case 0x35: //5
      return Key::NUMBER_5;

    case 0x36: //6
      return Key::NUMBER_6;

    case 0x37: //7
      return Key::NUMBER_7;

    case 0x38: //8
      return Key::NUMBER_8;

    case 0x39: //9
      return Key::NUMBER_9;

    case 0x41: //A
      return Key::A;

    case 0x42: //B
      return Key::B;

    case 0x43: //C
      return Key::C;

    case 0x44: //D
      return Key::D;

    case 0x45: //E
      return Key::E;

    case 0x46: //F
      return Key::F;

    case 0x47: //G
      return Key::G;

    case 0x48: //H
      return Key::H;

    case 0x49: //I
      return Key::I;

    case 0x4A: //J
      return Key::J;

    case 0x4B: //K
      return Key::K;

    case 0x4C: //L
      return Key::L;

    case 0x4D: //M
      return Key::M;

    case 0x4E: //N
      return Key::N;

    case 0x4F: //O
      return Key::O;

    case 0x50: //P
      return Key::P;

    case 0x51: //Q
      return Key::Q;

    case 0x52: //R
      return Key::R;

    case 0x53: //S
      return Key::S;

    case 0x54: //T
      return Key::T;

    case 0x55: //U
      return Key::U;

    case 0x56: //V
      return Key::V;

    case 0x57: //W
      return Key::W;

    case 0x58: //X
      return Key::X;

    case 0x59: //Y
      return Key::Y;

    case 0x5A: //Z
      return Key::Z;

    case VK_NUMPAD0: //Numeric keypad 0 key
      return Key::KP_0;

    case VK_NUMPAD1: //Numeric keypad 1 key
      return Key::KP_1;

    case VK_NUMPAD2: //Numeric keypad 2 key
      return Key::KP_2;

    case VK_NUMPAD3: //Numeric keypad 3 key
      return Key::KP_3;

    case VK_NUMPAD4: //Numeric keypad 4 key
      return Key::KP_4;

    case VK_NUMPAD5: //Numeric keypad 5 key
      return Key::KP_5;

    case VK_NUMPAD6: //Numeric keypad 6 key
      return Key::KP_6;

    case VK_NUMPAD7: //Numeric keypad 7 key
      return Key::KP_7;

    case VK_NUMPAD8: //Numeric keypad 8 key
      return Key::KP_8;

    case VK_NUMPAD9: //Numeric keypad 9 key
      return Key::KP_9;

    case VK_F1: //F1 key
      return Key::F1;

    case VK_F2: //F2 key
      return Key::F2;

    case VK_F3: //F3 key
      return Key::F3;

    case VK_F4: //F4 key
      return Key::F4;

    case VK_F5: //F5 key
      return Key::F5;

    case VK_F6: //F6 key
      return Key::F6;

    case VK_F7: //F7 key
      return Key::F7;

    case VK_F8: //F8 key
      return Key::F8;

    case VK_F9: //F9 key
      return Key::F9;

    case VK_F10: //F10 key
      return Key::F10;

    case VK_F11: //F11 key
      return Key::F11;

    case VK_F12: //F12 key
      return Key::F12;

    default:
      return Key::UNKNOWN;
    }
  }
}