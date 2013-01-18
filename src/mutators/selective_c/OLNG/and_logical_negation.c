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
#include "ASTNode.h"


static gboolean mutator_milu_and_logical_negation_node_checking(ASTNode *);
static gboolean mutator_milu_and_logical_negation_clean(ASTNode * node, gint type);
static gboolean mutator_milu_and_logical_negation_mutate(ASTNode * node, gint type);


Mutator * mutator_milu_and_logical_negation()
{
	Mutator * mut = mutator_new("and_logical negation", "");
	mut->node_checking = & mutator_milu_and_logical_negation_node_checking;
	mut->mutate = & mutator_milu_and_logical_negation_mutate;
	mut->clean = & mutator_milu_and_logical_negation_clean;
	mut->size = 3;
	return mut;
}


static gboolean mutator_milu_and_logical_negation_node_checking(ASTNode * node)
{
	if(is_ASTNode_and_op(node))
		return TRUE;
	return FALSE;
}

static gboolean mutator_milu_and_logical_negation_mutate(ASTNode * node, gint type)
{

	ASTNode * pnode ;
	ASTNode * unode ;
	ASTNode * cnode ;
	ASTNode * ori_parent;
	ASTNode * ori_next;
	ASTNode * ori_prev;

	switch(type)
	{

		case 1:
			cnode = node;
			ori_parent = cnode->parent;
			ori_next = cnode->next_sibling;
			ori_prev = cnode->prev_sibling;

			pnode = ASTNode_new_paren_node(cnode);
			unode = ASTNode_new_uop_node("!", pnode);
			ASTNode_replace_with_ori_links(cnode, unode, ori_parent, ori_next, ori_prev);

			return TRUE;
		case 2:
			cnode = node->children;
			ori_parent = cnode->parent;
			ori_next = cnode->next_sibling;
			ori_prev = cnode->prev_sibling;

			pnode = ASTNode_new_paren_node(cnode);
			unode = ASTNode_new_uop_node("!", pnode);
			ASTNode_replace_with_ori_links(cnode, unode, ori_parent, ori_next, ori_prev);
			return TRUE;

		case 3:
			cnode = node->children->next_sibling;
			ori_parent = cnode->parent;
			ori_next = cnode->next_sibling;
			ori_prev = cnode->prev_sibling;

			pnode = ASTNode_new_paren_node(cnode);
			unode = ASTNode_new_uop_node("!", pnode);
			ASTNode_replace_with_ori_links(cnode, unode, ori_parent, ori_next, ori_prev);
			return TRUE;

		default:
			break;
	}



	return FALSE;
}

static gboolean mutator_milu_and_logical_negation_clean(ASTNode * node, gint type)
{

	ASTNode * cnode;


	if(type == 1)
	{
		cnode = node;

	}
	else if(type == 2 )
	{
			cnode = node->children->children->children;

	}
	else if(type == 3 )
		{
				cnode = node->children->next_sibling->children->children;

	}
	ASTNode * pnode = cnode->parent;
	ASTNode * unode = pnode->parent ;
	ASTNode * ori_parent = unode->parent;
	ASTNode * ori_next = unode->next_sibling;
	ASTNode * ori_prev = unode->prev_sibling;

		ASTNode_clean_replace_ori_links(cnode, unode, ori_parent, ori_next, ori_prev);
		ASTNode_clean_link(unode);
		ASTNode_free(unode);
		ASTNode_clean_link(pnode);
		ASTNode_free(pnode);

	return TRUE;
}
