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

#include "Mutant.h"

static void mutants_compile_(GPtrArray * muts, gchar * command, gchar * driver);

extern gchar * mutation_template_check_function(gint);

GPtrArray * mutants_new(GPtrArray * ids, gchar * muts_path)
{
	GPtrArray * mutants = g_ptr_array_new();
	MutationId * curr_mut_id = NULL;

	for ( gint i = 0  ; i< ids->len; i++)
	{
		curr_mut_id = g_ptr_array_index(ids, i);
		g_ptr_array_add(mutants, (gpointer) mutant_new(curr_mut_id,i + 1, muts_path));
	}

	return mutants;
}


void mutants_print(GPtrArray * muts, FILE * output)
{
	Mutant * curr_mut = NULL;
	for(gint i = 0 ; i < muts->len ; i++)
	{
		curr_mut = g_ptr_array_index(muts, i);
		mutant_print(curr_mut, output);
		g_fprintf(output,CR);
	}
}


void mutants_compile(GPtrArray * muts)
{
	Mutant * curr_mut = NULL;
	for(gint i = 0 ; i < muts->len ; i++)
	{
		curr_mut = g_ptr_array_index(muts, i);
		mutant_compile(curr_mut, milu_options_get_compilation_cmd(), milu_options_get_test_driver_path());
	}
}

void mutants_check_equivalence(GPtrArray * muts, Mutant * std)
{
	Mutant * curr_mut = NULL;
	for(gint i = 0 ; i < muts->len ; i++)
	{
		curr_mut = g_ptr_array_index(muts, i);
		mutant_check_equivalence(curr_mut, std);
	}
}

void mutants_check_duplication(GPtrArray * muts)
{
	Mutant * curr_mut = NULL;
	Mutant * next_mut = NULL;
	for(gint i = 0 ; i < muts->len ; i++)
	{
	    curr_mut = g_ptr_array_index(muts, i);
            gint curr_loc = 0;  
            gint curr_i = 0;
            curr_loc = g_array_index(curr_mut->id, gint, curr_i);
            gchar * curr_func = mutation_template_check_function(curr_loc);

            if (!mutant_is_equivalent(curr_mut) && mutant_is_compilable(curr_mut))
            {
	        for(gint j = i+1 ; j < muts->len ; j++)
	        {
	            next_mut = g_ptr_array_index(muts, j);
                    gint next_loc = 0;  
                    gint next_i = 0;
                    next_loc = g_array_index(next_mut->id, gint, next_i);

                    gchar * next_func = mutation_template_check_function(curr_loc);

                    if (curr_func == next_func)
                    {       
                        if(!mutant_is_equivalent(next_mut) && mutant_is_compilable(next_mut) && !mutant_is_duplicated(next_mut))
                        mutant_check_duplication(curr_mut,next_mut); 
                    }

                }
            }

	}
}

static void mutants_compile_(GPtrArray * muts, gchar * command, gchar * driver) // Depreciate
{
	Mutant * curr_mut = NULL;
	for(gint i = 0 ; i < muts->len ; i++)
	{
		curr_mut = g_ptr_array_index(muts, i);
		mutant_compile(curr_mut, command, driver);
	}
}

gint mutants_get_compiled_number(GPtrArray * mutants)
{
    Mutant * curr_mut = NULL;
    gint num_non_eq = 0;

    for (gint i = 0; i < mutants->len; i++)
    {
        curr_mut = g_ptr_array_index(mutants, i);
        if (mutant_is_compilable(curr_mut))
            num_non_eq++;
    }
    return num_non_eq;
}

gint mutants_get_equivalent_number(GPtrArray * mutants)
{
    Mutant * curr_mut = NULL;
    gint num_non_eq = 0;

    for (gint i = 0; i < mutants->len; i++)
    {
        curr_mut = g_ptr_array_index(mutants, i);
        if (mutant_is_equivalent(curr_mut))
            num_non_eq++;
    }
    return num_non_eq;
}

