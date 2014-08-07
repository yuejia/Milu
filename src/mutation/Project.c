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

#include "Project.h"

// Public function implementation
extern gboolean MILU_GLOBAL_VERBOSE;
void milu_project_prepare_original_program(Project * project)
{
	g_assert(project != NULL);

    Mutant * original = NULL;

    if (project->original_program == NULL)
    {
	    original = mutant_new_original(project->mutants_path);
    }
    else original = project->original_program;

	mutant_save(original);
	mutant_compile(original, project->compilation_cmd, project->test_driver_path);
    mutant_check_compiled(original);

	if (!mutant_is_compilable(original))
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot compile the original program.") ;
	}

	project->original_program = original;

	g_assert(project->original_program != NULL);
}

Project * milu_project_new(const gchar * source_path, const gchar * output_path, const gchar * test_driver_path, const gchar * tests_path, const gchar * compilation_cmd)
{
	g_assert(source_path && output_path && "Invalid arguments!");

	if (!g_file_test(source_path, G_FILE_TEST_EXISTS))
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot find the source file.") ;
	}

    Project * project = g_slice_new0(Project);
    project->output_path = output_path ? g_string_chunk_insert (MiluStringPool, output_path) : NULL;
    project->source_path = source_path ? g_string_chunk_insert (MiluStringPool, source_path) : NULL;
    project->tests_path = tests_path ? g_string_chunk_insert (MiluStringPool, tests_path) : NULL;
    project->test_driver_path = test_driver_path ? g_string_chunk_insert (MiluStringPool, test_driver_path) : NULL;
    project->compilation_cmd= compilation_cmd? g_string_chunk_insert (MiluStringPool, compilation_cmd) : NULL;


    /* temp loadding test data add for austin unit test(*/
    if(tests_path)
    {
    	if(milu_options_get_unit_test())
    	{
    		project->test_inputs = process_unit_testing_driver(tests_path);
    	}
    	else
        project->test_inputs = test_inputs_load(tests_path);
    }

    project->ast_unit = ASTUnit_new(project->source_path);
    project->mutation_locations = g_ptr_array_new();
    project->mutation_template = g_ptr_array_new();
 //   project->func_nodes = g_ptr_array_new();
 //   g_ptr_array_add(project->func_nodes, project->ast_unit->ast);
    project->func_nodes = ASTUnit_get_current_func_list();

    milu_utility_check_dir(project->output_path);
	GString * cmd = g_string_new("");
    g_string_printf(cmd, "%s/mutants", project->output_path);
    project->mutants_path = g_string_chunk_insert(MiluStringPool, cmd->str);
    milu_utility_check_dir(project->mutants_path);
	g_string_free(cmd, TRUE);

 /*   if(milu_project_is_executable(project))
    {
    	if(milu_options_get_unit_test())
    	{
    		project->test_inputs = process_unit_testing_driver(project->test_driver_path);
    		project->test_driver_path = "milu_driver.c";
    	}
    	else
        project->test_inputs = test_inputs_load(tests_path);
    }
    else
    {
        project->test_inputs = NULL;
    }
    */

    g_assert(project != NULL);
    return project;
}

void milu_project_analyse_template(Project * project)
{
    milu_mutation_analyse(project->func_nodes, project->mutation_operators, &(project->mutation_locations), &(project->mutation_template));
    project->mutation_num_template = mutation_template_num_form(project->mutation_template);
}

void milu_project_free(Project * project)
{
    g_assert(project != NULL);

   if(project->test_inputs)
       test_inputs_free(project->test_inputs);

    if(project->func_nodes)
        g_ptr_array_free(project->func_nodes, TRUE);

    mutation_operators_free(project->mutation_operators);
    mutation_template_location_free(project->mutation_template, project->mutation_locations);

    if(project->mutation_num_template)
        mutation_num_template_free(project->mutation_num_template);

	ASTUnit_free(project->ast_unit);

	g_slice_free(Project, project);
}

void milu_project_print(const Project * project, PrintMode mode, FILE * output)
{
    g_assert(project != NULL);
    g_assert(output != NULL);

    switch(mode)
    {
    	case PrintSource:
    	{
            ASTUnit_print(project->ast_unit,PrintSource,output);
    		break;
    	}
    	case PrintDot:
    	{
            ASTUnit_print(project->ast_unit,PrintDot,output);
            break;
    	}
    	default:
    	{
    		fprintf(output, "Milu Project:\n");
    		fprintf(output, "\tMutation Operators: %d\n", project->mutation_operators->len);
    		fprintf(output, "\tMutation Locations: %d\n", project->mutation_locations->len);
    		break;
    	}
    }
}

