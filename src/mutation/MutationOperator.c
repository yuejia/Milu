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

#include "MutationOperator.h"
#include <glib.h>

extern GStringChunk *  MiluStringPool;


Operator * mutation_operator_new(const gchar * name, const gchar * info)
{
	Operator * op =  g_slice_new0 (Operator);
	op->name = g_string_chunk_insert (MiluStringPool, name);
	op->info = g_string_chunk_insert (MiluStringPool, info);
	op->mutators = g_ptr_array_new();
	return op;
}


void mutation_operator_free(Operator * op)
{
    g_assert(op);

// TODO: free mutator
   for(gint i = 0 ; i < op->mutators->len; i++)
   {
	   Mutator * mut = g_ptr_array_index(op->mutators, i);
       mutator_free(mut);
   }
   g_ptr_array_free(op->mutators,TRUE);
    g_slice_free(Operator, op);
}

GPtrArray * mutation_operators_load(const gchar * mut_op_path)
{
    // add user define operator.
	GPtrArray * operators = g_ptr_array_new();

    if (mut_op_path != NULL)
    {
        GPtrArray * ops = milu_utility_load_text_file_to_gptrarray(mut_op_path);
        for (gint i = 0 ; i < ops->len; i++)
        {
        	gchar * op_name = g_ptr_array_index(ops, i);
            if(g_strcmp0("OAAN", op_name) == 0)
            {
	            g_ptr_array_add(operators, (gpointer)get_OAAN_operator());
            }
            else if(g_strcmp0("OBBN", op_name) == 0)
            {
	            g_ptr_array_add(operators, (gpointer)get_OBBN_operator());
            }
            else if(g_strcmp0("OAAA", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_OAAA_operator());
            }
            else if(g_strcmp0("OBBA", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_OBBA_operator());
            }
            else if(g_strcmp0("ORRN", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_ORRN_operator());
            }
            else if(g_strcmp0("OLLN", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_OLLN_operator());
            }
            else if(g_strcmp0("OLNG", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_OLNG_operator());
            }
            else if(g_strcmp0("OCNG", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_OCNG_operator());
            }
            else if(g_strcmp0("CRCR", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_CRCR_operator());
            }
            else if(g_strcmp0("OIDO", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_OIDO_operator());
            }
            else if(g_strcmp0("SBRC", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_SBRC_operator());
            }
            else if(g_strcmp0("SSDL", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_SSDL_operator());
            }
            else if(g_strcmp0("ABS", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_ABS_operator());
            }
            else if(g_strcmp0("UOI", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_UOI_operator());
            }
            else if(g_strcmp0("Austin_ORRN", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_Austin_ORRN_operator());
            }
            else if(g_strcmp0("Austin_OAAN", op_name) == 0)
              {
              	g_ptr_array_add(operators, (gpointer)get_Austin_OAAN_operator());
              }
            else if(g_strcmp0("Austin_OLLN", op_name) == 0)
              {
              	g_ptr_array_add(operators, (gpointer)get_Austin_OLLN_operator());
              }
            else if(g_strcmp0("Austin_OLNG", op_name) == 0)
              {
              	g_ptr_array_add(operators, (gpointer)get_Austin_OLNG_operator());
              }
            else if(g_strcmp0("Austin_CRCR", op_name) == 0)
              {
              	g_ptr_array_add(operators, (gpointer)get_Austin_CRCR_operator());
              }
            else if(g_strcmp0("RMNA", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_RMNA_operator());
            }
            else if(g_strcmp0("REDAWN", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_REDAWN_operator());
            }
            else if(g_strcmp0("REDAWZ", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_REDAWZ_operator());
            }
            else if(g_strcmp0("RESOTPE", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_RESOTPE_operator());
            }
            else if(g_strcmp0("REMSOTP", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_REMSOTP_operator());
            }
            else if(g_strcmp0("REC2M", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_REC2M_operator());
            }
            else if(g_strcmp0("RMFS", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_RMFS_operator());
            }
            else if(g_strcmp0("REM2A", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_REM2A_operator());
            }
            else if(g_strcmp0("REC2A", op_name) == 0)
            {
            	g_ptr_array_add(operators, (gpointer)get_REC2A_operator());
            }
            else
            {
            	g_assert_not_reached();
            }
        }
    }
    else
    {
	            g_ptr_array_add(operators, (gpointer)get_OAAN_operator());
	            g_ptr_array_add(operators, (gpointer)get_OAAA_operator());
            	g_ptr_array_add(operators, (gpointer)get_ORRN_operator());
    }


	return operators;
}

void mutation_operators_print(GPtrArray * mut_operators, FILE * output)
{
	Operator * curr_op;
	for (gint i = 0; i < mut_operators->len; i++)
	{
		curr_op = g_ptr_array_index(mut_operators, i);
		mutation_operator_print(curr_op, output);
	}
}

guint mutation_operator_size(Operator * op)
{
	return op->mutators->len;
}

void mutation_operator_print(Operator * op, FILE * output)
{
	g_fprintf(output, "Name: %s\n", op->name);
	g_fprintf(output, "Info: %s\n\n", op->info);

	mutators_print(op->mutators, output);
}

void mutation_operators_free(GPtrArray * mut_operators)
{
	Operator * curr_op;
	for (gint i = 0; i < mut_operators->len; i++)
	{
		curr_op = g_ptr_array_index(mut_operators, i);
		mutation_operator_free(curr_op);
	}
}
