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

int MenuManager::getMenuIndexByName(const String &name) const
{
    for (size_t i = 0; i < _menus.size(); i++)
    {
        if (_menus[i].name == name)
            return static_cast<int>(i);
    }
    return -1;
}

String MenuManager::getMenusDelimit(const String &delimiter,
                                    const String &excludeList) const
{
    // Parse exclude list into a vector<String>
    std::vector<String> exclude;
    if (excludeList.length() > 0)
    {
        int start = 0;
        while (true)
        {
            int idx = excludeList.indexOf(',', start);
            String token;

            if (idx == -1)
            {
                token = excludeList.substring(start);
            }
            else
            {
                token = excludeList.substring(start, idx);
                start = idx + 1;
            }

            // Trim whitespace
            token.trim();

            if (token.length() > 0)
                exclude.push_back(token);

            if (idx == -1)
                break;
        }
    }

    // Build output
    String result;
    bool first = true;

    for (const auto &m : _menus)
    {
        // Check if name should be excluded
        bool skip = false;
        for (const auto &ex : exclude)
        {
            if (m.name == ex)
            {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;

        // Append with delimiter rules
        if (!first)
            result += delimiter;

        result += m.name;
        first = false;
    }

    return result;
}

// ------------------------------------------------------------
// Load menu by index (executes loader or initFunc())
// ------------------------------------------------------------
void MenuManager::loadMenu(size_t index, bool callDestroyOnPreviousScreen) const
{
    if (index >= _menus.size())
    {
        if (_logger)
            _logger->error("[MenuManager] loadMenu invalid index: " + String(index));
        return;
    }

    const String &newName = _menus[index].name;

    // --- Destroy previous screen if requested ---
    if (callDestroyOnPreviousScreen && _currentIndex >= 0 && _currentIndex < _menus.size())
    {
        const String &oldName = _menus[_currentIndex].name;

        if (_logger)
            _logger->info("[MenuManager] Destroying previous screen: " + oldName);

        if (_menus[_currentIndex].destroyer)
        {
            _menus[_currentIndex].destroyer();

            if (_logger)
                _logger->info("[MenuManager] Destroyer executed for: " + oldName);
        }
        else
        {
            if (_logger)
                _logger->warn("[MenuManager] No destroyer defined for previous screen: " + oldName);
        }
    }
    else if (_logger)
    {
        _logger->info("[MenuManager] Skipping destroy of previous screen");
    }

    // --- Load new screen ---
    if (_logger)
        _logger->info("[MenuManager] Loading menu: " + newName);

    // -- Check for cached screen --
    if (_menus[index].cachedScreen)
    {
        if (_logger)
            _logger->info("[MenuManager] Loading cached screen: " + newName);
        lv_scr_load(_menus[index].cachedScreen);

        if (_menus[index].updater)
        {
            if (_logger)
                _logger->info("[MenuManager] Running updater for cached screen: " + newName);
            _menus[index].updater();
        }
        return;
    }

    if (!_menus[index].loader)
    {
        if (_logger)
            _logger->error("[MenuManager] Loader is nullptr for: " + newName);
        return;
    }

    _menus[index].loader();
    _currentIndex = index;

    if (_logger)
        _logger->info("[MenuManager] Load complete: " + newName);
}

// ------------------------------------------------------------
// Load menu by name
// ------------------------------------------------------------
bool MenuManager::loadMenu(const String &name, bool callDestroyOnPreviousScreen) const
{
    int idx = getMenuIndexByName(name);
    if (idx < 0)
    {
        if (_logger)
            _logger->error("[MenuManager] Menu not found: " + name);
        return false;
    }

    loadMenu(static_cast<size_t>(idx), callDestroyOnPreviousScreen);
    return true;
}

void MenuManager::queueMenu(const String &name, bool callDestroyOnPreviousScreen)
{
    // Append requested menu
    _loadQueue.push_back({name, callDestroyOnPreviousScreen});

    if (_logger)
    {
        _logger->info("[MenuManager] Queued menu: " + name +
                      " (destroy previous: " + String(callDestroyOnPreviousScreen ? "true" : "false") + ")");
    }

    // Only schedule LVGL async ONCE
    if (!_queuePending)
    {
        _queuePending = true;
        if (_logger)
            _logger->info("[MenuManager] Scheduling async pump for queued menus");
        lv_async_call(MenuManager::_asyncPump, this);
    }
}

void MenuManager::_asyncPump(void *data)
{
    MenuManager *self = static_cast<MenuManager *>(data);

    if (self->_loadQueue.empty())
    {
        self->_queuePending = false;
        if (self->_logger)
            self->_logger->info("[MenuManager] Async pump: queue empty, nothing to load");
        return;
    }

    // Pop the first queued request
    auto [menuName, destroyFlag] = self->_loadQueue.front();
    self->_loadQueue.erase(self->_loadQueue.begin());

    if (self->_logger)
        self->_logger->info("[MenuManager] Async pump: loading queued menu: " + menuName +
                            " (destroy previous: " + String(destroyFlag ? "true" : "false") + ")");

    // Perform actual load (LVGL-safe)
    self->loadMenu(menuName, destroyFlag);

    // If more queued, run again
    if (!self->_loadQueue.empty())
    {
        if (self->_logger)
            self->_logger->info("[MenuManager] Async pump: more menus queued, rescheduling");
        lv_async_call(MenuManager::_asyncPump, self);
    }
    else
    {
        self->_queuePending = false;
        if (self->_logger)
            self->_logger->info("[MenuManager] Async pump: all queued menus processed");
    }
}
