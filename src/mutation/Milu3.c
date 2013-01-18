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

// Private function interface


// Public function implementation
GHashTable* std_outputs;
void milu_init()
{
	MiluStringPool =  g_string_chunk_new(MILU_STRING_POOL_SIZE);
	MiluGRand = g_rand_new ();
	ASTNode_init();
	std_outputs = g_hash_table_new(g_str_hash, g_str_equal);
}

void milu_free()
{
	g_rand_free (MiluGRand);
	g_string_chunk_clear (MiluStringPool);
	g_string_chunk_free(MiluStringPool);
	g_hash_table_freeze(std_outputs);
}


GPtrArray * milu_explore_mutants_by_random_cover_fom(const Project * project, gint mutants_num, gint order_begin, gint order_end)
{
	g_assert(project && "Invalid arguments!");
	g_assert(mutants_num > 0 && "Invalid Random limit number!");
	g_assert(order_begin > 0 && order_end > 0 && order_begin <= order_end  && "Invalid order inputs!");

	GPtrArray * mutants = mutation_template_generate_random_mutants_cover_fom(project->mutation_num_template, mutants_num, order_begin, project->mutants_path);

	g_assert(mutants && "Generated 0 mutant!");
	return mutants;
}

GPtrArray * milu_explore_mutants_by_random(const Project * project, gint mutants_num, gint order_begin, gint order_end)
{
	g_assert(project && "Invalid arguments!");
	g_assert(mutants_num > 0 && "Invalid Random limit number!");
	g_assert(order_begin > 0 && order_end > 0 && order_begin <= order_end  && "Invalid order inputs!");

	GPtrArray * mutants = mutation_template_generate_random_mutants(project->mutation_num_template, mutants_num, order_begin, order_end, project->mutants_path);

	g_assert(mutants && "Generated 0 mutant!");
	return mutants;
}


void milu_save_mutants(GPtrArray * mutants)
{
	milu_multi_process_mutants(mutants, &mutants_save);
}

void milu_compile_mutants(GPtrArray * mutants)
{
	milu_multi_process_mutants(mutants, &mutants_compile);
	// mutants_compile(mutants, MILU_GLOBAL_COMPILER, project->test_driver_path);
}

void milu_run_mutation_testing(Project * project, GPtrArray * mutants, StrategyType strategy_type)
{
	g_assert(project && mutants && "Invalid arguments!");
	g_assert(mutants->len > 0 && "Invalid arguments!");

	if (milu_project_is_executable(project))
	{
		milu_project_prepare_original_program(project);
		milu_compile_mutants(mutants);
		mutants_check_compiled(mutants);
		mutant_executor_run(mutants, project->original_program, project->test_inputs, strategy_type);
	}
	else
	{
		g_log ("Milu",G_LOG_LEVEL_WARNING,"To run mutants, please set test inputs path and test driver path.") ;
		exit(0);
	}
}

void milu_print_results(Project * project, GPtrArray * mutants, FILE * output)
{
	g_assert(project && output && mutants && "Invalid arguments!");
	g_assert(mutants->len > 0 && "Invalid arguments!");

	milu_project_print(project, PrintDefault, stdout);
	printf("MS: %d, %f\n", mutants->len, mutants_cal_mutation_score(mutants));
}

void milu_print_killing_results(Project * project, GPtrArray * mutants, FILE * output)
{
	for (gint i = 0 ; i < mutants->len; i ++)
	{
		Mutant * mut = g_ptr_array_index(mutants,i);
		for(gint i = 0 ; i < mut->results->len; i++)
		{
			g_fprintf(output,"%d", g_array_index(mut->results, guint8, i));
		}
			g_fprintf(output,"%s", "\n");
		/*
		if(!mut->iskilled)
			g_fprintf(output,"Mut %d, Compiled: %d, Killed: %d\n",i , mut->compilable, mut->iskilled);
			mutant_print(mut,stdout);
			*/

	}
//	printf("MS: %d, %f\n", mutants->len, mutants_cal_mutation_score(mutants));
}


GPtrArray * milu_restore_mutants_from_mid_file(Project * project, gchar * mid_file)
{
	GPtrArray * mids_text = milu_utility_load_text_file_to_gptrarray(mid_file);
	GPtrArray * mids = g_ptr_array_new();

	for(gint i = 0 ; i < mids_text->len; i++)
	{
		gchar * mid_text = g_ptr_array_index(mids_text, i);
		MutationId * mid = mutation_id_new_from_text(mid_text);
		g_ptr_array_add(mids, mid);
	}

	GPtrArray * muts = mutants_new(mids, project->mutants_path);
	g_ptr_array_free(mids_text, TRUE);
	return muts;
}

GPtrArray * milu_generate_mutants_by_order(Project * project, gint order_begin, gint order_end)
{
	g_assert(project && "Invalid project!");
	g_assert(order_begin > 0 && order_end > 0 && "Invalid order inputs!");

	GPtrArray * mutants = g_ptr_array_new();

	for(gint i = order_begin; i<= order_end; i++)
	{
		GPtrArray * curr_mutants = mutation_template_generate_mutants( project->mutation_num_template,  i, project->mutants_path);
		milu_utility_append_gptrarray(mutants, curr_mutants);
		g_ptr_array_free(curr_mutants, TRUE);
	}

	g_assert(mutants && "Cannot generate mutants!");
	g_assert(mutants->len> 0 && "Return 0 mutant!");

	return mutants;
}

