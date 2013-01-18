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


extern GHashTable* std_outputs; //hash table for original program outpt
guint8 MUTANT_RESULT_KILL= 1;
guint8 MUTANT_RESULT_NOT_KILL= 0;

static void apply_mutation(Mutant * mut);
static void clean_mutation(Mutant * mut);

static void apply_mutation(Mutant * mut)
{
	gint curr_id = 0;
	gint curr_loc = 0;
	for (gint i = 0; i < mut->id->len; i = i + 2)
	{
		curr_loc = g_array_index(mut->id, gint, i);
		curr_id = g_array_index(mut->id, gint, i + 1);
		mutation_template_apply_mutation(curr_loc, curr_id);
	}
}

static void clean_mutation(Mutant * mut)
{
	gint curr_id = 0;
	gint curr_loc = 0;
	for (gint i = 0; i < mut->id->len; i = i + 2)
	{
		curr_loc = g_array_index(mut->id, gint, i);
		curr_id = g_array_index(mut->id, gint, i + 1);
		mutation_template_clean_mutation(curr_loc, curr_id);
	}
}

Mutant * mutant_new(MutationId * id, gint * file_id, gchar * base_path)
{
	Mutant * mut = g_slice_new(Mutant);
	mut->id = id;
	mut->file_id = file_id;
	mut->base_path = g_strdup_printf("%s/%d", base_path, file_id);
//	Yue removed: unnessary to create folder here
//	mutant_check_dir(mut->base_path);

	mut->src_path = g_strdup_printf("%s/src", mut->base_path);
	mut->bin_path = g_strdup_printf("%s/bin", mut->base_path);
	mut->html_path = g_strdup_printf("%s/html", mut->base_path);

	mut->output_path = g_strdup_printf("%s/output", mut->base_path);
    mut->iskilled = FALSE;
    mut->compilable = TRUE;
    mut->isequivalent = FALSE;

    mut->results = NULL;//g_array_new(FALSE, FALSE, sizeof (guint8));
    mut->foms = g_ptr_array_new();

	return mut;
}

void mutant_results_clean(Mutant * mut)
{
	if (mut->results == NULL)
	{
		mut->results = g_array_new(FALSE, FALSE, sizeof (guint8));
	}
	else
	{
		if (mut->results->len > 0)
		{
			// TODO free previous results;
			mut->results = g_array_new(FALSE, FALSE, sizeof (guint8));
		}
	}
}

void mutant_results_set_kill(Mutant * mut)
{
    g_array_append_val(mut->results, MUTANT_RESULT_KILL);
}

void mutant_results_set_not_kill(Mutant * mut)
{
    g_array_append_val(mut->results, MUTANT_RESULT_NOT_KILL);
}


void mutant_save(Mutant * mut)
{
	mut == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"mutation save error.") : 0;

	gchar * mut_name = ASTUnit_get_file_name(ASTUnit_get_current());

	GString * cmd = g_string_new("");
	mutant_check_dir(mut->base_path);
	mutant_check_dir(mut->src_path);

	g_string_printf(cmd,"%s/%s",mut->src_path, mut_name);
	FILE * mfile = fopen(cmd->str,"w");
	mfile == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"cannot write mutant %s", cmd->str) : 0;

	apply_mutation(mut);
	ASTUnit_print(ASTUnit_get_current(), PrintSource,mfile);
	clean_mutation(mut);

	fclose(mfile);
	g_string_free(cmd, TRUE);
}

void mutant_save_html(Mutant * mut)
{
	mut == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"mutation save error.") : 0;

	gchar * mut_name = ASTUnit_get_file_name(ASTUnit_get_current());

	GString * cmd = g_string_new("");

	mutant_check_dir(mut->html_path);

	g_string_printf(cmd,"%s/%s",mut->html_path, "mut.html");
	FILE * mfile = fopen(cmd->str,"w");
	mfile == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"cannot write mutant %s", cmd->str) : 0;

	apply_mutation(mut);
	ASTUnit_print(ASTUnit_get_current(), PrintHtml,mfile);
	clean_mutation(mut);

	fclose(mfile);
	g_string_free(cmd, TRUE);
}

