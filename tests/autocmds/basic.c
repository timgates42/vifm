#include <stic.h>

#include <stddef.h> /* NULL */

#include "../../src/engine/autocmds.h"

static void handler(const char action[], void *arg);

static const char *action;

TEARDOWN()
{
	action = NULL;
}

TEST(different_path_does_not_match)
{
	assert_success(vle_aucmd_on_execute("cd", "/path", "action", &handler));
	vle_aucmd_execute("cd", "/other", NULL);
	assert_string_equal(NULL, action);
}

TEST(same_path_matches)
{
	assert_success(vle_aucmd_on_execute("cd", "/path", "action", &handler));

	vle_aucmd_execute("cd", "/path", NULL);
	assert_string_equal("action", action);
}

TEST(extra_shashes_match)
{
	assert_success(vle_aucmd_on_execute("cd", "/path/", "action", &handler));

	vle_aucmd_execute("cd", "/path//", NULL);
	assert_string_equal("action", action);
}

TEST(one_level_match)
{
	assert_success(vle_aucmd_on_execute("cd", "/parent/*", "action", &handler));

	vle_aucmd_execute("cd", "/parent/child/grand-child", NULL);
	assert_string_equal(NULL, action);

	vle_aucmd_execute("cd", "/parent/child", NULL);
	assert_string_equal("action", action);
}

TEST(leading_match)
{
	assert_success(vle_aucmd_on_execute("cd", "**/.git", "action", &handler));

	vle_aucmd_execute("cd", "/path/to/repo/.git", NULL);
	assert_string_equal("action", action);
}

TEST(trailing_match)
{
	assert_success(vle_aucmd_on_execute("cd", "**/.git/**", "action", &handler));

	vle_aucmd_execute("cd", "/path/to/repo/.git", NULL);
	assert_string_equal(NULL, action);

	vle_aucmd_execute("cd", "/path/to/repo/.git/objects", NULL);
	assert_string_equal("action", action);
}

TEST(trail_glob_match)
{
	assert_success(vle_aucmd_on_execute("cd", "*.d", "action", &handler));

	vle_aucmd_execute("cd", "/etc/conf.d", NULL);
	assert_string_equal("action", action);
}

static void
handler(const char a[], void *arg)
{
	action = a;
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 filetype=c : */
