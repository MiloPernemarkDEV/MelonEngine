#ifndef MELONENGINE_KEY_BINDINGS_H
#define MELONENGINE_KEY_BINDINGS_H


#include <string>
// #include <functional>
#include <vector>

// goal, abstraction from GLFWs key bindings to get more flexibility
enum Keycode {
    KEY_SPACE              = 32,
    KEY_APOSTROPHE         = 39,
    KEY_COMMA              = 44,
    KEY_MINUS              = 45,
    KEY_PERIOD             = 46,
    KEY_SLASH              = 47,

    KEY_0                  = 48,
    KEY_1                  = 49,
    KEY_2                  = 50,
    KEY_3                  = 51,
    KEY_4                  = 52,
    KEY_5                  = 53,
    KEY_6                  = 54,
    KEY_7                  = 55,
    KEY_8                  = 56,
    KEY_9                  = 57,

    KEY_SEMICOLON          = 59,
    KEY_EQUAL              = 61,

    KEY_A                  = 65,
    KEY_B                  = 66,
    KEY_C                  = 67,
    KEY_D                  = 68,
    KEY_E                  = 69,
    KEY_F                  = 70,
    KEY_G                  = 71,
    KEY_H                  = 72,
    KEY_I                  = 73,
    KEY_J                  = 74,
    KEY_K                  = 75,
    KEY_L                  = 76,
    KEY_M                  = 77,
    KEY_N                  = 78,
    KEY_O                  = 79,
    KEY_P                  = 80,
    KEY_Q                  = 81,
    KEY_R                  = 82,
    KEY_S                  = 83,
    KEY_T                  = 84,
    KEY_U                  = 85,
    KEY_V                  = 86,
    KEY_W                  = 87,
    KEY_X                  = 88,
    KEY_Y                  = 89,
    KEY_Z                  = 90,

    KEY_LEFT_BRACKET       = 91,
    KEY_BACKSLASH          = 92,
    KEY_RIGHT_BRACKET      = 93,
    KEY_GRAVE_ACCENT       = 96,

    KEY_ESCAPE             = 256,
    KEY_ENTER              = 257,
    KEY_TAB                = 258,
    KEY_BACKSPACE          = 259,
    KEY_INSERT             = 260,
    KEY_DELETE             = 261,

    KEY_RIGHT              = 262,
    KEY_LEFT               = 263,
    KEY_DOWN               = 264,
    KEY_UP                 = 265,

    KEY_PAGE_UP            = 266,
    KEY_PAGE_DOWN          = 267,
    KEY_HOME               = 268,
    KEY_END                = 269,

    KEY_CAPS_LOCK          = 280,

    KEY_F1                 = 290,
    KEY_F2                 = 291,
    KEY_F3                 = 292,
    KEY_F4                 = 293,
    KEY_F5                 = 294,
    KEY_F6                 = 295,
    KEY_F7                 = 296,
    KEY_F8                 = 297,
    KEY_F9                 = 298,
    KEY_F10                = 299,
    KEY_F11                = 300,
    KEY_F12                = 301,

    KEY_KP_0               = 320,
    KEY_KP_1               = 321,
    KEY_KP_2               = 322,
    KEY_KP_3               = 323,
    KEY_KP_4               = 324,
    KEY_KP_5               = 325,
    KEY_KP_6               = 326,
    KEY_KP_7               = 327,
    KEY_KP_8               = 328,
    KEY_KP_9               = 329,

    KEY_KP_DECIMAL         = 330,
    KEY_KP_DIVIDE          = 331,
    KEY_KP_MULTIPLY        = 332,
    KEY_KP_SUBTRACT        = 333,
    KEY_KP_ADD             = 334,
    KEY_KP_ENTER           = 335,

    KEY_LEFT_SHIFT         = 340,
    KEY_LEFT_CONTROL       = 341,
    KEY_LEFT_ALT           = 342,
    KEY_LEFT_SUPER         = 343,

    KEY_RIGHT_SHIFT        = 344,
    KEY_RIGHT_CONTROL      = 345,
    KEY_RIGHT_ALT          = 346,
    KEY_RIGHT_SUPER        = 347,
    NONE                   = 0,
};

#define KEY_CODE_LIST \
    X(KEY_SPACE, "Space") \
    X(KEY_APOSTROPHE, "'") \
    X(KEY_COMMA, ",") \
    X(KEY_MINUS, "-") \
    X(KEY_PERIOD, ".") \
    X(KEY_SLASH, "/") \
\
    X(KEY_0, "0") \
    X(KEY_1, "1") \
    X(KEY_2, "2") \
    X(KEY_3, "3") \
    X(KEY_4, "4") \
    X(KEY_5, "5") \
    X(KEY_6, "6") \
    X(KEY_7, "7") \
    X(KEY_8, "8") \
    X(KEY_9, "9") \
