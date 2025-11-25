#include "MenuManager.h"

// ------------------------------------------------------------
// Add a menu using a custom ScreenLoader
// ------------------------------------------------------------
void MenuManager::addMenu(const String &name, ScreenLoader loader, ScreenLoader destroyer)
{
    if (!loader)
    {
        if (_logger)
            _logger->warn("[MenuManager] addMenu called with nullptr loader for: " + name);
        return;
    }

    _menus.push_back({name, loader, destroyer});

    if (_logger)
        _logger->info("[MenuManager] Added menu (custom loader): " + name);
}

// ------------------------------------------------------------
// Add a menu using ONLY an init function
// (SquareLine initFunc handles lv_scr_load internally)
// ------------------------------------------------------------
void MenuManager::addMenu(const String &name, void (*initFunc)(), void (*destroyFunc)())
{
    if (!initFunc)
    {
        if (_logger)
            _logger->error("[MenuManager] addMenu called with nullptr initFunc for: " + name);
        return;
    }

    _menus.push_back({name, makeLoader(initFunc, _logger), makeDestroyer(destroyFunc, _logger)});

    if (_logger)
        _logger->info("[MenuManager] Added menu (initFunc only): " + name);
}

// ------------------------------------------------------------
// Private helper to wrap initFunc into a ScreenLoader
// ------------------------------------------------------------
// MenuManager::ScreenLoader MenuManager::makeLoader(void (*initFunc)(), RumpshiftLogger *logger)
// {
//     return [initFunc, logger]()
//     {
//         if (!initFunc)
//         {
//             if (logger)
//                 logger->error("[MenuManager] initFunc is nullptr");
//             return;
//         }

//         if (logger)
//             logger->info("[MenuManager] Running init function");

//         // NOTE:
//         // SquareLine init function ALREADY creates the screen + calls lv_scr_load()
//         initFunc();
//     };
// }

// ------------------------------------------------------------
// Get total menu count
// ------------------------------------------------------------
size_t MenuManager::getMenuCount() const
{
    return _menus.size();
}

// ------------------------------------------------------------
// Get menu name by index
// ------------------------------------------------------------
String MenuManager::getMenu(size_t index) const
{
    if (index >= _menus.size())
    {
        if (_logger)
            _logger->warn("[MenuManager] getMenu invalid index: " + String(index));
        return "";
    }

    return _menus[index].name;
}

// ------------------------------------------------------------
// Return all menu names concatenated with a delimiter
// (default = newline for LVGL dropdowns)
// ------------------------------------------------------------
String MenuManager::getMenusDelimit(const String &delimiter) const
{
    String result;

    for (size_t i = 0; i < _menus.size(); i++)
    {
        result += _menus[i].name;
        if (i < _menus.size() - 1)
            result += delimiter;
    }

    return result;
}

// ------------------------------------------------------------
// Load menu by index (executes loader or initFunc())
// ------------------------------------------------------------
void MenuManager::loadMenu(size_t index) const
{
    if (index >= _menus.size())
    {
        if (_logger)
            _logger->error("[MenuManager] loadMenu invalid index: " + String(index));
        return;
    }

    if (!_menus[index].loader)
    {
        if (_logger)
            _logger->warn("[MenuManager] loadMenu called but loader is nullptr for: " + _menus[index].name);
        return;
    }

    // Destroy previous screen
    if (_currentIndex >= 0 && _currentIndex < _menus.size())
    {
        if (_menus[_currentIndex].destroyer)
            _menus[_currentIndex].destroyer();
    }

    // Load new screen
    if (_logger)
        _logger->info("[MenuManager] Loading menu: " + _menus[index].name);
    _menus[index].loader();
    _currentIndex = index;
}
