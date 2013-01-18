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

#include "MutationId.h"

// Private function interface

static GArray * merge_2mids(GArray * mid1, GArray * mid2);
static gboolean exist_loc_mid(GArray * mid, gint loc);
static gboolean is_equal_2mids(GArray * mid1, GArray * mid2);
static gboolean is_equal_2_ordered_mids(GArray * mid1, GArray * mid2);
static gint get_id_by_loc(GArray * mid, gint loc);
static gboolean exist_loc_id_mid(GArray * mid, gint loc, gint id);
static gboolean exist_mid_mids(MutationId * mid, GPtrArray * mids);
static gboolean exist_ordered_mid_mids(MutationId * mid, GPtrArray * mids);
static void sort_mid(GArray * mid);
static void mutation_compact_id_expend(MutationCompactId * mcid, gint pos, MutationId * curr_id, GPtrArray * mids);
static MutationId * mutation_id_clone(MutationId * mid);
static void mutation_compact_ids_get_next_order(GArray * mut_num_template, gint pos, gint order, MutationCompactId * curr_mcid, GPtrArray * mcids);

// Public function implementation

MutationId * mutation_id_new()
{
	return g_array_new (FALSE, FALSE, sizeof (gint));
}

MutationId * mutation_id_new_from_text(gchar * mid_text)
{
	MutationId * mid = mutation_id_new();
	gchar ** inputs  = g_strsplit (mid_text, " ", -1);
	gchar ** curr_input = inputs;
    while(*curr_input)
    {
     //   gchar * tmp_input = (*curr_input)[0] != '\0' ? g_string_chunk_insert (MiluStringPool, *curr_input) : NULL;
     //   if (tmp_input != NULL)
     //       g_ptr_array_add(test_inputs, tmp_input);

    	if(*curr_input)
    	{
    		if((*curr_input)[0] != '\0')
    		{
    		gint m = atoi(*curr_input);
    		g_array_append_val (mid, m);
    		}
    	}
    	curr_input++;
    }
	g_strfreev (inputs);
	return mid;
}

void mutation_id_free(MutationId * mid)
{
    g_array_free(mid,TRUE);
}

void mutation_ids_free(GPtrArray * mids)
{
    for(gint i = 0 ; i < mids->len; i++ )
    {
        MutationId * curr_mid = g_ptr_array_index(mids,i);
	    mutation_id_free(curr_mid);
    }
}

GPtrArray * mutation_ids_get_next_order(GPtrArray * mids, GPtrArray * fom_mids )
{
	GPtrArray * new_mids = g_ptr_array_new();

	for (int i = 0; i < mids->len; i++)
	{
		GArray * curr_mid = g_ptr_array_index(mids,i);
		for (int j = 0; j < fom_mids->len ; j++)
		{
			GArray * curr_fom_mid = g_ptr_array_index(fom_mids,j);

			GArray * mid = merge_2mids(curr_mid, curr_fom_mid);


			if (! is_equal_2mids(mid, curr_mid) && !exist_mid_mids(mid, new_mids))
			{
				g_ptr_array_add(new_mids, mid);
			}
			else
			{
				mutation_id_free(mid);
			}
		}
	}

	return new_mids;
}

GPtrArray * mutant_ids_get_random_nth_order(GArray * num_template, gint mutants_num, gint order_begin, gint order_end)
{
	GPtrArray * mids = g_ptr_array_new();
    gint curr_num = 0;
    gint trial_times = 0;

    while (curr_num < mutants_num && trial_times < MiluRandomMutantsTrialTimes)
    {
        gint order = g_rand_int_range(MiluGRand, order_begin , order_end + 1);
        MutationId * mid = mutation_id_get_random_nth_order(num_template, order);

        if (!exist_ordered_mid_mids(mid, mids))
        {
        g_ptr_array_add(mids, (gpointer) mid);
        curr_num++;
        }

        trial_times ++;
    }

    return mids;
}