\
    X(KEY_SEMICOLON, ";") \
    X(KEY_EQUAL, "=") \
\
    X(KEY_A, "A") \
    X(KEY_B, "B") \
    X(KEY_C, "C") \
    X(KEY_D, "D") \
    X(KEY_E, "E") \
    X(KEY_F, "F") \
    X(KEY_G, "G") \
    X(KEY_H, "H") \
    X(KEY_I, "I") \
    X(KEY_J, "J") \
    X(KEY_K, "K") \
    X(KEY_L, "L") \
    X(KEY_M, "M") \
    X(KEY_N, "N") \
    X(KEY_O, "O") \
    X(KEY_P, "P") \
    X(KEY_Q, "Q") \
    X(KEY_R, "R") \
    X(KEY_S, "S") \
    X(KEY_T, "T") \
    X(KEY_U, "U") \
    X(KEY_V, "V") \
    X(KEY_W, "W") \
    X(KEY_X, "X") \
    X(KEY_Y, "Y") \
    X(KEY_Z, "Z") \
\
    X(KEY_LEFT_BRACKET, "[") \
    X(KEY_BACKSLASH, "\\") \
    X(KEY_RIGHT_BRACKET, "]") \
    X(KEY_GRAVE_ACCENT, "`") \
\
    X(KEY_ESCAPE, "Escape") \
    X(KEY_ENTER, "Enter") \
    X(KEY_TAB, "Tab") \
    X(KEY_BACKSPACE, "Backspace") \
    X(KEY_INSERT, "Insert") \
    X(KEY_DELETE, "Delete") \
\
    X(KEY_RIGHT, "Right") \
    X(KEY_LEFT, "Left") \
    X(KEY_DOWN, "Down") \
    X(KEY_UP, "Up") \
\
    X(KEY_PAGE_UP, "Page Up") \
    X(KEY_PAGE_DOWN, "Page Down") \
    X(KEY_HOME, "Home") \
    X(KEY_END, "End") \
\
    X(KEY_CAPS_LOCK, "Caps Lock") \
\
    X(KEY_F1, "F1") \
    X(KEY_F2, "F2") \
    X(KEY_F3, "F3") \
    X(KEY_F4, "F4") \
    X(KEY_F5, "F5") \
    X(KEY_F6, "F6") \
    X(KEY_F7, "F7") \
    X(KEY_F8, "F8") \
    X(KEY_F9, "F9") \
    X(KEY_F10, "F10") \
    X(KEY_F11, "F11") \
    X(KEY_F12, "F12") \
\
    X(KEY_KP_0, "Num 0") \
    X(KEY_KP_1, "Num 1") \
    X(KEY_KP_2, "Num 2") \
    X(KEY_KP_3, "Num 3") \
    X(KEY_KP_4, "Num 4") \
    X(KEY_KP_5, "Num 5") \
    X(KEY_KP_6, "Num 6") \
    X(KEY_KP_7, "Num 7") \
    X(KEY_KP_8, "Num 8") \
    X(KEY_KP_9, "Num 9") \
\
    X(KEY_KP_DECIMAL, "Num .") \
    X(KEY_KP_DIVIDE, "Num /") \
    X(KEY_KP_MULTIPLY, "Num *") \
    X(KEY_KP_SUBTRACT, "Num -") \
    X(KEY_KP_ADD, "Num +") \
    X(KEY_KP_ENTER, "Num Enter") \
\
    X(KEY_LEFT_SHIFT, "Left Shift") \
    X(KEY_LEFT_CONTROL, "Left Ctrl") \
    X(KEY_LEFT_ALT, "Left Alt") \
    X(KEY_LEFT_SUPER, "Left Super") \
\
    X(KEY_RIGHT_SHIFT, "Right Shift") \
    X(KEY_RIGHT_CONTROL, "Right Ctrl") \
    X(KEY_RIGHT_ALT, "Right Alt") \
    X(KEY_RIGHT_SUPER, "Right Super")


inline const char* KeyCodeToString(Keycode key)
{
    switch (key)
    {
#define X(code, name) case code: return name;
        KEY_CODE_LIST
        #undef X
        default: return "Unknown";
    }
}

struct KeyBinding {
    Keycode key;

     const char* to_c_str() {
        switch (key)
        {
            #define X(code, name) case code: return name;
            KEY_CODE_LIST
            #undef X
            default: return "Unknown";
        }
    }
};

/* When it's needed to expand on this to allow keybindings, but it's okay for now how it is
 *
 *  inline std::vector<KeyBinding> g_keyBindings;
 *
 *  inline KeyBinding bindF3{
 *      .keyCode = KeyCodes::KEY_F3
 *  };
*/


#endif // MELONENGINE_KEY_BINDINGS_H