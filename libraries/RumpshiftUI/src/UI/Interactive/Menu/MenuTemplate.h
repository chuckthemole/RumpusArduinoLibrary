#pragma once
#include <Arduino.h>
#include <cstddef>
#include <vector>

/**
 * @brief Defines a generic menu template for any UI system.
 *
 * This structure is UI-agnostic. It contains menu items and optional metadata
 * such as icons or voice command aliases for advanced UIs.
 *
 * Example usage:
 *   MenuTemplate mainMenu = MenuTemplate::create({
 *       "Start",
 *       "Settings",
 *       "About"
 *   });
 */
struct MenuTemplate
{
    const char **items; ///< Array of menu item strings
    size_t numItems;    ///< Number of menu items

    const char **icons;     ///< Optional graphical icons (nullptr if unused)
    const char **voiceTags; ///< Optional voice command aliases (nullptr if unused)

    /**
     * @brief Construct a menu template.
     * @param items Array of item strings
     * @param numItems Number of items
     * @param icons Optional array of icons
     * @param voiceTags Optional array of voice tags
     */
    MenuTemplate(const char **items,
                 size_t numItems,
                 const char **icons = nullptr,
                 const char **voiceTags = nullptr)
        : items(items),
          numItems(numItems),
          icons(icons),
          voiceTags(voiceTags)
    {
    }

    /**
     * @brief Factory helper to create a MenuTemplate from initializer list
     */
    static MenuTemplate create(std::initializer_list<const char *> itemList)
    {
        std::vector<const char *> items(itemList); // copy initializer_list to vector
        return MenuTemplate(items.data(), items.size());
    }
};