//void mutant_run(gchar * bin);


gchar * mutant_get_src_path(gchar * mut_path)
{
	GString * cmd = g_string_new("");
	g_string_printf(cmd,"%s/src",mut_path);
	gchar * src_path = g_string_free(cmd, FALSE);
	return src_path;

	// need to be free
}
void  mutant_check_dir(gchar * path)
{
	GString * cmd = g_string_new("");
    if (g_file_test(path, G_FILE_TEST_EXISTS))
    {
	 if (g_file_test (path, G_FILE_TEST_IS_DIR))
	 {
		 g_string_printf(cmd,"rm -r %s",path);
	     system(cmd->str);
	 }
	 else
	 {
		 g_log ("Milu",G_LOG_LEVEL_WARNING,"checking the wrong dir") ;
	 }
    }

	g_string_printf(cmd,"mkdir %s",path);

	system(cmd->str);
	g_string_free(cmd, TRUE);
}

void mutant_print(Mutant * mut, FILE * output)
{
	mutation_id_print(mut->id, output);
	g_fprintf(output,"Compiled: %d, Killed: %d\n", mut->compilable, mut->iskilled);

	if ( mutant_has_results(mut))
	{
		g_fprintf(output,"Results: ");
		for(gint i = 0 ; i < mut->results->len; i++)
		{
			g_fprintf(output,"%d ", g_array_index(mut->results, guint8, i));
		}
	}
}


void mutant_compile(Mutant * mut, gchar * command, gchar * driver)
{

	mut == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"mutant compilation error") : 0;
	gchar * mut_name = ASTUnit_get_file_name(ASTUnit_get_current());
	mutant_check_dir(mut->bin_path);
	GString * cmd = g_string_new("");
	//TODO: fix fix fix just for ms ddate
//	g_string_printf(cmd,"%s %s/%s %s -lm -o %s/%s",command, mut->src_path, ASTUnit_get_file_name(ASTUnit_get_current()), driver,  mut->bin_path, "mut.exe");

	g_string_printf(cmd,"cat %s >> %s/%s",driver, mut->src_path, ASTUnit_get_file_name(ASTUnit_get_current()),  mut->bin_path, "mut.exe");
//printf("%s\n",cmd->str);
//	g_string_printf(cmd,"%s/%s", mut->src_path, ASTUnit_get_file_name(ASTUnit_get_current()),  mut->bin_path, "mut.exe");

//	ff = fopen("driver")

	system(cmd->str);
	/*
	g_spawn_command_line_sync (cmd->str,
				NULL, //gchar **standard_output,
				NULL, //gchar **standard_error,
				NULL, //gint *exit_status,
				NULL //GError **error
		);
		*/

	g_string_printf(cmd,"");

	g_string_printf(cmd,"%s %s/%s -lm -o %s/%s",command, mut->src_path, ASTUnit_get_file_name(ASTUnit_get_current()),  mut->bin_path, "mut.exe");
//    printf("%s\n" , cmd->str);
		g_spawn_command_line_sync (cmd->str,
					NULL, //gchar **standard_output,
					NULL, //gchar **standard_error,
					NULL, //gint *exit_status,
					NULL //GError **error
			);


	g_string_free(cmd, TRUE);

}



// return ture if mutant is killed otherwise false
gboolean mutant_run(Mutant * mut, Mutant * std, gchar * test)
{

	mut == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"mutant running error") : 0;
	gchar * mut_output = NULL;
    gchar * std_output = NULL;
    gboolean is_killed = FALSE;

	if (! mut->compilable)
	{
        is_killed = TRUE;
		return is_killed;
	}

	GString * cmd = g_string_new("");
