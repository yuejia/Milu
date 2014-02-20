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

#include "MutationTemplate.h"

static GPtrArray * curr_mutation_operators  = NULL;
static GPtrArray * curr_mutation_locations =  NULL;
static GPtrArray * curr_mutation_template  = NULL;
static MutationNumTemplate * curr_mutation_num_template  = NULL;


// Private function interface

static gboolean mutation_template_check_mutation(ASTNode * node, gpointer data);


// Public function implementation

MutationTemplate * mutation_template_get_current()
{
	return curr_mutation_template;
}

void milu_mutation_analyse(GPtrArray * functions, GPtrArray * mut_operators, GPtrArray **  mut_locations, GPtrArray ** mut_template)
{
	 g_assert(functions && mut_operators &&  mut_locations && mut_template && "Invalid arguments!");
	 g_assert(functions->len > 0 && mut_operators->len > 0 && "Invalid arguments!");

	curr_mutation_operators = mut_operators;
	curr_mutation_locations = g_ptr_array_new();
	curr_mutation_template = g_ptr_array_new();

	*mut_locations = curr_mutation_locations;
	*mut_template = curr_mutation_template;

    for (gint i = 0 ; i < functions->len; i++)
    {
    	ASTNode * curr_func = g_ptr_array_index(functions, i);
	    parse_tree_node_traverse_pre_order (curr_func, &mutation_template_check_mutation, NULL);
    }
}

GArray * mutation_template_num_form(GPtrArray * mut_template)
{
	GArray * num_template = g_array_new (FALSE, FALSE, sizeof (gint));
	GPtrArray * curr_mutators = NULL;
	Mutator * curr_mut = NULL;
	gint curr_size;
	for (int i = 0; i < mut_template->len; i++)
	{
		curr_size = 0;
		curr_mutators =  g_ptr_array_index(mut_template, i);

		for (int j = 0; j < curr_mutators->len; j++)
		{
			curr_mut = g_ptr_array_index(curr_mutators, j);
			curr_size += curr_mut->size;
		}
	    g_array_append_val (num_template, curr_size);
	}

	curr_mutation_num_template = num_template;
	return num_template;
}

void mutation_template_num_form_print(GArray * mut_num_template, FILE * output)
{
	  for (int i = 0; i < mut_num_template->len; i++)
		  g_fprintf(output,"%d ", g_array_index (mut_num_template, gint, i) );
}

GPtrArray * mutation_template_generate_random_mutants(GArray * num_template, gint mutants_num, gint order_begin, gint order_end, gchar * muts_path)
{

    g_assert(num_template && muts_path && mutants_num > 0 &&"Invalid arguments!");
    g_assert(order_begin > 0 && order_end > 0 && order_begin <= order_end && "Invalid order inputs!");

	GPtrArray * mids =  mutant_ids_get_random_nth_order(num_template, mutants_num, order_begin, order_end);
	GPtrArray * mutants = mutants_new(mids, muts_path);

    return mutants;
}

GPtrArray * mutation_template_generate_random_mutants_cover_fom(GArray * num_template, gint mutants_num, gint order, gchar * muts_path)
{

    g_assert(num_template && muts_path && mutants_num > 0 &&"Invalid arguments!");
    g_assert(order > 0 && "Invalid order inputs!");

	GPtrArray * mids = mutant_ids_get_random_nth_order_cover_fom(num_template, mutants_num, order);
	GPtrArray * mutants = mutants_new(mids, muts_path);

    return mutants;
}

GPtrArray * mutation_template_generate_mutants(GArray * num_template, gint order, gchar * muts_path)
{
	g_assert(num_template && muts_path && "Invalid arguments!");
	g_assert(order > 0 && "Order should be greater than 1") ;

    GPtrArray * mcids = mutation_compact_ids_get_nth_order(num_template, order);
    GPtrArray * mids = mutation_compact_ids_expend(mcids);


    //deprecated  implementation
    /*
	GPtrArray * fom_ids = mutation_ids_get_1st_order(num_template);
	GPtrArray * curr_mids = fom_ids;

	for (gint i = 2; i <= order; i++ )
	{
	     GPtrArray * prev_mids = curr_mids;
         curr_mids = mutation_ids_get_next_order(prev_mids, fom_ids);
         if (prev_mids != fom_ids)
         mutation_ids_free(prev_mids);
	}
    */

    mutation_ids_free(mcids);
	GPtrArray * mutants = mutants_new(mids, muts_path);
	return mutants;
}


void mutation_template_clean_mutation(gint loc, gint id)
{
	GPtrArray * curr_mutators = g_ptr_array_index(curr_mutation_template, loc);
	ASTNode * curr_node = g_ptr_array_index(curr_mutation_locations, loc);
	curr_node->is_mutant = FALSE;
	gint curr_size = 0;

		for (int j = 0; j < curr_mutators->len; j++)
		{
			Mutator * curr_mut = g_ptr_array_index(curr_mutators, j);
			curr_size += curr_mut->size;
			if (curr_size >= id)
			{
				gint curr_mut_id = curr_mut->size - (curr_size - id );
				curr_mut->clean(curr_node, curr_mut_id);
				//testing
		//	g_printf("end: %p, %d\n", curr_node, curr_mut_id);

			// bug fix Yue: October 27, 2012
			break;
			}
		}
}

gint mutation_template_apply_mutation(gint loc, gint id)
{
	GPtrArray * curr_mutators = g_ptr_array_index(curr_mutation_template, loc);
	ASTNode * curr_node = g_ptr_array_index(curr_mutation_locations, loc);
    curr_node->is_mutant = TRUE;

	gint curr_size = 0;

	for (int j = 0; j < curr_mutators->len; j++)
	{
		Mutator * curr_mut = g_ptr_array_index(curr_mutators, j);
		curr_size += curr_mut->size;
		if (curr_size >= id)
		{
			gint curr_mut_id = curr_mut->size - (curr_size - id );
			curr_mut->mutate(curr_node, curr_mut_id);
			//testing:
		//	g_printf("start: %p, %d\n", curr_node, curr_mut_id);

			break; // bug fix Yue: October 27, 2012
		}
	}
        return curr_node->line_start; 
}


void mutation_template_location_free(GPtrArray * mut_template, GPtrArray * mut_locations)
{
	g_ptr_array_free(mut_locations, TRUE);

	for(gint i = 0 ; i < mut_template->len; i++)
	{
		GPtrArray * gp = g_ptr_array_index(mut_template, i);
        g_ptr_array_free(gp, TRUE);
	}
    g_ptr_array_free(mut_template, TRUE);
}

void mutation_num_template_free(MutationNumTemplate * mut_num_template)
{
    g_assert(mut_num_template);

    g_array_free(mut_num_template, TRUE);
}

// Private function implementation

static gboolean mutation_template_check_mutation(ASTNode * node, gpointer data)
{

	GPtrArray * curr_mut_slot = g_ptr_array_new();

	for (int i = 0; i < curr_mutation_operators->len; i++)
	{
		Operator * curr_opt = g_ptr_array_index(curr_mutation_operators, i);

		for (int j = 0; j< curr_opt->mutators->len; j++)
		{
			Mutator * curr_mut = g_ptr_array_index(curr_opt->mutators, j);

			if (curr_mut->node_checking(node))
			{
				g_ptr_array_add(curr_mut_slot, (gpointer) curr_mut);
			}
		}
	}

	if (curr_mut_slot->len > 0)
	{
		g_ptr_array_add(curr_mutation_template, (gpointer) curr_mut_slot);
		g_ptr_array_add(curr_mutation_locations, (gpointer) node);
	}

	return FALSE;
}
