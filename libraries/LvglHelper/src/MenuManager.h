#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <string>
#include <functional>
#include <Arduino.h>
#include <RumpshiftLogger.h>
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

    void setLogger(RumpshiftLogger *logger) { _logger = logger; }

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
    void addMenu(const String &name, void (*initFunc)(), void(*destroyFunc)());

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
     * @brief Get all menus as a single string delimited by \n.
     *        Useful for LVGL dropdown options.
     * @return Delimited string of menu names.
     */
    String getMenusDelimit(const String &delimiter = "\n") const;

    /**
     * @brief Call to load a menu.
     */
    void loadMenu(size_t index) const;

private:
    struct MenuEntry
    {
        String name;
        ScreenLoader loader;
        ScreenLoader destroyer;
    };

    std::vector<MenuEntry> _menus;
    RumpshiftLogger *_logger = nullptr;
    mutable int _currentIndex = -1;

    // NOTE: safeLoadScreen is no longer used but kept for compatibility
    // static void safeLoadScreen(lv_obj_t *screen, RumpshiftLogger *logger)
    // {
    //     if (!screen)
    //     {
    //         if (logger)
    //             logger->error("[MenuManager] Attempted to load nullptr screen!");
    //         return;
    //     }

    //     if (logger)
    //         logger->info("[MenuManager] safeLoadScreen called, but direct lv_scr_load usage is deprecated.");

    //     // We do NOT call lv_scr_load here anymore.
    // }

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
