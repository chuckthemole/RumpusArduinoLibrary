#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <string>
#include <functional>
#include <Arduino.h>
#include <RumpshiftLogger.h>
#include <LvglHelperUtils.h>
#include "lvgl.h"

/**
 * @class MenuManager
 * @brief Manages the list of application menus and provides
 * utility functions for dropdowns and selection.
 * Supports optional RumpshiftLogger for debug logging.
 */
class MenuManager
{
public:
    MenuManager() = default;

    /**
     * @brief Construct a MenuManager with optional logger
     * @param logger Optional RumpshiftLogger pointer for debug messages
     */
    explicit MenuManager(RumpshiftLogger *logger)
        : _logger(logger) {}

    ~MenuManager() = default;

    void setLogger(RumpshiftLogger *logger)
    {
        _logger = logger;
    }

    /**
     * Function to load a screen
     */
    using ScreenLoader = std::function<void(void)>;

    /**
     * @brief Add a new menu entry.
     * @param name Name of the menu.
     * @param loader Screen loader.
     */
    void addMenu(const String &name, ScreenLoader loader, ScreenLoader destroyer = nullptr);

    /**
     * @brief Add a menu entry using ONLY an init function.
     *
     * NOTE (refactor):
     * SquareLine init functions already call lv_scr_load()
     * so we DO NOT use or store lv_obj_t* anymore.
     *
     * @param name Menu display name.
     * @param initFunc Screen initialization function (loads screen internally).
     */
    void addMenu(const String &name, void (*initFunc)(), void (*destroyFunc)());

    /**
     * @brief Get the number of menus.
     * @return Number of menus.
     */
    size_t getMenuCount() const;

    /**
     * @brief Get menu name by index.
     * @param index Index of the menu.
     * @return Menu name as String.
     */
    String getMenu(size_t index) const;

    /**
     * @brief Get menu index by name (exact match).
     * @return Index or -1 if not found.
     */
    int getMenuIndexByName(const String &name) const;

    /**
     * @brief Get all menus as a single string delimited by `delimiter`,
     *        optionally excluding a comma-delimited list of names.
     *
     * @param delimiter Delimiter between menu names (default: "\n")
     * @param excludeList Optional comma-delimited list of menu names to filter out.
     * @return Delimited string of menu names.
     */
    String getMenusDelimit(
        const String &delimiter = "\n",
        const String &excludeList = "") const;

    /**
     * @brief Call to load a menu.
     * @param index Menu index to load.
     * @param callDestroyOnPreviousScreen Optional flag to destroy previous screen (default = true)
     */
    void loadMenu(size_t index, bool callDestroyOnPreviousScreen = false) const;

    /**
     * @brief Load a menu by display name.
     * @param name Menu display name.
     * @param callDestroyOnPreviousScreen Optional flag to destroy previous screen (default = true)
     * @return true if loaded, false if not found.
     */
    bool loadMenu(const String &name, bool callDestroyOnPreviousScreen = false) const;

    /**
     * @brief Queue a menu to be loaded safely (LVGL-async).
     *        This prevents re-entrant screen loads.
     */
    void queueMenu(const String &name, bool callDestroyOnPreviousScreen = false);

    /**
     * @brief Internal pump function called asynchronously by LVGL.
     *        Do not call directly.
     */
    static void _asyncPump(void *data);

    /**
     * @brief Set a cached LVGL screen for a menu entry.
     * This allows reusing a pre-created screen without calling init again.
     * @param menuName Name of the menu to cache the screen for.
     * @param screen Pointer to an already-initialized LVGL screen.
     * @return true if menu found and cached, false otherwise.
     */
    inline bool setCachedScreen(const String &menuName, lv_obj_t *screen)
    {
        for (auto &entry : _menus)
        {
            if (entry.name == menuName)
            {
                entry.cachedScreen = screen;
                if (_logger)
                    _logger->info("[MenuManager] Cached screen for menu: " + menuName);
                return true;
            }
        }
        if (_logger)
            _logger->warn("[MenuManager] Menu not found to cache screen: " + menuName);
        return false;
    }

    inline void setUpdater(const String &menuName, ScreenLoader fn)
    {
        for (auto &entry : _menus)
        {
            if (entry.name == menuName)
            {
                entry.updater = fn;
                if (_logger)
                    _logger->info("[MenuManager] Updater set for menu: " + menuName);
                return;
            }
        }
        if (_logger)
            _logger->warn("[MenuManager] Menu not found to set updater: " + menuName);
    }

private:
    struct MenuEntry
    {
        String name;
        ScreenLoader loader;
        ScreenLoader destroyer;
        ScreenLoader updater;
        lv_obj_t *cachedScreen = nullptr;
    };

    std::vector<MenuEntry> _menus;
    RumpshiftLogger *_logger = nullptr;
    mutable int _currentIndex = -1;
    mutable bool _queuePending = false;
    mutable std::vector<std::pair<String, bool>> _loadQueue;

    static ScreenLoader makeLoader(void (*initFunc)(), RumpshiftLogger *logger)
    {
        return [initFunc, logger]()
        {
            if (!initFunc)
            {
                if (logger)
                    logger->error("[MenuManager] initFunc is nullptr");
                return;
            }

            if (logger)
                logger->info("[MenuManager] Running init function");

            // SquareLine handles screen creation + lv_scr_load internally
            initFunc();
        };
    }

    static ScreenLoader makeDestroyer(void (*destroyFunc)(), RumpshiftLogger *logger)
    {
        return [destroyFunc, logger]()
        {
            if (!destroyFunc)
            {
                if (logger)
                    logger->error("[MenuManager] destroyFunc is nullptr");
                return;
            }

            if (logger)
                logger->info("[MenuManager] Running destroy function");

            destroyFunc();
        };
    }
};

#endif // MENU_MANAGER_H