gint mutants_get_duplicated_number(GPtrArray * mutants)
{
    Mutant * curr_mut = NULL;
    gint num_non_eq = 0;

    for (gint i = 0; i < mutants->len; i++)
    {
        curr_mut = g_ptr_array_index(mutants, i);
        if (mutant_is_duplicated(curr_mut))
            num_non_eq++;
    }
    return num_non_eq;
}

gint mutants_get_non_equivalent_number(GPtrArray * mutants)
{
    Mutant * curr_mut = NULL;
    gint num_non_eq = 0;

    for (gint i = 0; i < mutants->len; i++)
    {
        curr_mut = g_ptr_array_index(mutants, i);
        if (!mutant_is_equivalent(curr_mut))
            num_non_eq++;
    }
    return num_non_eq;
}

gint mutants_get_killed_number(GPtrArray * mutants)
{
    Mutant * curr_mut = NULL;
    gint num_killed = 0;

    for (gint i = 0; i < mutants->len; i++)
    {
        curr_mut = g_ptr_array_index(mutants, i);
        if (mutant_is_killed(curr_mut))
            num_killed++;
    }
    return num_killed;
}



gfloat mutants_cal_mutation_score(GPtrArray * mutants)
{
    return (gfloat) mutants_get_killed_number(mutants) / mutants_get_non_equivalent_number(mutants);
}

//This is used for Austin Milu
gint CURR_SAVING_MUT = 0;

void mutants_save(GPtrArray * muts)
{
	Mutant * curr_mut = NULL;
	for(gint i = 0 ; i < muts->len ; i++)
	{
		curr_mut = g_ptr_array_index(muts, i);
        CURR_SAVING_MUT = i ;
		mutant_save(curr_mut);
	}
}


void mutants_results_clean(GPtrArray * mutants)
{
	Mutant * curr_mut = NULL;
	for(gint i = 0 ; i < mutants->len ; i++)
	{
		curr_mut = g_ptr_array_index(mutants, i);
		mutant_results_clean(curr_mut);
	}
}

void mutants_results_save(GPtrArray * muts, FILE * output)
{
	Mutant * curr_mut = NULL;
	for(gint i = 0 ; i < muts->len ; i++)
	{
		curr_mut = g_ptr_array_index(muts, i);
		mutant_results_save(curr_mut, output);
		g_fprintf(output,CR);
	}
}

void mutants_free(GPtrArray * mutants)
{
    //TODO: free
}

void mutants_results_load(GPtrArray * muts, gchar * muts_results_path)
{
	mutants_results_clean(muts);
	gchar * contents;
	gsize len;
	if(!g_file_get_contents(muts_results_path, &contents, &len, NULL))
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot load the mutants' results.") ;
	}
	gchar ** inputs  = g_strsplit (contents, CR, -1);
	gchar ** curr_input = inputs;

	gint i = 0;
	while(*curr_input)
	{
		Mutant * curr_mut = g_ptr_array_index(muts, i);
		mutant_results_load(curr_mut, *curr_input) ;
		curr_input++;
		i++;
	}
	g_strfreev (inputs);
	g_free(contents);

}

void mutants_append_mutants(GPtrArray * mutantsA, GPtrArray * mutantsB)
{
    for(gint i = 0; i < mutantsB->len; i++)
    {
    	Mutant* curr_mut = g_ptr_array_index(mutantsB, i);
        g_ptr_array_add(mutantsA, curr_mut);
    }

    g_ptr_array_free(mutantsB, TRUE);

}

void mutants_check_compiled(GPtrArray * muts)
{
    for(gint i = 0; i < muts->len; i++)
    {
    	Mutant* curr_mut = g_ptr_array_index(muts, i);
        mutant_check_compiled(curr_mut);
    }
}


void mutants_save_id(GPtrArray * muts, FILE * output)
{
    for(gint i = 0; i < muts->len; i++)
    {
    	Mutant* curr_mut = g_ptr_array_index(muts, i);
    	mutation_id_print(curr_mut->id, output);
    	fprintf(output, "%s", CR);
    }
}
