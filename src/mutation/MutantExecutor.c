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

#include "MutantExecutor.h"

// Private function interface

static gchar * get_tmp_dir();
static void get_multi_process_mut(Mutant * curr_mut, Mutant * original, GPtrArray * tests, gchar * tmp_results_path);

/* Run mutants using one addtionaly process. */
static void get_multi_process(GPtrArray * mutants, Mutant * original, GPtrArray * tests, MutantExecutionStrategy strategy, gchar * tmp_results_path);

/* For each mutant, run all tests until the mutant is killed. */
static void execution_strategy_mutant_first (GPtrArray * mutants, Mutant * original, GPtrArray * tests);

/* For each test input, run all alive mutants. */
static void execution_strategy_test_first (GPtrArray * mutants, Mutant * original, GPtrArray * tests);

/* For each test input, run all mutants.*/
static void execution_strategy_test_first_full (GPtrArray * mutants, Mutant * original, GPtrArray * tests);

/* For each mutant, run all tests. */
static void execution_strategy_mutant_first_full (GPtrArray * mutants, Mutant * original, GPtrArray * tests);


// Private function implementation

static void execution_strategy_mutant_first_full (GPtrArray * mutants, Mutant * original, GPtrArray * tests)
{
	mutants_results_clean(mutants);
	for (gint i = 0 ; i < mutants->len; i++)
	{
//		g_printf("%d out of %d are complete.\n", i, mutants->len);
		Mutant * curr_mut = g_ptr_array_index(mutants, i);
		for (gint j = 0 ; j < tests->len; j++)
		{
			gchar * curr_test = g_ptr_array_index(tests, j);
			if (mutant_run(curr_mut, original, curr_test))
			{
				mutant_set_killed(curr_mut);
				mutant_results_set_kill(curr_mut);
			}
			else
			{
				mutant_results_set_not_kill(curr_mut);
			}
		}
	}
}

static void execution_strategy_mutant_first (GPtrArray * mutants, Mutant * original, GPtrArray * tests)
{
	mutants_results_clean(mutants);
	for (gint i = 0 ; i < mutants->len; i++)
	{
		//g_printf("mutant first: %d out of %d are complete.\n", i, mutants->len);
		Mutant * curr_mut = g_ptr_array_index(mutants, i);
		for (gint j = 0 ; j < tests->len; j++)
		{
			gchar * curr_test = g_ptr_array_index(tests, j);
			if (mutant_run(curr_mut, original, curr_test))
			{
				mutant_set_killed(curr_mut);
				break;
			}
		}
	}
}

static void execution_strategy_test_first_full (GPtrArray * mutants, Mutant * original, GPtrArray * tests)
{
	mutants_results_clean(mutants);
	for (gint i = 0 ; i < tests->len; i++)
	{
		//g_printf("%d out of %d are complete.\n", i, tests->len);
		gchar * curr_test = g_ptr_array_index(tests, i);
		for (gint j = 0 ; j < mutants->len; j++)
		{
			Mutant * curr_mut = g_ptr_array_index(mutants, j);
			if (mutant_run(curr_mut, original, curr_test))
			{
				mutant_set_killed(curr_mut);
				mutant_results_set_kill(curr_mut);
			}
			else
			{
				mutant_results_set_not_kill(curr_mut);
			}
		}
	}
}

static void execution_strategy_test_first (GPtrArray * mutants, Mutant * original, GPtrArray * tests)
{
	mutants_results_clean(mutants);
	for (gint i = 0 ; i < tests->len; i++)
	{
		//g_printf("test first: %d out of %d are complete.\n", i, tests->len);
		gchar * curr_test = g_ptr_array_index(tests, i);
		for (gint j = 0 ; j < mutants->len; j++)
		{
			Mutant * curr_mut = g_ptr_array_index(mutants, j);
			if (!mutant_is_killed(curr_mut))
			{
				if (mutant_run(curr_mut, original, curr_test))
							{
								mutant_set_killed(curr_mut);
								mutant_results_set_kill(curr_mut);
							}
							else
							{
								mutant_results_set_not_kill(curr_mut);
							}
			}
		}
	}
}


static void get_multi_process(GPtrArray * mutants, Mutant * original, GPtrArray * tests, MutantExecutionStrategy strategy, gchar * tmp_results_path)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot spawn process to execute mutants.") ;
	}
	else if (pid == 0)
	{
		strategy(mutants, original, tests);

		FILE * output = fopen(tmp_results_path,"w");
		mutants_results_save(mutants, output);
		fclose(output);

		exit(0);
	}
}