GPtrArray * mutant_ids_get_random_nth_order_cover_fom(GArray * num_template, gint mutants_num, gint order)
{
    gint curr_order = 0;
    gint next_size = num_template->len;
    GPtrArray * mids = g_ptr_array_new();
    gboolean complete = FALSE;
    while(mids->len < mutants_num)
    {
    	for(gint m = 0 ; m < 100; m++)
    	{
    		for(gint i = 0 ; i < num_template->len / order; i++)
    	//	for(gint i = 0 ; i < order; i++)
    		{
    			GArray * mid = g_array_new (FALSE, FALSE, sizeof (gint));
   // 			for (gint j = i ; j < i + order ; j++)
  			for (gint j = 0 ; j < order  ; j ++ )
    			{
    				gint curr_loc = j * num_template->len/order + i;
    				gint curr_val_size = g_array_index (num_template , gint, curr_loc);
    				if (m < curr_val_size)
    				{
    					g_array_append_val (mid, curr_loc);
    					g_array_append_val (mid, m);
    				}
    			}
    			if(mid->len > 0)
    			{
    				sort_mid(mid);
    				g_ptr_array_add(mids, (gpointer) mid);
    			//	mutation_id_print(mid, stdout);
    			//	printf("\n");
    			}
    			if(mids->len == mutants_num)
    			{
    				complete = TRUE;
    				break;
    			}

    		}
    		if (complete)
    	    		break;
    	}
    	  if (complete)
    	    	break;
    }
    return mids;



}

GArray * mutation_id_get_random_nth_order(GArray * num_template, gint order)
{
	GArray * mid = g_array_new (FALSE, FALSE, sizeof (gint));

	gint curr_order = 0;
	gint next_size = num_template->len;

    while(curr_order != order)
    {
    	gint curr_loc = g_rand_int_range(MiluGRand, 0 , next_size);

    	for (gint i = 0 ; i < mid->len; i = i + 2)
    	{
            gint curr_check_loc  = g_array_index (mid, gint, i);

            if(curr_check_loc <= curr_loc)
            {
            	curr_loc++;
            }
    	}

        gint curr_val_size = g_array_index (num_template , gint, curr_loc);
        gint curr_val = g_rand_int_range(MiluGRand, 1 , curr_val_size + 1);

        g_array_append_val (mid, curr_loc);
        g_array_append_val (mid, curr_val);

        next_size--;
        curr_order++;
    }

    sort_mid(mid);
    return mid;
}

GPtrArray * mutation_compact_ids_get_nth_order(GArray * num_template, gint order)
{

    g_assert(num_template && order > 0 && "Invalid arguments!");

	if(num_template->len < order)
	{
        if(num_template->len == 0)
        {
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot generate mutants, please try different operators!") ;

        }
        else
		g_log ("Milu",G_LOG_LEVEL_WARNING,"The max order for current subject under tt is num_template->len!") ;
	}

	GPtrArray * mcids = g_ptr_array_new();
    MutationCompactId * curr_mcid = mutation_id_new();
	mutation_compact_ids_get_next_order(num_template, 0, order,  curr_mcid,  mcids);
    mutation_id_free(curr_mcid);

	return mcids;
}


GPtrArray * mutation_compact_ids_expend(GPtrArray * mcids)
{
    GPtrArray * mids = g_ptr_array_new();

    for(gint i = 0 ; i< mcids->len; i++)
    {
        MutationCompactId * curr_mcid = g_ptr_array_index(mcids,i);
        MutationId * tmp_mid = mutation_id_new();
        mutation_compact_id_expend(curr_mcid, 1,tmp_mid, mids);
        mutation_id_free(tmp_mid);
    }
    return mids;
}

GPtrArray * mutation_ids_get_1st_order(GArray * num_template)
{
	GPtrArray * mids = g_ptr_array_new();
	for (int i = 0; i < num_template->len; i++)
	{
		gint curr_size = g_array_index (num_template , gint, i);
		for (int j = 1; j <= curr_size ; j++)
		{
			MutationId  * mid = mutation_id_new();
			g_array_append_val (mid, i);
			g_array_append_val (mid, j);
			g_ptr_array_add(mids, mid);
		}
	}
	return mids;
}

void mutation_id_print(MutationId * mid, FILE * output)
{
	for (gint i = 0; i < mid->len; i++)
		g_fprintf(output,"%d ", g_array_index (mid, gint, i) );
}

