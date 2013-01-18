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
#include "Mutator.h"

Mutator * mutator_new(const gchar * name, const gchar * info)
{
	 Mutator * mut = g_slice_new0 (Mutator);
	 mut->name = g_string_chunk_insert (MiluStringPool, name);
	 mut->info = g_string_chunk_insert (MiluStringPool, info);
	 return mut;
}

void mutator_free(Mutator * mut)
{
	g_slice_free(Mutator, mut);
}

void mutators_print(GPtrArray * muts, FILE * output)
{
	for (int i = 0; i < muts->len; i++)
	{
		Mutator * curr_mut = g_ptr_array_index(muts, i);
		mutator_print(curr_mut, output);
	}
}
void mutator_print(Mutator * mut, FILE * output)
{
	g_fprintf(output, "Name: %s\n", mut->name);
	g_fprintf(output, "Info: %s\n\n", mut->info);
}

extern gint CURR_SAVING_MUT;
ASTNode * mutator_bio_node_generator_for_austin_milu(ASTNode * node, gchar * mut_text)
{
            ASTNode * lop = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 1), TRUE);
            ASTNode * rop = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 2), TRUE);

            ASTNode * lop1 = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 1), TRUE);
            ASTNode * rop1 = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 2), TRUE);

            ASTNode * mop = ASTNode_new_bop_node(mut_text, lop, rop);
            ASTNode * oop = ASTNode_new_bop_node(node->text, lop1, rop1);


            ASTNode * lp = ASTNode_new_paren_node(mop);
            ASTNode * rp = ASTNode_new_paren_node(oop);
            ASTNode * exp = ASTNode_new_bop_node("!=", lp, rp);

            ASTNode * comp = ASTNode_new_comp_stmt_node();

            gchar * mfun = g_strdup_printf("killed_mutant(%d)", CURR_SAVING_MUT+1);
            ASTNode * src = ASTNode_new_milu_src_node (mfun);
            g_free(mfun);

            ASTNode_append_child(comp,src);
            ASTNode * if_node = ASTNode_new_if_node(exp,comp);

            ASTNode * insert_node = ASTNode_search_for_parent_node_with_parent(node, NodeKind_CompoundStmt);
            ASTNode_insert_before(insert_node, if_node);

            return if_node;
}

ASTNode * mutator_integer_constant_node_generator_for_austin_milu(ASTNode * node, gchar * mut_text)
{
	  	    ASTNode * lp = ASTNode_shallow_copy(node, TRUE);

            ASTNode * rp = ASTNode_new_integer_literal_node(mut_text);

            ASTNode * exp = ASTNode_new_bop_node("!=", lp, rp);

            ASTNode * comp = ASTNode_new_comp_stmt_node();

            gchar * mfun = g_strdup_printf("killed_mutant(%d)", CURR_SAVING_MUT+1);
            ASTNode * src = ASTNode_new_milu_src_node (mfun);
            g_free(mfun);

            ASTNode_append_child(comp,src);
            ASTNode * if_node = ASTNode_new_if_node(exp,comp);

            ASTNode * insert_node = ASTNode_search_for_parent_node_with_parent(node, NodeKind_CompoundStmt);
            ASTNode_insert_before(insert_node, if_node);

            return if_node;
}
ASTNode * mutator_reaching_node_only_generator_for_austin_milu(ASTNode * node, gchar * mut_text)
{
	/*
            ASTNode * lop = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 1), TRUE);
            ASTNode * rop = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 2), TRUE);

            ASTNode * lop1 = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 1), TRUE);
            ASTNode * rop1 = ASTNode_shallow_copy(ASTNode_get_nth_child(node , 2), TRUE);

            ASTNode * mop = ASTNode_new_bop_node(mut_text, lop, rop);
            ASTNode * oop = ASTNode_new_bop_node(node->text, lop1, rop1);


            ASTNode * lp = ASTNode_new_paren_node(mop);
            ASTNode * rp = ASTNode_new_paren_node(oop);
            ASTNode * exp = ASTNode_new_bop_node("!=", lp, rp);

            ASTNode * comp = ASTNode_new_comp_stmt_node();
*/
            gchar * mfun = g_strdup_printf("killed_mutant(%d)", CURR_SAVING_MUT+1);
            ASTNode * src = ASTNode_new_milu_src_node (mfun);
            g_free(mfun);

       //     ASTNode_append_child(comp,src);
         //   ASTNode * if_node = ASTNode_new_if_node(exp,comp);

            ASTNode * insert_node = ASTNode_search_for_parent_node_with_parent(node, NodeKind_CompoundStmt);
            ASTNode_insert_before(insert_node, src);

            return src;
}