static void get_multi_process_mut(Mutant * curr_mut, Mutant * original, GPtrArray * tests, gchar * tmp_results_path)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot spawn process to execute mutants.") ;
	}
	else if (pid == 0)
	{
		for (gint j = 0 ; j < tests->len; j++)
		{
			gchar * curr_test = g_ptr_array_index(tests, j);
			if (mutant_run(curr_mut, original, curr_test))
			{
				mutant_set_killed(curr_mut);
				mutant_results_set_kill(curr_mut);
			}
			else
			{
				mutant_results_set_not_kill(curr_mut);
			}
		}

		FILE * output = fopen(tmp_results_path,"w");
		mutant_results_save(curr_mut, output);
		fclose(output);

		exit(0);
	}
}


// Public function implementation

void mutant_executor_run(GPtrArray * mutants, Mutant * original, GPtrArray * tests, StrategyType strategy_type)
{

    MutantExecutionStrategy strategy = NULL;

    switch(strategy_type)
    {
        case StrategyMutantFirst:
        {
            strategy =  &execution_strategy_mutant_first;
            break;
        }
        case StrategyTestFirst:
        {
            strategy =  &execution_strategy_test_first;
            break;
        }
        case StrategyMutantFirstFull:
        {
            strategy =  &execution_strategy_mutant_first_full;
            break;
        }
        case StrategyTestFirstFull:
        {
            strategy =  &execution_strategy_test_first_full;
            break;
        }
        default:
        {
    	g_log ("Milu",G_LOG_LEVEL_WARNING,"Non supporting strategy type.") ;
            strategy =  &execution_strategy_mutant_first;
        	break;
        }
    }

    MultiProcess mp = milu_get_multi_process();

    if(mp == MultiProcessNo)
    {
    	strategy(mutants, original, tests);
    }
    else
    {
    	gint split_num = mutants->len > mp * mp ?  mutants->len/mp : mp;

        GPtrArray * muts = milu_utility_split_gptrarray(mutants,split_num);
        int s;
        for(gint i = 0 ; i < mp; i++)
   		 {
        	gchar * path = g_strdup_printf("%s/p%d.milu", get_tmp_dir (), i);
        	get_multi_process(g_ptr_array_index(muts,i), original, tests, strategy, path);
        	g_free(path);
   		 }
        for(gint i = mp ; i < muts->len; i++)
        {
        	wait(&s);
            gchar * path = g_strdup_printf("%s/p%d.milu", get_tmp_dir (), i);
    	    get_multi_process(g_ptr_array_index(muts,i), original, tests, strategy, path);
            g_free(path);
        }
        for(gint i = 0 ; i < mp; i++)
        {
        	wait(&s);
        }

        for(gint i = 0; i < muts->len; i++)
        {
        	gchar * path = g_strdup_printf("%s/p%d.milu", get_tmp_dir (), i);
        	mutants_results_load(g_ptr_array_index(muts,i), path);
        	g_free(path);
        }

        g_ptr_array_free(muts,TRUE);

    }
    /*
    switch(mp)
    {
    case MultiProcessNo:
    {
    	strategy(mutants, original, tests);
    	break;
    }
    case 80:
    {
    	mutants_results_clean(mutants);

        gchar * path1 = g_strdup_printf("%s/%s", get_tmp_dir (), "p1.milu");
        gchar * path2 = g_strdup_printf("%s/%s", get_tmp_dir (), "p2.milu");
        gchar * path3 = g_strdup_printf("%s/%s", get_tmp_dir (), "p3.milu");
        gchar * path4 = g_strdup_printf("%s/%s", get_tmp_dir (), "p4.milu");
        gchar * path5 = g_strdup_printf("%s/%s", get_tmp_dir (), "p5.milu");
        gchar * path6 = g_strdup_printf("%s/%s", get_tmp_dir (), "p6.milu");
        gchar * path7 = g_strdup_printf("%s/%s", get_tmp_dir (), "p7.milu");
        gchar * path8 = g_strdup_printf("%s/%s", get_tmp_dir (), "p8.milu");

	    get_multi_process_mut(g_ptr_array_index(mutants,0), original, tests, path1);
	    get_multi_process_mut(g_ptr_array_index(mutants,1), original, tests, path2);
	    get_multi_process_mut(g_ptr_array_index(mutants,2), original, tests, path3);
	    get_multi_process_mut(g_ptr_array_index(mutants,3), original, tests, path4);
	    get_multi_process_mut(g_ptr_array_index(mutants,4), original, tests, path5);
	    get_multi_process_mut(g_ptr_array_index(mutants,5), original, tests, path6);
	    get_multi_process_mut(g_ptr_array_index(mutants,6), original, tests, path7);
	    get_multi_process_mut(g_ptr_array_index(mutants,7), original, tests, path8);

    	int s = 0;
    	for (gint i = 8 ; i < mutants->len; i++ )
    	{
    		wait(&s);
    		gchar * pathn = g_strdup_printf("%s/p%d.milu", get_tmp_dir (), i+1);
    		get_multi_process_mut(g_ptr_array_index(mutants,i), original, tests, pathn);
    		g_free(pathn);
    	}
    		wait(&s);
    		wait(&s);
    		wait(&s);
    		wait(&s);
    		wait(&s);
    		wait(&s);
    		wait(&s);

    		gchar * contents;
    		gsize len;

    		for (gint i = 0 ; i < mutants->len ; i ++)
    		{
    			gchar * mut_results_path= g_strdup_printf("%s/p%d.milu", get_tmp_dir (), i+1);
         		if(!g_file_get_contents(mut_results_path, &contents, &len, NULL))
    			{
    				g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot load the mutants' results.") ;
    			}
         		g_assert(contents[len] != '\n');
    			Mutant * curr_mut = g_ptr_array_index(mutants, i);
    			mutant_results_load(curr_mut, contents) ;
    			g_free(contents);
    			g_free(mut_results_path);
    		}


    		    g_free(path1);
    		    g_free(path2);
    		    g_free(path3);
    		    g_free(path4);
    		    g_free(path5);
    		    g_free(path6);
    		    g_free(path7);
    		    g_free(path8);

        break;
    }
    case MultiProcessOne:
    {
    	GPtrArray * cmd = g_string_new("");


        gchar * path1 = g_strdup_printf("%s/%s", get_tmp_dir (), "p1.milu");

    	get_multi_process(mutants, original, tests, strategy, path1);
	    int s = 0;
	    wait(&s);
        mutants_results_load(mutants, path1);

        g_free(path1);
        break;
    }

    case MultiProcessTwo:
    {

        GPtrArray * muts = milu_utility_split_gptrarray(mutants,2);

        gchar * path1 = g_strdup_printf("%s/%s", get_tmp_dir (), "p1.milu");
        gchar * path2 = g_strdup_printf("%s/%s", get_tmp_dir (), "p2.milu");

	    get_multi_process(g_ptr_array_index(muts,0), original, tests, strategy, path1);
	    get_multi_process(g_ptr_array_index(muts,1), original, tests, strategy, path2);

	    int s = 0;
	    wait(&s);
	    wait(&s);

        mutants_results_load(g_ptr_array_index(muts,0), path1);
        mutants_results_load(g_ptr_array_index(muts,1), path2);

        g_free(path1);
        g_free(path2);
        g_ptr_array_free(muts,TRUE);
        break;
    }
    case MultiProcessFour:
    {
        GPtrArray * muts = milu_utility_split_gptrarray(mutants,4);

        gchar * path1 = g_strdup_printf("%s/%s", get_tmp_dir (), "p1.milu");
        gchar * path2 = g_strdup_printf("%s/%s", get_tmp_dir (), "p2.milu");
        gchar * path3 = g_strdup_printf("%s/%s", get_tmp_dir (), "p3.milu");
        gchar * path4 = g_strdup_printf("%s/%s", get_tmp_dir (), "p4.milu");

	    get_multi_process(g_ptr_array_index(muts,0), original, tests, strategy, path1);
	    get_multi_process(g_ptr_array_index(muts,1), original, tests, strategy, path2);
	    get_multi_process(g_ptr_array_index(muts,2), original, tests, strategy, path3);
	    get_multi_process(g_ptr_array_index(muts,3), original, tests, strategy, path4);

	    int s = 0;
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);

        mutants_results_load(g_ptr_array_index(muts,0), path1);
        mutants_results_load(g_ptr_array_index(muts,1), path2);
        mutants_results_load(g_ptr_array_index(muts,2), path3);
        mutants_results_load(g_ptr_array_index(muts,3), path4);

        g_free(path1);
        g_free(path2);
        g_free(path3);
        g_free(path4);

        g_ptr_array_free(muts,TRUE);
        break;


    }
    case MultiProcessSix:
    {
        GPtrArray * muts = milu_utility_split_gptrarray(mutants,6);

        gchar * path1 = g_strdup_printf("%s/%s", get_tmp_dir (), "p1.milu");
        gchar * path2 = g_strdup_printf("%s/%s", get_tmp_dir (), "p2.milu");
        gchar * path3 = g_strdup_printf("%s/%s", get_tmp_dir (), "p3.milu");
        gchar * path4 = g_strdup_printf("%s/%s", get_tmp_dir (), "p4.milu");
        gchar * path5 = g_strdup_printf("%s/%s", get_tmp_dir (), "p5.milu");
        gchar * path6 = g_strdup_printf("%s/%s", get_tmp_dir (), "p6.milu");

	    get_multi_process(g_ptr_array_index(muts,0), original, tests, strategy, path1);
	    get_multi_process(g_ptr_array_index(muts,1), original, tests, strategy, path2);
	    get_multi_process(g_ptr_array_index(muts,2), original, tests, strategy, path3);
	    get_multi_process(g_ptr_array_index(muts,3), original, tests, strategy, path4);
	    get_multi_process(g_ptr_array_index(muts,4), original, tests, strategy, path3);
	    get_multi_process(g_ptr_array_index(muts,5), original, tests, strategy, path4);

	    int s = 0;
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);

        mutants_results_load(g_ptr_array_index(muts,0), path1);
        mutants_results_load(g_ptr_array_index(muts,1), path2);
        mutants_results_load(g_ptr_array_index(muts,2), path3);
        mutants_results_load(g_ptr_array_index(muts,3), path4);
        mutants_results_load(g_ptr_array_index(muts,4), path5);
        mutants_results_load(g_ptr_array_index(muts,5), path6);

        g_free(path1);
        g_free(path2);
        g_free(path3);
        g_free(path4);
        g_free(path5);
        g_free(path6);

        g_ptr_array_free(muts,TRUE);
        break;


    }
    case MultiProcessEight:
    {
        GPtrArray * muts = milu_utility_split_gptrarray(mutants,8);

        gchar * path1 = g_strdup_printf("%s/%s", get_tmp_dir (), "p1.milu");
        gchar * path2 = g_strdup_printf("%s/%s", get_tmp_dir (), "p2.milu");
        gchar * path3 = g_strdup_printf("%s/%s", get_tmp_dir (), "p3.milu");
        gchar * path4 = g_strdup_printf("%s/%s", get_tmp_dir (), "p4.milu");
        gchar * path5 = g_strdup_printf("%s/%s", get_tmp_dir (), "p5.milu");
        gchar * path6 = g_strdup_printf("%s/%s", get_tmp_dir (), "p6.milu");
        gchar * path7 = g_strdup_printf("%s/%s", get_tmp_dir (), "p7.milu");
        gchar * path8 = g_strdup_printf("%s/%s", get_tmp_dir (), "p8.milu");

	    get_multi_process(g_ptr_array_index(muts,0), original, tests, strategy, path1);
	    get_multi_process(g_ptr_array_index(muts,1), original, tests, strategy, path2);
	    get_multi_process(g_ptr_array_index(muts,2), original, tests, strategy, path3);
	    get_multi_process(g_ptr_array_index(muts,3), original, tests, strategy, path4);
	    get_multi_process(g_ptr_array_index(muts,4), original, tests, strategy, path5);
	    get_multi_process(g_ptr_array_index(muts,5), original, tests, strategy, path6);
	    get_multi_process(g_ptr_array_index(muts,6), original, tests, strategy, path7);
	    get_multi_process(g_ptr_array_index(muts,7), original, tests, strategy, path8);

	    int s = 0;
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);
	    wait(&s);

        mutants_results_load(g_ptr_array_index(muts,0), path1);
        mutants_results_load(g_ptr_array_index(muts,1), path2);
        mutants_results_load(g_ptr_array_index(muts,2), path3);
        mutants_results_load(g_ptr_array_index(muts,3), path4);
        mutants_results_load(g_ptr_array_index(muts,4), path5);
        mutants_results_load(g_ptr_array_index(muts,5), path6);
        mutants_results_load(g_ptr_array_index(muts,6), path7);
        mutants_results_load(g_ptr_array_index(muts,7), path8);

        g_free(path1);
        g_free(path2);
        g_free(path3);
        g_free(path4);
        g_free(path5);
        g_free(path6);
        g_free(path7);
        g_free(path8);

        g_ptr_array_free(muts,TRUE);
        break;

    }
    default:
    {
    	g_log ("Milu",G_LOG_LEVEL_WARNING,"Non supporting  process number.") ;
	    strategy(mutants, original, tests);
        break;
    }
    }
    */
}


static gchar * get_tmp_dir()
{
//	return g_get_tmp_dir();
	return "/tmp";
}