void mutation_ids_print(GPtrArray * mids, FILE * output)
{
	for (gint i = 0; i < mids->len; i++)
	{
		MutationId * mid = g_ptr_array_index(mids,i);
		mutation_id_print(mid,output);
		g_fprintf(output,"\n");
	}
}


// Private function implementation

static void sort_mid(MutationId * mid) // an optimised buble sort for mutation id
{
	g_assert(mid->len != 0);
	gint n = mid->len;
	gint curr_loc = 0;
	gint curr_id = 0;
	gint prev_loc = 0;
	gint prev_id = 0;

	gint * curr_loc_ptr;
	gint * curr_id_ptr;
	gint * prev_loc_ptr;
	gint * prev_id_ptr;
	gint tmp_int;

	while (n!=0)
	{
		gint newn = 0;
		for (int i = 2; i < n; i = i+2)
		{
			curr_loc = g_array_index(mid, gint, i);
			curr_id = g_array_index(mid, gint, i+1);
			prev_loc = g_array_index(mid, gint, i-2);
			prev_id = g_array_index(mid, gint, i+1-2);

			if( prev_loc > curr_loc)
			{
					curr_loc_ptr = & g_array_index(mid, gint, i);
					curr_id_ptr = & g_array_index(mid, gint, i+1);
					prev_loc_ptr = & g_array_index(mid, gint, i-2);
					prev_id_ptr = & g_array_index(mid, gint, i+1-2);

					tmp_int = *curr_loc_ptr;
					*curr_loc_ptr = * prev_loc_ptr;
					*prev_loc_ptr = tmp_int;

					tmp_int = *curr_id_ptr;
					*curr_id_ptr = * prev_id_ptr;
					*prev_id_ptr = tmp_int;

					newn = i;
			}
			else if(prev_loc == curr_loc)
			{
				if (prev_id > curr_id)
				{
					//	swap();
					curr_loc_ptr = & g_array_index(mid, gint, i);
					curr_id_ptr = & g_array_index(mid, gint, i+1);
					prev_loc_ptr = & g_array_index(mid, gint, i-2);
					prev_id_ptr = & g_array_index(mid, gint, i+1-2);

					tmp_int = *curr_loc_ptr;
					*curr_loc_ptr = * prev_loc_ptr;
					*prev_loc_ptr = tmp_int;

					tmp_int = *curr_id_ptr;
					*curr_id_ptr = * prev_id_ptr;
					*prev_id_ptr = tmp_int;

					newn = i;
				}
			}
		}
		n = newn;
	}

}

static gint get_id_by_loc(GArray * mid, gint loc) // Depreciated
{
	gint curr_loc = 0;

	for (int i = 0; i < mid->len; i = i + 2)
	{
		curr_loc = g_array_index(mid, gint, i);
		if (curr_loc == loc)
			return g_array_index(mid, gint, i+1);
	}

	g_log ("Milu",G_LOG_LEVEL_ERROR,"Should not be reach") ;
	return -1;
}

static gboolean exist_mid_mids(MutationId * mid, GPtrArray * mids)
{
	MutationId * curr_mid;
	for(int i =0; i < mids->len; i++)
	{
		curr_mid = g_ptr_array_index(mids, i);
		if (is_equal_2mids(curr_mid, mid))
		{
			return TRUE;
		}
	}
	return FALSE;
}

static gboolean exist_ordered_mid_mids(MutationId * mid, GPtrArray * mids)
{
	MutationId * curr_mid;
	for(int i =0; i < mids->len; i++)
	{
		curr_mid = g_ptr_array_index(mids, i);
		if (is_equal_2_ordered_mids(curr_mid, mid))
		{
			return TRUE;
		}
	}
	return FALSE;
}

 gboolean mutant_ids_is_equal_2_ordered_mids(GArray * mid1, GArray * mid2)
{
    if(mid1->len != mid2->len)
        return FALSE;

	for (int i = 0; i < mid1->len; i ++)
	{
		if(g_array_index(mid1, gint, i) != g_array_index(mid2, gint, i))
			return FALSE;
	}
	return TRUE;

}
static gboolean is_equal_2_ordered_mids(GArray * mid1, GArray * mid2)
{
    if(mid1->len != mid2->len)
        return FALSE;

	for (int i = 0; i < mid1->len; i ++)
	{
		if(g_array_index(mid1, gint, i) != g_array_index(mid2, gint, i))
			return FALSE;
	}
	return TRUE;

}

