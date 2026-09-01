/*****************************************************************************
 *   Ledger App Aleo.
 *   (c) 2025 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdbool.h>  // bool
#include <string.h>   // memset

#include "os.h"
#include "glyphs.h"
#include "nbgl_layout.h"
#include "nbgl_use_case.h"
#include "os_io_seph_cmd.h"

#include "display.h"
#include "menu.h"

#ifdef HAVE_SE_TOUCH
static void pageCallback(int token, uint8_t index)
{
    (void) token;
    (void) index;
    ui_menu_main();
}
#endif  // HAVE_SE_TOUCH

int ui_display_warning(const char *message)
{
#ifdef HAVE_SE_TOUCH
    nbgl_pageInfoDescription_t info = {0};

#ifdef HAVE_PIEZO_SOUND
    os_io_seph_cmd_piezo_play_tune(TUNE_ERROR);
#endif  // HAVE_PIEZO_SOUND
    info.centeredInfo.icon  = &LARGE_WARNING_ICON;
    info.centeredInfo.style = LARGE_CASE_INFO;
    info.centeredInfo.text1 = message;
    info.tapActionText      = "";
    info.tapActionToken     = 0;
    info.tuneId             = TUNE_TAP_CASUAL;
    nbgl_pageDrawInfo(&pageCallback, NULL, &info);
    nbgl_refreshSpecial(FULL_COLOR_PARTIAL_REFRESH);
#else   // !HAVE_SE_TOUCH
    nbgl_useCaseAction(&LARGE_WARNING_ICON, message, NULL, ui_menu_main);
#endif  // !HAVE_SE_TOUCH

    return 0;
}