#pragma once


#include <string>
#include <list>
#include <vector>

class MainMenuBar{
public:

    using ParentMenu = struct MenuProperties*;
    using MenuEventCb = void(*)();

    enum class MenuType{
        MENU_TYPE_TITLE,
        MENU_TYPE_ITEM
    };

    struct MenuProperties{
        MenuProperties* parent;
        MenuEventCb eventCb;
        const char * menuName;
        MenuType type;
        std::list<MenuProperties *> childs;
    };

    void execute();

    void addMenu(MenuProperties* menuProp);

private:
    std::list<MenuProperties*> menus;
};