GPtrArray * milu_generate_mutants_by_order_range(Project * project, gint order_begin, gint order_end)
{
	g_assert(project && "Invalid project!");
	g_assert(order_begin > 0 && order_end > 0 && order_end >= order_begin && "Invalid order inputs!");

	GPtrArray * mutants = g_ptr_array_new() ;
	for (gint i = order_begin; i<=order_end; i++)
	{
		GPtrArray * curr_mutants = mutation_template_generate_mutants( project->mutation_num_template,  i, project->mutants_path);
		mutants_append_mutants(mutants, curr_mutants);
	}
	return mutants;
}


GPtrArray * milu_search_hom(const Project * project, gint mutants_num , gint strategy, gint order_begin, gint order_end)
{
	GPtrArray * mutants = milu_generate_mutants_by_order(project, 1, 1);
	milu_save_mutants(mutants);
	if(milu_project_is_executable(project))
	{
		// run mutation testing
		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_INFO,"Analyse First order mutants") : 0;
		milu_run_mutation_testing(project, mutants, 3);
		//	milu_print_results(project, mutants, stdout);
	}


	switch(strategy)
	{
	case 0:
	{
		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_INFO,"Analyse Higher order mutants") : 0;
		GPtrArray * pop = milu_explore_mutants_by_random(project,milu_options_get_rand_limit_number(),order_begin, order_end);
		GPtrArray * res = g_ptr_array_new();
		milu_save_mutants(pop);
		if(milu_project_is_executable(project))
		{
			// run mutation testing
			MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_INFO,"Analyse First order mutants") : 0;
			milu_run_mutation_testing(project, pop, 3);
			milu_print_results(project, pop, stdout);
		}

		for(gint i = 0; i < pop->len; i ++)
		{
			Mutant * mut = g_ptr_array_index(pop, i);
			// 	printf(" %d\n", mut->results->len);
			mutant_evaluate_subsuming_fitness(mut, mutants);
			if (mut->fitness <0.3)
				g_ptr_array_add(res, mut);
		}
		if(milu_project_is_executable(project))
		{
			// run mutation testing
			MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_INFO,"Analyse Higher order mutants") : 0;
			milu_run_mutation_testing(project, res, 3);
			milu_print_results(project, res, stdout);
		}
		exit(0);
		break;
	}
	case 1:
	{

		MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_INFO,"Analyse First order mutants") : 0;
		GPtrArray * pop = milu_explore_mutants_by_random(project, milu_options_get_rand_limit_number(),order_begin, order_end);
		GPtrArray * res = g_ptr_array_new();
		milu_save_mutants(pop);
		if(milu_project_is_executable(project))
		{
			// run mutation testing
			MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_INFO,"Analyse First order mutants") : 0;
			milu_run_mutation_testing(project, pop, 3);
			milu_print_results(project, pop, stdout);
			printf("\n");
			milu_print_killing_results(project, pop, stdout);
		}

		for(gint i = 0; i < pop->len; i ++)
		{
			Mutant * mut = g_ptr_array_index(pop, i);
			// 	printf(" %d\n", mut->results->len);
			mutant_evaluate_strongly_subsuming_fitness(mut, mutants);
	//		g_printf("-----------\n");
			mutant_print(mut,stdout);
	//		g_printf("\n--%f\n", mut->fitness);
			for(gint j = 0 ; j < mut->foms->len; j++)
			{
				Mutant * f = g_ptr_array_index(mut->foms, j);
	//			mutant_print(f,stdout);
	//			g_printf("\n");
			}
			if (mut->fitness == 0.0 && mutant_is_killed(mut))
			{
				g_ptr_array_add(res, mut);
			}
		}
		g_printf("Summery: \n");
		for(gint i = 0 ; i < pop->len; i++)
		{

			Mutant * m = g_ptr_array_index(pop,i);
			g_printf("-----------\n");
						mutant_print(m,stdout);
						g_printf("\n--%f\n", m->fitness);
						for(gint j = 0 ; j < m->foms->len; j++)
						{
							Mutant * f = g_ptr_array_index(m->foms, j);
							mutant_print(f,stdout);
							g_printf("\n");
						}
		}
		if(res->len == 0)
		{
			printf("Didn't find any mutants\n");
			exit(0);
		}
		return res;
		break;
	}
	default:
	{    	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_INFO,"Analyse First order mutants") : 0;
	GPtrArray * pop = milu_explore_mutants_by_random(project,100,order_begin, order_end);
	for(gint i = 0; i < mutants->len; i ++)
	{
		Mutant * mut = g_ptr_array_index(pop, i);
		mutant_evaluate_subsuming_fitness(mut, mutants);
	}
	break;

	}

	}
//	exit(0);
}
