#include <string.h>

#include "seatest.h"

#include "../../src/cmds.h"
#include "../../src/commands.h"
#include "../../src/ui.h"

extern struct cmds_conf cmds_conf;

static void
setup_lwin(void)
{
	strcpy(lwin.curr_dir, "/lwin");

	lwin.list_rows = 4;
	lwin.list_pos = 2;
	lwin.dir_entry = calloc(lwin.list_rows, sizeof(*lwin.dir_entry));
	lwin.dir_entry[0].name = strdup("lfile0");
	lwin.dir_entry[1].name = strdup("lfile1");
	lwin.dir_entry[2].name = strdup("lfile2");
	lwin.dir_entry[3].name = strdup("lfile3");

	lwin.dir_entry[0].selected = 1;
	lwin.dir_entry[2].selected = 1;
	lwin.selected_files = 2;
}

static void
setup_rwin(void)
{
	strcpy(rwin.curr_dir, "/rwin");

	rwin.list_rows = 6;
	rwin.list_pos = 5;
	rwin.dir_entry = calloc(rwin.list_rows, sizeof(*rwin.dir_entry));
	rwin.dir_entry[0].name = strdup("rfile0");
	rwin.dir_entry[1].name = strdup("rfile1");
	rwin.dir_entry[2].name = strdup("rfile2");
	rwin.dir_entry[3].name = strdup("rfile3");
	rwin.dir_entry[4].name = strdup("rfile4");
	rwin.dir_entry[5].name = strdup("rfile5");

	rwin.dir_entry[1].selected = 1;
	rwin.dir_entry[3].selected = 1;
	rwin.dir_entry[5].selected = 1;
	rwin.selected_files = 3;
}

static void
setup(void)
{
	setup_lwin();
	setup_rwin();
}

static void
teardown(void)
{
	int i;

	for(i = 0; i < lwin.list_rows; i++)
		free(lwin.dir_entry[i].name);
	free(lwin.dir_entry);

	for(i = 0; i < rwin.list_rows; i++)
		free(rwin.dir_entry[i].name);
	free(rwin.dir_entry);
}

static void
one_number_range_test(void)
{
	struct cmd_info cmd_info = {
		.begin = -1, .end = -1
	};

	curr_view = &lwin;
	select_range(&cmd_info);
	assert_int_equal(2, lwin.selected_files);

	curr_view = &rwin;
	select_range(&cmd_info);
	assert_int_equal(3, rwin.selected_files);
}

void
one_number_range(void)
{
	test_fixture_start();

	fixture_setup(setup);
	fixture_teardown(teardown);

	run_test(one_number_range_test);

	test_fixture_end();
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab : */
