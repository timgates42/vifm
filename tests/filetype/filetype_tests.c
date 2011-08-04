#include <stdlib.h>

#include "seatest.h"

#include "../../src/filetype.h"
#include "../../src/status.h"

static void
test_one_ext(void)
{
	char *buf;

	set_programs("*.tar", "tar prog", 0);

	buf = get_default_program_for_file("file.version.tar");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("tar prog", buf);
	free(buf);
}

static void
test_many_ext(void)
{
	char *buf;

	set_programs("*.tar", "tar prog", 0);
	set_programs("*.tar.gz", "tar.gz prog", 0);

	buf = get_default_program_for_file("file.version.tar.gz");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("tar.gz prog", buf);
	free(buf);
}

static void
test_many_fileext(void)
{
	char *buf;

	set_programs("*.tgz,*.tar.gz", "tar.gz prog", 0);

	buf = get_default_program_for_file("file.version.tar.gz");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("tar.gz prog", buf);
	free(buf);
}

static void
test_dont_match_hidden(void)
{
	char *buf;

	set_programs("*.tgz,*.tar.gz", "tar.gz prog", 0);

	buf = get_default_program_for_file(".file.version.tar.gz");
	assert_true(buf == NULL);
}

static void
test_match_empty(void)
{
	char *buf;

	set_programs("a*bc", "empty prog", 0);

	buf = get_default_program_for_file("abc");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("empty prog", buf);
	free(buf);
}

static void
test_match_full_line(void)
{
	char *buf;

	set_programs("abc", "full prog", 0);

	buf = get_default_program_for_file("abcd");
	assert_true(buf == NULL);

	buf = get_default_program_for_file("0abc");
	assert_true(buf == NULL);

	buf = get_default_program_for_file("0abcd");
	assert_true(buf == NULL);

	buf = get_default_program_for_file("abc");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("full prog", buf);
	free(buf);
}

static void
test_match_qmark(void)
{
	char *buf;

	set_programs("a?c", "full prog", 0);

	buf = get_default_program_for_file("ac");
	assert_true(buf == NULL);

	buf = get_default_program_for_file("abc");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("full prog", buf);
	free(buf);
}

static void
test_escaping(void)
{
	char *buf;

	set_programs("a\\?c", "qmark prog", 0);

	buf = get_default_program_for_file("abc");
	assert_true(buf == NULL);

	buf = get_default_program_for_file("a?c");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("qmark prog", buf);
	free(buf);

	set_programs("a\\*c", "star prog", 0);

	buf = get_default_program_for_file("abc");
	assert_true(buf == NULL);

	buf = get_default_program_for_file("a*c");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("star prog", buf);
	free(buf);
}

static void
test_xfiletypes1_c(void)
{
	char *buf;

	curr_stats.is_console = 1;
	set_programs("*.tar", "x prog", 1);
	set_programs("*.tar", "console prog", 0);

	buf = get_default_program_for_file("file.version.tar");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("console prog", buf);
	free(buf);
}

static void
test_xfiletypes1_x(void)
{
	char *buf;

	curr_stats.is_console = 0;
	set_programs("*.tar", "x prog", 1);
	set_programs("*.tar", "console prog", 0);

	buf = get_default_program_for_file("file.version.tar");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("x prog", buf);
	free(buf);
}

static void
test_xfiletypes2_c(void)
{
	char *buf;

	curr_stats.is_console = 1;
	set_programs("*.tgz", "2 x prog", 1);

	buf = get_default_program_for_file("file.version.tgz");
	assert_true(buf == NULL);
}

static void
test_xfiletypes2_x(void)
{
	char *buf;

	curr_stats.is_console = 0;
	set_programs("*.tgz", "2 x prog", 1);

	buf = get_default_program_for_file("file.version.tgz");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("2 x prog", buf);
	free(buf);
}

static void
test_xfiletypes3_c(void)
{
	char *buf;

	curr_stats.is_console = 0;
	set_programs("*.tar.bz2", "3 console prog", 0);

	buf = get_default_program_for_file("file.version.tar.bz2");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("3 console prog", buf);
	free(buf);
}

static void
test_xfiletypes3_x(void)
{
	char *buf;

	curr_stats.is_console = 1;
	set_programs("*.tar.bz2", "3 console prog", 0);

	buf = get_default_program_for_file("file.version.tar.bz2");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("3 console prog", buf);
	free(buf);
}


static void
test_removing(void)
{
	char *buf;

	curr_stats.is_console = 1;
	set_programs("*.tar.bz2", "3 console prog", 0);
	set_programs("*.tgz", "3 console prog", 0);

	buf = get_default_program_for_file("file.version.tar.bz2");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("3 console prog", buf);
	free(buf);

	buf = get_default_program_for_file("file.version.tgz");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("3 console prog", buf);
	free(buf);

	remove_filetypes("*.tar.bz2");

	buf = get_default_program_for_file("file.version.tar.bz2");
	assert_true(buf == NULL);

	buf = get_default_program_for_file("file.version.tgz");
	assert_false(buf == NULL);
	if(buf != NULL)
		assert_string_equal("3 console prog", buf);
	free(buf);
}

void
filetype_tests(void)
{
	test_fixture_start();

	run_test(test_one_ext);
	run_test(test_many_ext);
	run_test(test_many_fileext);
	run_test(test_dont_match_hidden);
	run_test(test_match_empty);
	run_test(test_match_full_line);
	run_test(test_match_qmark);
	run_test(test_escaping);

	run_test(test_xfiletypes1_c);
	run_test(test_xfiletypes1_x);
	run_test(test_xfiletypes2_c);
	run_test(test_xfiletypes2_x);
	run_test(test_xfiletypes3_c);
	run_test(test_xfiletypes3_x);

	run_test(test_removing);

	test_fixture_end();
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab : */
