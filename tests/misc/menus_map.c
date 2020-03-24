#include <stic.h>

#include <stddef.h> /* NULL */

#include "../../src/engine/keys.h"
#include "../../src/menus/map_menu.h"
#include "../../src/modes/modes.h"
#include "../../src/modes/wk.h"
#include "../../src/ui/statusbar.h"
#include "../../src/ui/ui.h"
#include "../../src/cmd_core.h"

#include "utils.h"

SETUP()
{
	conf_setup();
	init_modes();
	init_commands();

	curr_view = &lwin;
	other_view = &rwin;
	view_setup(&lwin);

	curr_stats.load_stage = -1;
	curr_stats.save_msg = 0;
}

TEARDOWN()
{
	vle_keys_reset();
	conf_teardown();

	view_teardown(&lwin);
	curr_view = NULL;
	other_view = NULL;

	curr_stats.load_stage = 0;
}

TEST(empty_mappings_menu_is_not_displayed)
{
	ui_sb_msg("");
	assert_failure(show_map_menu(&lwin, "normal", NORMAL_MODE, L"nonsense"));
	assert_string_equal("No mappings found", ui_sb_last());

	vle_keys_user_add(L"this", L"that", NORMAL_MODE, KEYS_FLAG_NONE);
	ui_sb_msg("");
	assert_failure(show_map_menu(&lwin, "normal", NORMAL_MODE, L"nonsense"));
	assert_string_equal("No mappings found", ui_sb_last());
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */
