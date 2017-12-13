/* liblouis Braille Translation and Back-Translation Library

 Copyright (C) 2012 James Teh <jamie@nvaccess.org>
 Copyright (C) 2014 Mesar Hameed <mesar.hameed@gmail.com>
 Copyright (C) 2015 Mike Gray <mgray@aph.org>
 Copyright (C) 2010-2017 Swiss Library for the Blind, Visually Impaired and Print Disabled
 Copyright (C) 2016-2017 Davy Kager <mail@davykager.nl>

 Copying and distribution of this file, with or without modification,
 are permitted in any medium without royalty provided the copyright
 notice and this notice are preserved. This file is offered as-is,
 without any warranty.
*/

/* Functionality to check a translation. This is mostly needed for the
 * tests in ../tests but it is also needed for lou_checkyaml. So this
 * functionality is packaged up in what automake calls a convenience
 * library, a lib that is solely built at compile time but never
 * installed.
 */

#include "liblouis.h"

typedef struct {
	const formtype *typeform;
	const int cursorPos;
	const int mode;
	const int direction;
	const int diagnostics;
	const int *expected_inputPos;
	const int *expected_outputPos;
	const int expected_cursorPos;
} optional_test_params;

/** Check a translation
 *
 * Check if an input string is translated as expected.
 *
 * @param tableList comma separated list of tables
 * @param input string to translate
 * @param expected expected output
 * @param typeform (optional) the typeform for this translation. If not specified it
 * defaults to NULL.
 * @param mode (optional) the translation mode. If not specified it defaults to 0.
 * @param expected_inputPos (optional) the expected input positions. If not specified
 * it defaults to NULL.
 * @param expected_outputPos (optional) the expected input positions. If not specified
 * it defaults to NULL.
 * @param cursorPos (optional) the cursor position for this translation. If not specified
 * it defaults to -1.
 * @param expected_cursorPos (optional) the expected cursor position after this
 * translation. If not specified it defaults to -1.
 * @param direction (optional) 0 for forward translation, 1 for backwards translation. If
 * not specified it defaults to 0.
 * @param diagnostics (optional) Print diagnostic output on failure if diagnostics is not
 * 0. If not specified it defaults to 1.
 * @return Return 0 if the translation is as expected and 1 otherwise.
 */
#define check(tables, input, expected, ...)                                      \
	check_base(tables, input, expected, (optional_test_params){.typeform = NULL, \
												.cursorPos = -1,                 \
												.expected_cursorPos = -1,        \
												.expected_inputPos = NULL,       \
												.expected_outputPos = NULL,      \
												.mode = 0,                       \
												.direction = 0,                  \
												.diagnostics = 1,                \
												__VA_ARGS__ })

int
check_base(const char *tableList, const char *input, const char *expected,
		optional_test_params in);

/** Check the cursor positions for a translation
 *
 * For a given input string iterate over all initial cursor positions
 * and check if the returned cursor position equals the one in
 * expected_pos at the same index.
 *
 * Note: This check always translates with compbrlAtCursor and does
 * not check the translation. This would not make sense anyway as the
 * translation changes depending on the initial cursor position. For
 * that reason this function is no longer used in checkyaml.
 *
 * @return Return 0 if the cursor position for each initial position
 * in the input string equals the one in expected_pos at the same
 * index and 1 otherwise.
 * @deprecated use the check function instead
 */
int
check_cursor_pos(const char *tableList, const char *str, const int *expected_pos);

/* Check if a string is hyphenated as expected. Return 0 if the
 * hyphenation is as expected and 1 otherwise. */
int
check_hyphenation(const char *tableList, const char *str, const char *expected);

/* Helper function to convert a typeform string of '0's, '1's, '2's etc.
 * to the required format, which is an array of 0s, 1s, 2s, etc.
 * For example, "0000011111000" is converted to {0,0,0,0,0,1,1,1,1,1,0,0,0}
 * The caller is responsible for freeing the returned array. */
formtype *
convert_typeform(const char *typeform_string);

void
update_typeform(const char *typeform_string, formtype *typeform, typeforms kind);
