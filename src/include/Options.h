/*
Milu v3.0 - A Higher Order Mutation Testing Tool

Copyright (c) 2011, Yue Jia
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the University College London nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MENU_H_
#define MENU_H_

#include <glib.h>
#include <stdlib.h>
#include "MultiProcessing.h"
#include "Utility.h"


/**@file
 * Implement menu options.
 */

/**@addtogroup opt_arg Options and Arguments
 * @{
 */

/**
 * \brief Set verbose output.
 */
extern gboolean MILU_GLOBAL_VERBOSE;

/**
 * \brief Set TCE optimisation.
 */
extern gboolean MILU_TCE_OPTIMISATION;



/**
 * \brief Handle options from Milu input.
 *
 * \param argc The number of input arguments.
 * \param argv The input arguments array.
 **/
void milu_options_parse(int argc, char *argv[ ]);

/**
 * \brief Free options memory.
 **/
void milu_options_free();

/**
 * \brief Return the test input path.
 **/
gchar * milu_options_get_test_input_path();

/**
 * \brief Return the test driver path.
 **/
gchar * milu_options_get_test_driver_path();

/**
 * \brief Return the milu ouput path.
 **/
gchar * milu_options_get_output_path();

/**
 * \brief Return the debug flag.
 **/
gchar * milu_options_get_debug_info();

/**
 * \brief Return the mutant generation strategy.
 **/
gchar * milu_options_get_mut_strategy();

/**
 * \brief Return the search strategy.
 **/
gchar * milu_options_get_search_strategy();

/**
 * \brief Return the mutant execution strategy.
 **/
 gint milu_options_get_exec_strategy();

/**
 * \brief Return the order for mutant generation.
 **/
gint  milu_options_get_mut_order();

/**
 * \brief Return the random limit number for mutant generation.
 **/
gint  milu_options_get_rand_limit_number();

/**
 * \brief Return the path of selected function list.
 **/
gchar * milu_options_get_func_list_path();

/**
 * \brief Return name of the selected function.
 **/
gchar * milu_options_get_func_name();

/**
 * \brief Return the path of selected mutation operator.
 **/
gchar * milu_options_get_mut_operators_path();

/**
 * \brief Return the begin order for mutant generation.
 **/
gint milu_options_get_order_start();


/**
 * \brief Return the end order for mutant generation.
 **/
gint milu_options_get_order_end();

/**
 * \brief Return command to compile mutants.
 **/
gchar * milu_options_get_compilation_cmd();


/**
 * \brief Return setting for generation of html viewer.
 **/
gboolean milu_options_get_html_view();

/**
 * \brief Return setting for unit test.
 **/
gboolean milu_options_get_unit_test();

/**
 * \brief Set the driver path.
 * \driver the driver path.
 **/
void milu_options_set_test_driver_path(gchar * driver);

/**
 * \brief Return TRUE if save mid file.
 **/
gboolean milu_options_get_save_mid();

/**
 * \brief Return path to mid file.
 **/
gchar * milu_options_get_mid_file_path();

/**
 * \brief Return flag to print milu killing results.
 **/
gboolean milu_options_print_killing_result();

/**
 * \brief Return flag to print all html mutants.
 **/
gboolean milu_options_get_html_save_all();

/**
 * \brief Return flag if save mutants.
 **/
gboolean milu_options_not_save_mutants();

/**
 * \brief Return flag if save mutant killing results.
 **/
gboolean milu_options_save_killing_result();

/**
 * \brief Return the path of selected tag list.
 **/
gchar * milu_options_get_tag_list_path();

/**
 * \brief Return the path of src path.
 **/
void milu_options_set_src_path(gchar * path);

/**
 * \brief Return the path of src path.
 **/
gchar * milu_options_get_src_path();

/**
 * \brief Set true for generate html viewer for all mutants.
 **/

void milu_options_set_html_save_all();

#endif /* MENU_H_ */

/**@}*/
