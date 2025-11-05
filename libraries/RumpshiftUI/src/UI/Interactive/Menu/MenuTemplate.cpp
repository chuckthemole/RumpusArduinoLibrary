#include "MenuTemplate.h"

// Currently minimal; all logic is in header because this is mostly a POD structure.
// Could extend in future for default scrolling behavior or helper functions.

/**
 * Example of defining reusable menus:
 */
namespace MenuTemplates
{
    // Main menu example
    const char *mainMenuItems[] = {
        "Start",
        "Settings",
        "About"};
    const MenuTemplate MainMenu(mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0]));

    // Settings menu example
    const char *settingsMenuItems[] = {
        "Brightness",
        "Volume",
        "WiFi"};
    const MenuTemplate SettingsMenu(settingsMenuItems, sizeof(settingsMenuItems) / sizeof(settingsMenuItems[0]));

    // Help / Info menu example
    const char *infoMenuItems[] = {
        "Help",
        "Version",
        "Contact"};
    const MenuTemplate InfoMenu(infoMenuItems, sizeof(infoMenuItems) / sizeof(infoMenuItems[0]));
}
