#pragma  once

enum eComponent
{
    Component_MOVE,
    Component_COCOS,
    Component_SPRITE,
    Component_HP,
    Component_WEAPON,
    Component_TRAIL,
    Component_AI,
    Component_RADAR,
    Component_BOX2D,
    Component_WALLS,
    Component_HexWALLS,
    Component_Bezier,
    Component_SmoothLine,
    Component_Raytrace,
    Component_Fluid,
    Component_Segment,
    Entity_,
    Component_COUNT,


};
enum eTelegram
{
    Telegram_NONE,
    Telegram_HURT,
    Telegram_DEAD,
    Telegram_ACTION_HURT,
    Telegram_SET_POS,
    Telegram_SET_ROTATION,
    Telegram_SET_SCALE,
    Telegram_SET_CONTENSIZE,
    Telegram_ARRIVE,
    Telegram_UPDATE,
    Telegram_AI,
    Telegram_DRAW,

    Telegram_ACCESS_NODE,
    Telegram_MOVE_ON_WALL,
    Telegram_CLIK_ON_WALL,
    Telegram_ADD_TARGET,
    Telegram_SEARCH,
    Telegram_SEARCH_RESULT,
    Telegram_TOUCH_DOUBLE,
    Telegram_TOUCH_BEGIN,
    Telegram_TOUCH_MOVE,
    Telegram_TOUCH_END,
    Telegram_TOUCH_CANCEL,
};
