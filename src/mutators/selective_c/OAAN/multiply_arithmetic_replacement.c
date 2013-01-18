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

static gboolean mutator_milu_multiply_arithmetic_replacement_node_checking(ASTNode *);
static gboolean mutator_milu_multiply_arithmetic_replacement_clean(ASTNode * node, gint type);
static gboolean mutator_milu_multiply_arithmetic_replacement_mutate(ASTNode * node, gint type);


Mutator * mutator_milu_multiply_arithmetic_replacement()
{
	Mutator * mut = mutator_new("Arithmetic multiply operator", "");
	mut->node_checking = & mutator_milu_multiply_arithmetic_replacement_node_checking;
	mut->mutate = & mutator_milu_multiply_arithmetic_replacement_mutate;
	mut->clean = & mutator_milu_multiply_arithmetic_replacement_clean;
	mut->size = 4;
	return mut;
}

static gboolean mutator_milu_multiply_arithmetic_replacement_node_checking(ASTNode * node)
{
	if(is_ASTNode_multiply_op(node))
		return TRUE;
	return FALSE;
}

static gboolean mutator_milu_multiply_arithmetic_replacement_mutate(ASTNode * node, gint type)
{
	switch(type)
	{
		case 1:
			set_ASTNode_text (node, "+");
			return TRUE;

		case 2:
			set_ASTNode_text (node, "-");
			return TRUE;

		case 3:
			set_ASTNode_text (node, "/");
			return TRUE;

		case 4:
			set_ASTNode_text (node, "%");
			return TRUE;

		default:
			break;
	}

	return FALSE;
}

static gboolean mutator_milu_multiply_arithmetic_replacement_clean(ASTNode * node, gint type)
{
	set_ASTNode_text (node, "*");
	return TRUE;
}