gboolean milu_project_is_executable(const Project * project)
{
    g_assert(project != NULL);
    if(project->tests_path &&
    		project->test_driver_path &&
    		project->compilation_cmd)
        return TRUE;
    else
    {

    	if(!project->tests_path)
    		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Cannot find the test file.")  : 0 ;
    	if(!project->test_driver_path)
    		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Cannot find the diver file.")  : 0 ;
    	if(!project->compilation_cmd)
    		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Cannot compilation command.")  : 0 ;

    	if(!project->tests_path)
    		MILU_GLOBAL_VERBOSE ?g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Cannot find the test file.") : 0 ;
    	return FALSE;
    }
}

void milu_project_load_mut_operators_settings (Project * project, const gchar * mut_op_path)
{
    g_assert(project && "Invalid porject!");

    project->mutation_operators = mutation_operators_load(mut_op_path);
}

void milu_project_load_function_settings(Project * project, const gchar * func_path)
{
    g_assert(project && "Invalid porject!");

    if (func_path != NULL)
    {
    //	if(project->func_nodes)
    //		g_ptr_array_free(project->func_nodes, TRUE);


    		GPtrArray * func_text = milu_utility_load_text_file_to_gptrarray(func_path);
    		for(gint i = 0 ; i < project->func_nodes->len; i++)
    		{
    			gboolean found = FALSE;
    			for(gint j = 0 ; j < func_text->len; j++)
    			{
    				gchar * func_name = g_ptr_array_index(func_text, j);
    				ASTNode * tmp_node = g_ptr_array_index(project->func_nodes, i);

    				if(g_strcmp0(func_name, tmp_node->text)==0)
    				{
    					found = TRUE;
    					break;
    				}
    			}
    			if(!found)
    			{
    				g_ptr_array_remove_index(project->func_nodes, i);
    				i--;
    			}
    		}
    }

}

void milu_project_save_mid(Project * project, GPtrArray * mutants)
{
	GString * cmd = g_string_new("");
	g_string_printf(cmd, "%s/mid.txt", project->output_path);
    FILE * findex = fopen(cmd->str,"w");
    for(gint i = 0 ; i < mutants->len ; i ++)
    {
    	Mutant * m = g_ptr_array_index(mutants, i);
    	mutation_id_print(m->id, findex);
    	fprintf(findex, "%s", "\n");
    }
    fclose(findex);	
    g_string_free(cmd, TRUE);
}

void milu_project_save_html_output(Project * project, GPtrArray * mutants)
{
	GString * cmd = g_string_new("");
	g_string_printf(cmd, "%s/html", project->output_path);
	milu_utility_check_dir(cmd->str);

	g_string_printf(cmd, "%s/html/index.html", project->output_path);

	FILE * findex = fopen(cmd->str,"w");
	fprintf(findex, "<html><frameset cols=\"25%%,75%%\">"
			"<frame name=\"mut_list\" src=\"mut_list.html\" />"
			"<frame name=\"mut_code\" src=\"\" /></frameset></html>");
	fclose(findex);


	g_string_printf(cmd, "%s/html/mut_list.html", project->output_path);


	FILE * fmut_list = fopen(cmd->str,"w");

	fprintf(fmut_list, "<html>"
			"<head>"
			"<title>Mutant List</title>"
			"<base target=\"mut_code\">"
			"</head>"
			"<body>");

	// printmut source path
	if(!milu_project_is_executable(project))
		milu_options_set_html_save_all();
	for(gint i = 0 ; i< mutants->len ; i++)
	{
		Mutant * curr_mut = g_ptr_array_index(mutants, i);

		if (milu_options_get_html_save_all() )
		{
			mutant_save_html(curr_mut);
			gchar * mut_name = ASTUnit_get_file_name(ASTUnit_get_current());
			//fprintf(fmut_list, "<a href=\"%s/mut.html\" class=\"mut_code\">Mut %d</a><br />",curr_mut->html_path, i);
			fprintf(fmut_list, "<a href=\"../mutants/%d/html/mut.html\" class=\"mut_code\">Mut %d</a><br />",i+1, i);
		}
		else
		{
		if(!mutant_is_killed(curr_mut))
		{
			mutant_save_html(curr_mut);
			gchar * mut_name = ASTUnit_get_file_name(ASTUnit_get_current());
			//fprintf(fmut_list, "<a href=\"%s/mut.html\" class=\"mut_code\">Mut %d</a><br />",curr_mut->html_path, i);
			fprintf(fmut_list, "<a href=\"../mutants/%d/html/mut.html\" class=\"mut_code\">Mut %d</a><br />",i+1, i);
		}
		}

	}
	fprintf(fmut_list,"</body>" 
			"</html>");
	fclose(fmut_list);
	g_string_free(cmd, TRUE);

}