//	mutant_check_dir(mut->output_path);


	g_string_printf(cmd,"%s/%s %s", mut->bin_path, "mut.exe", test);

		g_spawn_command_line_sync (cmd->str,
					&mut_output, //gchar **standard_output,
					NULL, //gchar **standard_error,
					NULL, //gint *exit_status,
					NULL //GError **error
			);

	// debug printf("%s\n", mut_output);

	if(!g_hash_table_lookup (std_outputs,test))
	{
	g_string_printf(cmd,"%s/%s %s", std->bin_path, "mut.exe", test);

		g_spawn_command_line_sync (cmd->str,
					&std_output, //gchar **standard_output,
					NULL, //gchar **standard_error,
					NULL, //gint *exit_status,
					NULL //GError **error
			);

	g_hash_table_insert(std_outputs, test, g_string_chunk_insert (MiluStringPool, std_output));

    if (g_strcmp0(mut_output, std_output) == 0)
    {
    	is_killed = FALSE;
    }
    else
    {
    	is_killed = TRUE;
    }

    g_free(std_output);
	}

	else{
    if (g_strcmp0(mut_output, g_hash_table_lookup (std_outputs,test)) == 0)
    {
    	is_killed = FALSE;
    }
    else
    {
    	is_killed = TRUE;
    }

	}

	// compare results,
	g_string_free(cmd, TRUE);
    g_free(mut_output);

    return is_killed;
}


Mutant * mutant_new_original(gchar * base_path)
{
	return mutant_new(g_array_new (FALSE, FALSE, sizeof (gint)) , 0, base_path);
}


void mutant_set_killed(Mutant * mut)
{
    mut->iskilled = TRUE;
}


gboolean mutant_is_killed(Mutant * mut)
{
    return mut->iskilled;
}

gboolean mutant_is_compilable(Mutant * mut)
{
    return mut->compilable;
}

gboolean mutant_is_equivalent(Mutant * mut)
{
	return mut->isequivalent;
}

gboolean mutant_check_compiled(Mutant * mut)
{
    GString * cmd = g_string_new("");
	g_string_printf(cmd,"%s/%s", mut->bin_path, "mut.exe");
	 if (!g_file_test (cmd->str, G_FILE_TEST_EXISTS))
	 {
		 mut->compilable = FALSE;
	 }

     g_string_free(cmd, TRUE);

}
gboolean mutant_has_results(Mutant * mut)
{
    if(mut->results == NULL)
        return FALSE;
    else if(mut->results->len == 0)
    {
    	return FALSE;
    }
    else return TRUE;
}

void mutant_results_save(Mutant * mut, FILE * output)
{
    if (mutant_has_results(mut))
    {
 //     g_fprintf(output,"MILU_MUTANTS_RESULTS_FULL\n");
        for(gint i = 0 ; i < mut->results->len; i++)
        {
            g_fprintf(output,"%u", g_array_index(mut->results, guint8, i));
        }
    }
    else
    {
 //     g_fprintf(output,"MILU_MUTANTS_RESULTS_COMPACT\n");
        if(mutant_is_killed(mut))
        {
            g_fprintf(output,"%d", 1);
        }
        else
        {
            g_fprintf(output,"%d", 0);
        }
    }
}


void mutant_results_load(Mutant * mut, gchar * results)
{
    g_assert(results != NULL);
    gint size = strlen(results);

    if (size == 1)
    {
        if((gint) results[0] == '1')
        {
            mutant_set_killed(mut);
        }
    }

    else
    {
    for(gint i = 0; i < size; i++)
    {
        if(results[i] == '1')
        {
        	mutant_results_set_kill(mut);
            mutant_set_killed(mut);
        }
        else
        {
        	mutant_results_set_not_kill(mut);
        }
    }
    }
}

static Mutant * search_foms_with_mid(MutationId * mid, GPtrArray * foms)
{

	for(gint i = 0 ; i  < foms->len; i++)
	{
		Mutant * fom = g_ptr_array_index(foms, i);
		if( mutant_ids_is_equal_2_ordered_mids(mid, fom->id))
		{
			return fom;
		}
	}

	return NULL;
}

