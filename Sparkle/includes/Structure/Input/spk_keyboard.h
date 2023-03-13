#pragma once

#include <iostream>
#include <stdint.h>
#include <map>
#include <string>
#include "spk_input_status.h"
#include "Structure/Utils/spk_singleton.h"

namespace spk
{
    class Keyboard : public spk::Singleton<Keyboard>
    {
        friend class spk::Singleton<Keyboard>;
        friend class KeyboardModule;
    public:
        static const size_t C_NB_KEY = 255;

        enum Key
        {
            Backspace = 8,
            Tab = 9,
            Clear = 12,
            Return = 13,
            Shift = 16,
            Control = 17,
            Alt = 18,
            Pause = 19,
            Capslock = 20,
            Escape = 27,
            Convert = 28,
            Non_convert = 29,
            Accept = 30,
            Mode_change = 31,
            Space = 32,
            Prior = 33,
            Next = 34,
            End = 35,
            Home = 36,
            LeftArrow = 37,
            UpArrow = 38,
            RightArrow = 39,
            DownArrow = 40,
            Select = 41,
            Print = 42,
            Execute = 43,
            Snapshot = 44,
            Insert = 45,
            Delete = 46,
            Help = 47,
            Key0 = 48,
            Key1 = 49,
            Key2 = 50,
            Key3 = 51,
            Key4 = 52,
            Key5 = 53,
            Key6 = 54,
            Key7 = 55,
            Key8 = 56,
            Key9 = 57,
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
            LeftWindows = 91,
            RightWindows = 92,
            App = 93,
            Sleep = 95,
            Numpad0 = 96,
            Numpad1 = 97,
            Numpad2 = 98,
            Numpad3 = 99,
            Numpad4 = 100,
            Numpad5 = 101,
            Numpad6 = 102,
            Numpad7 = 103,
            Numpad8 = 104,
            Numpad9 = 105,
            NumpadMultiply = 106,
            NumpadPlus = 107,
            NumpadSeparator = 108,
            NumpadMinus = 109,
            NumpadDecimal = 110,
            NumpadDivide = 111,
            F1 = 112,
            F2 = 113,
            F3 = 114,
            F4 = 115,
            F5 = 116,
            F6 = 117,
            F7 = 118,
            F8 = 119,
            F9 = 120,
            F10 = 121,
            F11 = 122,
            F12 = 123,
            F13 = 124,
            F14 = 125,
            F15 = 126,
            F16 = 127,
            F17 = 128,
            F18 = 129,
            F19 = 130,
            F20 = 131,
            F21 = 132,
            F22 = 133,
            F23 = 134,
            F24 = 135,
            NumLock = 144,
            Scroll = 145,
            LeftShift = 160,
            RightShift = 161,
            LeftControl = 162,
            RightControl = 163,
            LeftMenu = 164,
            RightMenu = 165,
            SemiColon = 186,
            Plus = 187,
            Comma = 188,
            Minus = 189,
            Period = 190,
            QuestionMark = 191,
            Tilde = 192,
            LeftBracket = 219,
            VerticalLine = 220,
            RightBracket = 221,
            Quote = 222,
            Unknow = 223,
            AngleBracket = 226,
            Process = 229,
        };
        static inline std::map<Keyboard::Key, std::string> KeyToStringMap = {
            {Keyboard::Key::Backspace, "Backspace"},
            {Keyboard::Key::Tab, "Tab"},
            {Keyboard::Key::Clear, "Clear"},
            {Keyboard::Key::Return, "Return"},
            {Keyboard::Key::Shift, "Shift"},
            {Keyboard::Key::Control, "Control"},
            {Keyboard::Key::Alt, "Alt"},
            {Keyboard::Key::Pause, "Pause"},
            {Keyboard::Key::Capslock, "Capslock"},
            {Keyboard::Key::Escape, "Escape"},
            {Keyboard::Key::Convert, "Convert"},
            {Keyboard::Key::Non_convert, "Non_convert"},
            {Keyboard::Key::Accept, "Accept"},
            {Keyboard::Key::Mode_change, "Mode_change"},
            {Keyboard::Key::Space, "Space"},
            {Keyboard::Key::Prior, "Prior"},
            {Keyboard::Key::Next, "Next"},
            {Keyboard::Key::End, "End"},
            {Keyboard::Key::Home, "Home"},
            {Keyboard::Key::LeftArrow, "LeftArrow"},
            {Keyboard::Key::UpArrow, "UpArrow"},
            {Keyboard::Key::RightArrow, "RightArrow"},
            {Keyboard::Key::DownArrow, "DownArrow"},
            {Keyboard::Key::Select, "Select"},
            {Keyboard::Key::Print, "Print"},
            {Keyboard::Key::Execute, "Execute"},
            {Keyboard::Key::Snapshot, "Snapshot"},
            {Keyboard::Key::Insert, "Insert"},
            {Keyboard::Key::Delete, "Delete"},
            {Keyboard::Key::Help, "Help"},
            {Keyboard::Key::Key0, "Key0"},
            {Keyboard::Key::Key1, "Key1"},
            {Keyboard::Key::Key2, "Key2"},
            {Keyboard::Key::Key3, "Key3"},
            {Keyboard::Key::Key4, "Key4"},
            {Keyboard::Key::Key5, "Key5"},
            {Keyboard::Key::Key6, "Key6"},
            {Keyboard::Key::Key7, "Key7"},
            {Keyboard::Key::Key8, "Key8"},
            {Keyboard::Key::Key9, "Key9"},
            {Keyboard::Key::A, "A"},
            {Keyboard::Key::B, "B"},
            {Keyboard::Key::C, "C"},
            {Keyboard::Key::D, "D"},
            {Keyboard::Key::E, "E"},
            {Keyboard::Key::F, "F"},
            {Keyboard::Key::G, "G"},
            {Keyboard::Key::H, "H"},
            {Keyboard::Key::I, "I"},
            {Keyboard::Key::J, "J"},
            {Keyboard::Key::K, "K"},
            {Keyboard::Key::L, "L"},
            {Keyboard::Key::M, "M"},
            {Keyboard::Key::N, "N"},
            {Keyboard::Key::O, "O"},
            {Keyboard::Key::P, "P"},
            {Keyboard::Key::Q, "Q"},
            {Keyboard::Key::R, "R"},
            {Keyboard::Key::S, "S"},
            {Keyboard::Key::T, "T"},
            {Keyboard::Key::U, "U"},
            {Keyboard::Key::V, "V"},
            {Keyboard::Key::W, "W"},
            {Keyboard::Key::X, "X"},
            {Keyboard::Key::Y, "Y"},
            {Keyboard::Key::Z, "Z"},
            {Keyboard::Key::LeftWindows, "LeftWindows"},
            {Keyboard::Key::RightWindows, "RightWindows"},
            {Keyboard::Key::App, "App"},
            {Keyboard::Key::Sleep, "Sleep"},
            {Keyboard::Key::Numpad0, "Numpad0"},
            {Keyboard::Key::Numpad1, "Numpad1"},
            {Keyboard::Key::Numpad2, "Numpad2"},
            {Keyboard::Key::Numpad3, "Numpad3"},
            {Keyboard::Key::Numpad4, "Numpad4"},
            {Keyboard::Key::Numpad5, "Numpad5"},
            {Keyboard::Key::Numpad6, "Numpad6"},
            {Keyboard::Key::Numpad7, "Numpad7"},
            {Keyboard::Key::Numpad8, "Numpad8"},
            {Keyboard::Key::Numpad9, "Numpad9"},
            {Keyboard::Key::NumpadMultiply, "NumpadMultiply"},
            {Keyboard::Key::NumpadPlus, "NumpadPlus"},
            {Keyboard::Key::NumpadSeparator, "NumpadSeparator"},
            {Keyboard::Key::NumpadMinus, "NumpadMinus"},
            {Keyboard::Key::NumpadDecimal, "NumpadDecimal"},
            {Keyboard::Key::NumpadDivide, "NumpadDivide"},
            {Keyboard::Key::F1, "F1"},
            {Keyboard::Key::F2, "F2"},
            {Keyboard::Key::F3, "F3"},
            {Keyboard::Key::F4, "F4"},
            {Keyboard::Key::F5, "F5"},
            {Keyboard::Key::F6, "F6"},
            {Keyboard::Key::F7, "F7"},
            {Keyboard::Key::F8, "F8"},
            {Keyboard::Key::F9, "F9"},
            {Keyboard::Key::F10, "F10"},
            {Keyboard::Key::F11, "F11"},
            {Keyboard::Key::F12, "F12"},
            {Keyboard::Key::F13, "F13"},
            {Keyboard::Key::F14, "F14"},
            {Keyboard::Key::F15, "F15"},
            {Keyboard::Key::F16, "F16"},
            {Keyboard::Key::F17, "F17"},
            {Keyboard::Key::F18, "F18"},
            {Keyboard::Key::F19, "F19"},
            {Keyboard::Key::F20, "F20"},
            {Keyboard::Key::F21, "F21"},
            {Keyboard::Key::F22, "F22"},
            {Keyboard::Key::F23, "F23"},
            {Keyboard::Key::F24, "F24"},
            {Keyboard::Key::NumLock, "NumLock"},
            {Keyboard::Key::Scroll, "Scroll"},
            {Keyboard::Key::LeftShift, "LeftShift"},
            {Keyboard::Key::RightShift, "RightShift"},
            {Keyboard::Key::LeftControl, "LeftControl"},
            {Keyboard::Key::RightControl, "RightControl"},
            {Keyboard::Key::LeftMenu, "LeftMenu"},
            {Keyboard::Key::RightMenu, "RightMenu"},
            {Keyboard::Key::SemiColon, "SemiColon"},
            {Keyboard::Key::Plus, "Plus"},
            {Keyboard::Key::Comma, "Comma"},
            {Keyboard::Key::Minus, "Minus"},
            {Keyboard::Key::Period, "Period"},
            {Keyboard::Key::QuestionMark, "QuestionMark"},
            {Keyboard::Key::Tilde, "Tilde"},
            {Keyboard::Key::LeftBracket, "LeftBracket"},
            {Keyboard::Key::VerticalLine, "VerticalLine"},
            {Keyboard::Key::RightBracket, "RightBracket"},
            {Keyboard::Key::Quote, "Quote"},
            {Keyboard::Key::Unknow, "Unknow"},
            {Keyboard::Key::AngleBracket, "AngleBracket"},
            {Keyboard::Key::Process, "Process"},
        };
    private:
        InputStatus _keys[C_NB_KEY];
        uint8_t _entry;

        void _updateState()
        {
            _entry = '\0';
            for (size_t i = 0; i < C_NB_KEY; i++)
            {
                if (_keys[i] == InputStatus::Pressed)
                {
                    _keys[i] = InputStatus::Down;
                }
                else if (_keys[i] == InputStatus::Released)
                {
                    _keys[i] = InputStatus::Up;
                }
            }
        }
        void _updateEntry(uint8_t p_entry) {
            _entry = p_entry;
        }
        Keyboard() :
            _entry('\0')
        {
            for (size_t i = 0; i < C_NB_KEY; i++)
                _keys[i] = InputStatus::Up;
        }

    public:
        const InputStatus& getKey(Keyboard::Key p_key) const
        {
            return (_keys[p_key]);
        }
        const uint8_t& getEntry() const
        {
            return (_entry);
        }

        friend std::ostream& operator<<(std::ostream& p_os, const Keyboard::Key& p_key)
        {
            if (KeyToStringMap.count(p_key) == 0)
                p_os << "Unknow key";
            else 
                p_os << KeyToStringMap[p_key];

            return p_os;
        }
    };
}