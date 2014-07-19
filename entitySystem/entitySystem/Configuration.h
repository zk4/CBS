#pragma  once

enum eComponent
{
    Component_MOVE,
    Component_COCOS,
    Component_HP,
    Component_WEAPON,
    Component_AI,
    Component_RADAR,

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
    Telegram_MOVETO_POS,
    Telegram_FOLLOW_POS,
    Telegram_AI,
    Telegram_ROTATE,
    Telegram_ADD_TARGET,
    Telegram_SEARCH,
    Telegram_SEARCH_RESULT,
};