static gboolean is_equal_2mids(GArray * mid1, GArray * mid2)
{
	gint curr_loc = 0;
	gint curr_id = 0;

    if(mid1->len != mid2->len)
        return FALSE;

	for (int i = 0; i < mid1->len; i = i +2)
	{
		curr_loc = g_array_index(mid1, gint, i);
		curr_id = g_array_index(mid1, gint, i+1);
		if(!exist_loc_id_mid(mid2, curr_loc, curr_id))
			return FALSE;
	}
	return TRUE;

}

static gboolean exist_loc_id_mid(GArray * mid, gint loc, gint id)
{
	gint curr_loc = 0;
	gint curr_id = 0;
	for (int i = 0; i < mid->len; i = i + 2)
	{
		curr_loc = g_array_index(mid, gint, i);
		curr_id = g_array_index(mid, gint, i+1);
		if (curr_loc == loc && curr_id == id)
			return TRUE;
	}

	return FALSE;
}

static gboolean exist_loc_mid(GArray * mid, gint loc)
{
	gint curr_loc = 0;
	for (int i = 0; i < mid->len; i = i + 2)
	{
		curr_loc = g_array_index(mid, gint, i);
		if (curr_loc == loc)
			return TRUE;
	}

	return FALSE;
}

static GArray * merge_2mids(GArray * mid1, GArray * mid2)
{
	GArray * mid = g_array_new (FALSE, FALSE, sizeof (gint));

	for (int i = 0; i < mid1->len; i = i +2)
	{
		gint curr_loc = g_array_index(mid1, gint, i);
		gint curr_id = g_array_index(mid1, gint, i+1);
		g_array_append_val (mid, curr_loc);
		g_array_append_val (mid, curr_id);
	}

	for (int j = 0; j < mid2->len ; j = j+2)
	{
		gint curr_loc = g_array_index(mid2, gint, j);
		gint curr_id = g_array_index(mid2, gint, j+1);
		if (!exist_loc_mid(mid, curr_loc))
		{
			g_array_append_val (mid, curr_loc);
			g_array_append_val (mid, curr_id);
		}
	}
	return mid;
}


static void mutation_compact_id_expend(MutationCompactId * mcid, gint pos, MutationId * curr_id, GPtrArray * mids)
{
    gint size = g_array_index(mcid, gint, pos);
    for (gint i = 1; i<= size; i++)
    {
        gint curr_loc = pos - 1;
        gint loc = g_array_index(mcid, gint, curr_loc);
        g_array_append_val(curr_id, loc);
        g_array_append_val(curr_id, i);

        if (pos >= mcid->len-1)
        {
        	g_ptr_array_add(mids, mutation_id_clone(curr_id));
        }
        else{
            mutation_compact_id_expend(mcid, pos + 2, curr_id, mids);
        }

        g_array_remove_index (curr_id, curr_id->len-1);
        g_array_remove_index (curr_id, curr_id->len-1);
    }
}

static MutationId * mutation_id_clone(MutationId * mid)
{
    	MutationId * new_mid = mutation_id_new();
        for(gint i = 0 ; i < mid->len ; i++)
        {
        	g_array_append_val(new_mid, g_array_index(mid,gint,i));
        }
        return new_mid;
}

static void mutation_compact_ids_get_next_order(GArray * mut_num_template, gint pos, gint order, MutationCompactId * curr_mcid, GPtrArray * mcids)
{
    for (gint i = pos; i<= mut_num_template->len - 1 ; i++)
    {
        gint size = g_array_index(mut_num_template, gint, i);

        g_array_append_val(curr_mcid, i);
        g_array_append_val(curr_mcid, size);

        if (order == 1)
        {
        	g_ptr_array_add(mcids, mutation_id_clone(curr_mcid));
        }
        else{
            mutation_compact_ids_get_next_order(mut_num_template, i + 1, order -1 , curr_mcid, mcids);
        }

        g_array_remove_index (curr_mcid, curr_mcid->len-1);
        g_array_remove_index (curr_mcid, curr_mcid->len-1);
    }
}