void mutant_evaluate_strongly_subsuming_fitness(Mutant * mut, GPtrArray * foms)
{
	g_assert(mut->id->len > 0);

//	printf("HOM: \n");
//	mutant_print(mut,stdout);
	for (gint i = 0 ; i < mut->id->len ; i  = i+ 2)
	{
		MutationId * mid = mutation_id_new();
		g_array_append_val (mid, g_array_index(mut->id, gint, i));
		g_array_append_val (mid, g_array_index(mut->id, gint, i+1));
		Mutant * f = search_foms_with_mid(mid, foms);
		g_ptr_array_add (mut->foms, f);
//		mutant_print(f,stdout);
	}


    guint  fom_results[mut->results->len] ;
    for(gint i = 0; i < mut->results->len; i++)
    {
          fom_results[i] = 0;
    }

    for(gint i = 0; i < mut->foms->len; i++)
    {
    	Mutant * f = g_ptr_array_index(mut->foms, i);
    	int eq_num = 0;
    	for(gint j = 0; j < f->results->len; j++)
    	{
    		 fom_results[i]  =  fom_results[i] + g_array_index(f->results,guint8,j);
    		 eq_num +=  g_array_index(f->results,guint8,j);
    	}
    	if(eq_num == 0)
    	{
    		 mut->fitness = -1.0;
    		 return;
    	}
    }

    int eq_num = 0;
    for(gint i = 0; i < mut->results->len; i++)
    {


    	//get intersection
          if(fom_results[i] > 1)
              fom_results[i] = 1;
          else
              fom_results[i] = 0;

    }


    guint  hom_results[mut->results->len] ;

    for(gint i = 0; i < mut->results->len; i++)
    {
          hom_results[i] = g_array_index(mut->results,guint8,i);
    }

    gint uv= 0;
    gint dv= 0;
    for(gint i = 0; i < mut->results->len; i++)
    {
          if( hom_results[i] != fom_results[i])
          {
        	  	 uv++;
          }
          if(hom_results[i] == 1)
          {
        	  dv++;
          }
    }


    if (dv == 0)
    	mut->fitness = 0;
    else
    mut->fitness = (float)uv/dv;
}

void mutant_evaluate_subsuming_fitness(Mutant * mut, GPtrArray * foms)
{
	g_assert(mut->id->len > 0);

	for (gint i = 0 ; i < mut->id->len ; i  = i+ 2)
	{
		MutationId * mid = mutation_id_new();
		g_array_append_val (mid, g_array_index(mut->id, gint, i));
		g_array_append_val (mid, g_array_index(mut->id, gint, i+1));
		Mutant * f = search_foms_with_mid(mid, foms);
		g_ptr_array_add (mut->foms, f);
	}


    guint  fom_results[mut->results->len] ;
    for(gint i = 0; i < mut->results->len; i++)
    {
          fom_results[i] = 0;
    }

    for(gint i = 0; i < mut->foms->len; i++)
    {
    	Mutant * f = g_ptr_array_index(mut->foms, i);
    	for(gint j = 0; j < f->results->len; j++)
    	{
    		 fom_results[i]  =  fom_results[i] + g_array_index(f->results,gint,j);
    	}
    }

    for(gint i = 0; i < mut->results->len; i++)
    {
    	//union
          if(fom_results[i] > 0)
              fom_results[i] = 1;
          else
              fom_results[i] = 0;
    }


    gdouble fomkills = 0;
    for(gint i = 0; i < foms->len; i++)
    {
    	 fomkills += fom_results[i] ;
    }
    gdouble homkills = 0;
    for(gint i = 0; i < mut->results->len; i++)
    {
    	 homkills += g_array_index(mut->results,gint,i);
    }

    if (fomkills == 0)
    	mut->fitness = 0;
    else
    	mut->fitness = homkills/fomkills;
}

void mutant_save_id(Mutant * mut, FILE * output)
{
	mutation_id_print(mut->id, output);
}
