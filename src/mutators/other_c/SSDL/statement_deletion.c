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
#include "ASTNodeUtil.h"

static gboolean mutator_milu_statement_node_checking(ASTNode * node);
static gboolean mutator_milu_statement_deletion_clean(ASTNode * node, gint type);
static gboolean mutator_milu_statement_deletion_mutate(ASTNode * node, gint type);

Mutator * mutator_milu_statement_deletion()
{
	Mutator * mut = mutator_new("Statement deletion", "");
	mut->node_checking = & mutator_milu_statement_node_checking;
	mut->mutate = & mutator_milu_statement_deletion_mutate;
	mut->clean = & mutator_milu_statement_deletion_clean;
	mut->size = 1;
	return mut;
}

static gboolean mutator_milu_statement_node_checking(ASTNode * node)
{
	if(node->parent->kind == NodeKind_CompoundStmt)
		return TRUE;
/*
    else if(node->parent->kind == NodeKind_IfStmt || ASTNode_get_nth_child(node->parent,2) == node)
		return TRUE;
            todo: need to add the following cases below
            node->parent->kink == NodeKind_WhileStmt ||
            node->parent->kink == NodeKind_DoStmt ||
            node->parent->kink == NodeKind_ForStmt  
*/
	return FALSE;
}

static gboolean mutator_milu_statement_deletion_mutate(ASTNode * node, gint type)
{
	ASTNodeType_save_original_kind(node);
	ASTNodeType_save_original_text(node);
	switch(type)
	{
		case 1:
			set_ASTNode_kind(node, NodeKind_MiluSource);
			set_ASTNode_text(node, "");
			return TRUE;
		default:
			break;
	}
	return FALSE;
}

static gboolean mutator_milu_statement_deletion_clean(ASTNode * node, gint type)
{
	ASTNodeType_restore_original_kind(node);
	ASTNodeType_restore_original_text(node);
	return TRUE;
}

