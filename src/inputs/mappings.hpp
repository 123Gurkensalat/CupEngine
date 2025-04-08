#pragma once

#include <GLFW/glfw3.h>
#include <cassert>
#include <unordered_map>

#define     KEY_UNDEFINED       -1
#define 	KEY_SPACE           0
#define 	KEY_APOSTROPHE      1
#define 	KEY_COMMA           2
#define 	KEY_MINUS           3
#define 	KEY_PERIOD          4
#define 	KEY_SLASH           5
#define 	KEY_0               6
#define 	KEY_1               7
#define 	KEY_2               8
#define 	KEY_3               9
#define 	KEY_4               10
#define 	KEY_5               11
#define 	KEY_6               12
#define 	KEY_7               13
#define 	KEY_8               14
#define 	KEY_9               15
#define 	KEY_SEMICOLON       16
#define 	KEY_EQUAL           17
#define 	KEY_A               18
#define 	KEY_B               19
#define 	KEY_C               20
#define 	KEY_D               21
#define 	KEY_E               22
#define 	KEY_F               23
#define 	KEY_G               24
#define 	KEY_H               25
#define 	KEY_I               26
#define 	KEY_J               27
#define 	KEY_K               28
#define 	KEY_L               29
#define 	KEY_M               30
#define 	KEY_N               31
#define 	KEY_O               32
#define 	KEY_P               33
#define 	KEY_Q               34
#define 	KEY_R               35
#define 	KEY_S               36
#define 	KEY_T               37
#define 	KEY_U               38
#define 	KEY_V               39
#define 	KEY_W               40
#define 	KEY_X               41
#define 	KEY_Y               42
#define 	KEY_Z               43
#define 	KEY_LEFT_BRACKET    44
#define 	KEY_BACKSLASH       45
#define 	KEY_RIGHT_BRACKET   46
#define 	KEY_GRAVE_ACCENT    47
#define 	KEY_WORLD_1         48
#define 	KEY_WORLD_2         49
#define 	KEY_ESCAPE          50
#define 	KEY_ENTER           51
#define 	KEY_TAB             52
#define 	KEY_BACKSPACE       53
#define 	KEY_INSERT          54
#define 	KEY_DELETE          55
#define 	KEY_RIGHT           56
#define 	KEY_LEFT            57
#define 	KEY_DOWN            58
#define 	KEY_UP              59
#define 	KEY_PAGE_UP         60
#define 	KEY_PAGE_DOWN       61
#define 	KEY_HOME            62
#define 	KEY_END             63
#define 	KEY_CAPS_LOCK       64
#define 	KEY_SCROLL_LOCK     65
#define 	KEY_NUM_LOCK        66
#define 	KEY_PRINT_SCREEN    67
#define 	KEY_PAUSE           68
#define 	KEY_F1              69
#define 	KEY_F2              70
#define 	KEY_F3              71
#define 	KEY_F4              72
#define 	KEY_F5              73
#define 	KEY_F6              74
#define 	KEY_F7              75
#define 	KEY_F8              76
#define 	KEY_F9              77
#define 	KEY_F10             78
#define 	KEY_F11             79
#define 	KEY_F12             80
#define 	KEY_KP_0            81
#define 	KEY_KP_1            82
#define 	KEY_KP_2            83
#define 	KEY_KP_3            84
#define 	KEY_KP_4            85
#define 	KEY_KP_5            86
#define 	KEY_KP_6            87
#define 	KEY_KP_7            88
#define 	KEY_KP_8            89
#define 	KEY_KP_9            90
#define 	KEY_KP_DECIMAL      91
#define 	KEY_KP_DIVIDE       92
#define 	KEY_KP_MULTIPLY     93
#define 	KEY_KP_SUBTRACT     94
#define 	KEY_KP_ADD          95
#define 	KEY_KP_ENTER        96
#define 	KEY_KP_EQUAL        97
#define 	KEY_LEFT_SHIFT      98
#define 	KEY_LEFT_CONTROL    99
#define 	KEY_LEFT_ALT        100
#define 	KEY_LEFT_SUPER      101
#define 	KEY_RIGHT_SHIFT     102
#define 	KEY_RIGHT_CONTROL   103
#define 	KEY_RIGHT_ALT       104
#define 	KEY_RIGHT_SUPER     105
#define 	KEY_MENU            106

#define     KEY_COUNT           KEY_MENU + 1

