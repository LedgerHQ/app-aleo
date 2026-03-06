#pragma once

#include <stdbool.h>  // bool

#if defined(TARGET_NANOX) || defined(TARGET_NANOS2)
#define ICON_APP_ALEO    C_app_aleo_14px_1bpp
#define ICON_APP_HOME    C_home_aleo_14px_1bpp
#define ICON_APP_WARNING C_icon_warning
#elif defined(TARGET_STAX) || defined(TARGET_FLEX)
#define ICON_APP_ALEO    C_app_aleo_64px_4bpp
#define ICON_APP_HOME    ICON_APP_ALEO
#define ICON_APP_WARNING C_Warning_64px
#elif defined(TARGET_APEX_P)
#define ICON_APP_ALEO    C_app_aleo_48px_1bpp
#define ICON_APP_HOME    ICON_APP_ALEO
#define ICON_APP_WARNING LARGE_WARNING_ICON
#endif

/**
 * Callback to reuse action with approve/reject in step FLOW.
 */
typedef void (*action_validate_cb)(bool);

/**
 * Display address on the device and ask confirmation to export.
 *
 * @return 0 if success, negative integer otherwise.
 *
 */
int ui_display_address(void);

/**
 * Display view key on the device and ask confirmation to export.
 *
 * @return 0 if success, negative integer otherwise.
 *
 */
int ui_display_view_key(void);

/**
 * Display transaction information on the device and ask confirmation to sign.
 *
 * @return 0 if success, negative integer otherwise.
 *
 */
int ui_display_transaction(void);
