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

#include "Milu3.h"
#include "Utility.h"
#include "ASTPrinter.h"
#include "ASTNode.h"
#include <glib.h>

int main(int argc, char *argv[ ] ) {


	milu_init();
	milu_options_parse(argc, argv);

	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Higher Order Mutation Testing System v3.0") : 0;

	milu_options_get_tag_list_path();
    if (argc < 1 || argv[1] == NULL)
    {
        g_log ("Milu",G_LOG_LEVEL_ERROR,"Usage: milu source_path ");
    }

	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Initialize project") : 0;

    gchar * source_path = argv[1];
    milu_options_set_src_path(source_path);
    Project * project = milu_project_new(source_path,
    		milu_options_get_output_path(),
    		milu_options_get_test_driver_path(),
    		milu_options_get_test_input_path(),
    		milu_options_get_compilation_cmd());


  //  process_unit_testing_driver(milu_options_get_test_driver_path());

   /// exit(1);
    gchar * milu_debug = milu_options_get_debug_info();
	if(milu_debug)
	{
		if ( g_strcmp0(milu_debug, "src") == 0)
            milu_project_print(project, PrintSource, stdout);
		else if ( g_strcmp0(milu_debug, "dot") == 0)
            milu_project_print(project, PrintDot, stdout);
		else
            g_log ("Milu",G_LOG_LEVEL_ERROR,"Please use --debug=src or --debug=dot!");
	}

	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Load function") : 0;
	milu_project_load_function_settings(project, milu_options_get_func_list_path());

	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Load operators") : 0;
	milu_project_load_mut_operators_settings(project, milu_options_get_mut_operators_path());


	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Analyse Temple") : 0;
    milu_project_analyse_template(project);

    /*
     * comment for milu austin
     * */

	GPtrArray * mutants = NULL;

	// generate mutants
	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Generate Mutants") : 0;

	if(g_strcmp0(milu_options_get_mut_strategy() , "rand") == 0)
	{
		//rand mode
		mutants = milu_explore_mutants_by_random(project, milu_options_get_rand_limit_number(),  milu_options_get_order_start() ,  milu_options_get_order_end() );
		milu_save_mutants(mutants);
	}
	else if(g_strcmp0(milu_options_get_mut_strategy() , "rand-cov") == 0)
	{

		mutants = milu_explore_mutants_by_random_cover_fom(project, milu_options_get_rand_limit_number(),  milu_options_get_order_start() ,  milu_options_get_order_end() );
		milu_save_mutants(mutants);
	}
	else if(milu_options_get_search_strategy())
	{
		mutants = milu_search_hom(project, milu_options_get_rand_limit_number() , 1,  milu_options_get_order_start() , milu_options_get_order_end());
		milu_save_mutants(mutants);

	}
	else if(g_strcmp0(milu_options_get_mut_strategy() , "default") == 0)
	{
		// default mode
		 mutants = milu_generate_mutants_by_order(project, milu_options_get_order_start() , milu_options_get_order_end() );
		 if(!milu_options_not_save_mutants())
		 {
			 milu_save_mutants(mutants);
		 }
	}
	else if(g_strcmp0(milu_options_get_mut_strategy() , "restore") == 0)
	{
		mutants = milu_restore_mutants_from_mid_file(project, milu_options_get_mid_file_path());
		milu_save_mutants(mutants);
	}
	else
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Unknown mutant generation strategy!");
	}
		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Generated %d mutants.",mutants->len) : 0 ;

	if(milu_project_is_executable(project) &&  !milu_options_not_save_mutants())
	{
		// run mutation testing
		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Run Mutants") : 0;
		milu_run_mutation_testing(project, mutants, milu_options_get_exec_strategy());
		milu_print_results(project, mutants, stdout);
		if(milu_options_print_killing_result())
			milu_print_killing_results(project, mutants, stdout);

		if(milu_options_save_killing_result())
		{
			GString * cmd = g_string_new("");
 		    g_string_printf(cmd, "%s/results.txt", project->output_path);
			FILE * f = fopen(cmd->str,"w");
			milu_print_killing_results(project, mutants, f);
			fclose(f);
			g_string_free(cmd, TRUE);
		}
	}
	else if(project->compilation_cmd)
	{
		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Found Compiler settings, try to compile mutants!") : 0;
		milu_project_prepare_original_program(project);
		milu_compile_mutants(mutants);
                if (MILU_TCE_OPTIMISATION)
                {
		    MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Apply TCE Optimisation") : 0;
		    milu_check_equivalent_mutants(mutants, project->original_program);
                    printf("Generated %d mutants%s", mutants->len, CR);
                    printf("Number of compilable mutants: %d%s", mutants_get_compiled_number(mutants),CR);
                    printf("Number of equivalent mutants: %d", mutants_get_equivalent_number(mutants),CR);
		    milu_check_duplicated_mutants(mutants);
                    printf("Number of duplicated mutants: %d", mutants_get_duplicated_number(mutants),CR);
                }
	}


    if(milu_options_get_html_view() &&  !milu_options_not_save_mutants())
    {
	// generate html files for mutants
	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Generate html viewer for mutants") : 0;
        milu_project_save_html_output(project, mutants);
    }

    if(milu_options_get_save_mid())
    {
        milu_project_save_mid(project, mutants);
    }

	mutants_free(mutants);


	milu_project_free(project);
	milu_options_free();
	milu_free();

	return EXIT_SUCCESS;
}