namespace cup::input {
inline int glfwKey_to_key(int glfwKey) 
{
    static std::unordered_map<int, int> look_up{
        {GLFW_KEY_SPACE         , KEY_SPACE         },
        {GLFW_KEY_APOSTROPHE    , KEY_APOSTROPHE    },
        {GLFW_KEY_COMMA         , KEY_COMMA         },
        {GLFW_KEY_MINUS         , KEY_MINUS         },
        {GLFW_KEY_PERIOD        , KEY_PERIOD        },
        {GLFW_KEY_SLASH         , KEY_SLASH         },
        {GLFW_KEY_0             , KEY_0             },
        {GLFW_KEY_1             , KEY_1             },
        {GLFW_KEY_2             , KEY_2             },
        {GLFW_KEY_3             , KEY_3             },
        {GLFW_KEY_4             , KEY_4             },
        {GLFW_KEY_5             , KEY_5             },
        {GLFW_KEY_6             , KEY_6             },
        {GLFW_KEY_7             , KEY_7             },
        {GLFW_KEY_8             , KEY_8             },
        {GLFW_KEY_9             , KEY_9             },
        {GLFW_KEY_SEMICOLON     , KEY_SEMICOLON     },
        {GLFW_KEY_EQUAL         , KEY_EQUAL         },
        {GLFW_KEY_A             , KEY_A             },
        {GLFW_KEY_B             , KEY_B             },
        {GLFW_KEY_C             , KEY_C             },
        {GLFW_KEY_D             , KEY_D             },
        {GLFW_KEY_E             , KEY_E             },
        {GLFW_KEY_F             , KEY_F             },
        {GLFW_KEY_G             , KEY_G             },
        {GLFW_KEY_H             , KEY_H             },
        {GLFW_KEY_I             , KEY_I             },
        {GLFW_KEY_J             , KEY_J             },
        {GLFW_KEY_K             , KEY_K             },
        {GLFW_KEY_L             , KEY_L             },
        {GLFW_KEY_M             , KEY_M             },
        {GLFW_KEY_N             , KEY_N             },
        {GLFW_KEY_O             , KEY_O             },
        {GLFW_KEY_P             , KEY_P             },
        {GLFW_KEY_Q             , KEY_Q             },
        {GLFW_KEY_R             , KEY_R             },
        {GLFW_KEY_S             , KEY_S             },
        {GLFW_KEY_T             , KEY_T             },
        {GLFW_KEY_U             , KEY_U             },
        {GLFW_KEY_V             , KEY_V             },
        {GLFW_KEY_W             , KEY_W             },
        {GLFW_KEY_X             , KEY_X             },
        {GLFW_KEY_Y             , KEY_Y             },
        {GLFW_KEY_Z             , KEY_Z             },
        {GLFW_KEY_LEFT_BRACKET  , KEY_LEFT_BRACKET  },
        {GLFW_KEY_BACKSLASH     , KEY_BACKSLASH     },
        {GLFW_KEY_RIGHT_BRACKET , KEY_RIGHT_BRACKET },
        {GLFW_KEY_GRAVE_ACCENT  , KEY_GRAVE_ACCENT  },
        {GLFW_KEY_WORLD_1       , KEY_WORLD_1       },
        {GLFW_KEY_WORLD_2       , KEY_WORLD_2       },
        {GLFW_KEY_ESCAPE        , KEY_ESCAPE        },
        {GLFW_KEY_ENTER         , KEY_ENTER         },
        {GLFW_KEY_TAB           , KEY_TAB           },
        {GLFW_KEY_BACKSPACE     , KEY_BACKSPACE     },
        {GLFW_KEY_INSERT        , KEY_INSERT        },
        {GLFW_KEY_DELETE        , KEY_DELETE        },
        {GLFW_KEY_RIGHT         , KEY_RIGHT         },
        {GLFW_KEY_LEFT          , KEY_LEFT          },
        {GLFW_KEY_DOWN          , KEY_DOWN          },
        {GLFW_KEY_UP            , KEY_UP            },
        {GLFW_KEY_PAGE_UP       , KEY_PAGE_UP       },
        {GLFW_KEY_PAGE_DOWN     , KEY_PAGE_DOWN     },
        {GLFW_KEY_HOME          , KEY_HOME          },
        {GLFW_KEY_END           , KEY_END           },
        {GLFW_KEY_CAPS_LOCK     , KEY_CAPS_LOCK     },
        {GLFW_KEY_SCROLL_LOCK   , KEY_SCROLL_LOCK   },
        {GLFW_KEY_NUM_LOCK      , KEY_NUM_LOCK      },
        {GLFW_KEY_PRINT_SCREEN  , KEY_PRINT_SCREEN  },
        {GLFW_KEY_PAUSE         , KEY_PAUSE         },
        {GLFW_KEY_F1            , KEY_F1            },
        {GLFW_KEY_F2            , KEY_F2            },
        {GLFW_KEY_F3            , KEY_F3            },
        {GLFW_KEY_F4            , KEY_F4            },
        {GLFW_KEY_F5            , KEY_F5            },
        {GLFW_KEY_F6            , KEY_F6            },
        {GLFW_KEY_F7            , KEY_F7            },
        {GLFW_KEY_F8            , KEY_F8            },
        {GLFW_KEY_F9            , KEY_F9            },
        {GLFW_KEY_F10           , KEY_F10           },
        {GLFW_KEY_F11           , KEY_F11           },
        {GLFW_KEY_F12           , KEY_F12           },
        {GLFW_KEY_F13           , KEY_UNDEFINED     },
        {GLFW_KEY_F14           , KEY_UNDEFINED     },
        {GLFW_KEY_F15           , KEY_UNDEFINED     },
        {GLFW_KEY_F16           , KEY_UNDEFINED     },
        {GLFW_KEY_F17           , KEY_UNDEFINED     },
        {GLFW_KEY_F18           , KEY_UNDEFINED     },
        {GLFW_KEY_F19           , KEY_UNDEFINED     },
        {GLFW_KEY_F20           , KEY_UNDEFINED     },
        {GLFW_KEY_F21           , KEY_UNDEFINED     },
        {GLFW_KEY_F22           , KEY_UNDEFINED     },
        {GLFW_KEY_F23           , KEY_UNDEFINED     },
        {GLFW_KEY_F24           , KEY_UNDEFINED     },
        {GLFW_KEY_F25           , KEY_UNDEFINED     },
        {GLFW_KEY_KP_0          , KEY_KP_0          },
        {GLFW_KEY_KP_1          , KEY_KP_1          },
        {GLFW_KEY_KP_2          , KEY_KP_2          },
        {GLFW_KEY_KP_3          , KEY_KP_3          },
        {GLFW_KEY_KP_4          , KEY_KP_4          },
        {GLFW_KEY_KP_5          , KEY_KP_5          },
        {GLFW_KEY_KP_6          , KEY_KP_6          },
        {GLFW_KEY_KP_7          , KEY_KP_7          },
        {GLFW_KEY_KP_8          , KEY_KP_8          },
        {GLFW_KEY_KP_9          , KEY_KP_9          },
        {GLFW_KEY_KP_DECIMAL    , KEY_KP_DECIMAL    },
        {GLFW_KEY_KP_DIVIDE     , KEY_KP_DIVIDE     },
        {GLFW_KEY_KP_MULTIPLY   , KEY_KP_MULTIPLY   },
        {GLFW_KEY_KP_SUBTRACT   , KEY_KP_SUBTRACT   },
        {GLFW_KEY_KP_ADD        , KEY_KP_ADD        },
        {GLFW_KEY_KP_ENTER      , KEY_KP_ENTER      },
        {GLFW_KEY_KP_EQUAL      , KEY_KP_EQUAL      },
        {GLFW_KEY_LEFT_SHIFT    , KEY_LEFT_SHIFT    },
        {GLFW_KEY_LEFT_CONTROL  , KEY_LEFT_CONTROL  },
        {GLFW_KEY_LEFT_ALT      , KEY_LEFT_ALT      },
        {GLFW_KEY_LEFT_SUPER    , KEY_LEFT_SUPER    },
        {GLFW_KEY_RIGHT_SHIFT   , KEY_RIGHT_SHIFT   },
        {GLFW_KEY_RIGHT_CONTROL , KEY_RIGHT_CONTROL },
        {GLFW_KEY_RIGHT_ALT     , KEY_RIGHT_ALT     },
        {GLFW_KEY_RIGHT_SUPER   , KEY_RIGHT_SUPER   },
        {GLFW_KEY_MENU          , KEY_MENU          }
    };

    assert(look_up.find(glfwKey) != look_up.end() && "Not a supported key");

    return look_up.at(glfwKey);
}
} // end of cup::input
