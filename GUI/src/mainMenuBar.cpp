

#include "mainMenuBar.h"

#include "imgui.h"

#include <iostream>
#include <queue>

void MainMenuBar::addMenu(MenuProperties* menuProp)
{
    if(nullptr == menuProp->parent)
    {
        menus.push_back(menuProp);
    }
    else
    {
        std::cout << "Parent Addr" << menuProp->parent << "\n";

        for(auto& item : menus)
        {
            if(item == menuProp->parent)
            {
                item->childs.push_back(menuProp);
            }
        }
        
    }
}

static void createMainMenuItem(std::list<MainMenuBar::MenuProperties *>::iterator iter)
{
    static std::queue<std::list<MainMenuBar::MenuProperties *>::iterator> prevRoots;

    if(iter.operator*()->type == MainMenuBar::MenuType::MENU_TYPE_TITLE)
    {
        std::cout << "Title: " << iter.operator*()->menuName << "\n";
        prevRoots.push(iter);
        ImGui::BeginMenu(iter.operator*()->menuName);
        return createMainMenuItem(iter.operator*()->childs.begin());
    }
    else
    {
        if(prevRoots.back().operator*()->childs.end() != iter)
        {
            std::cout << "Item: " << iter.operator*()->menuName << "\n";
            ImGui::MenuItem(iter.operator*()->menuName);
            
        }

        if(iter++ == prevRoots.back().operator*()->childs.end())
        {
            ImGui::EndMenu();
            auto returnIter = prevRoots.back();
            prevRoots.pop();
        }
        else
        {
            return createMainMenuItem(std::next(iter));   
        }
    }
}

void MainMenuBar::execute()
{
    createMainMenuItem(menus.begin());

}