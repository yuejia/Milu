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

#include "ASTNodeUtil.h"

// Private function interface

/* check if the text info of the given node*/
static gboolean is_ASTNode_has_text(const ASTNode * node, const gchar * text );

/* check if the kind info of the given node*/
static gboolean is_ASTNode_has_kind(const ASTNode * node, NodeKind kind);


// Public function implementation

static gboolean is_ASTNode_has_kind(const ASTNode * node, NodeKind kind)
{
	if (node->kind  == kind)
		return TRUE;
	else
		return FALSE;
}

gboolean is_ASTNode_func_decl_with_body(const ASTNode * node)
{
	if(is_ASTNode_has_kind(node, NodeKind_FunctionDecl))
	{
		ASTNode * child = node->children;
		while(child)
		{
			if(child->kind == NodeKind_CompoundStmt)
			{
				return TRUE;
			}
				child = child ->next_sibling;
		}
	}
	return FALSE;
}
static gboolean is_ASTNode_has_text(const ASTNode * node, const gchar * text )
{

	if(node && node->text)
	{
	if (g_ascii_strcasecmp (text, node->text) == 0)
		return TRUE;
	else
		return FALSE;
	}
	return FALSE;
}

gboolean is_ASTNode_bitwise_and_assignment_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "&=");
}

gboolean is_ASTNode_bitwise_or_assignment_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "|=");
}

gboolean is_ASTNode_bitwise_and_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "&");
}

gboolean is_ASTNode_bitwise_or_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "|");
}

gboolean is_ASTNode_and_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "&&");
}

gboolean is_ASTNode_or_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "||");
}

gboolean is_ASTNode_add_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "+");
}

gboolean is_ASTNode_subtract_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "-");
}

gboolean is_ASTNode_multiply_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "*");
}

gboolean is_ASTNode_divide_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "/");
}

gboolean is_ASTNode_modulus_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "%");
}

gboolean is_ASTNode_assign_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "=");
}

gboolean is_ASTNode_kind_binary_operator(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_BinaryOperator);
}

gboolean is_ASTNode_kind_compound_assign_operator(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_CompoundAssignOperator);
}

void set_ASTNode_text(ASTNode * node, const gchar * text)
{
	node->text = g_string_chunk_insert (MiluStringPool, text);
}

void set_ASTNode_kind(ASTNode * node, NodeKind kind)
{
	node->kind = kind;
}

gboolean ASTNode_is_first_child(const ASTNode * parent, const ASTNode * node)
{
    g_assert(parent && node && "Invalid ASTNodes!");
    ASTNode * children = parent->children;
    return children == node;
}

gboolean ASTNode_is_last_child(const ASTNode * parent, const ASTNode * node)
{
	g_assert(parent && node && "Invalid ASTNodes!");

    ASTNode * children = parent->children;
    if(children)
    {
    while(children->next_sibling)
    {
        children = children->next_sibling	;
    }
    }
    return children == node;
}

guint ASTNode_get_children_number(const ASTNode * parent)
{
	g_assert(parent && "Invalid ASTNodes!");

    ASTNode * children = parent->children;
    guint num = 0;
    while(children)
    {
        num++;
        children = children->next_sibling	;
    }
    return num;
}

gboolean is_ASTNode_greater_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, ">");
}
gboolean is_ASTNode_less_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "<");
}
gboolean is_ASTNode_greater_equal_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, ">=");
}
gboolean is_ASTNode_less_equal_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "<=");
}
gboolean is_ASTNode_equal_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "==");
}
gboolean is_ASTNode_not_equal_op(const ASTNode * node)
{
	return is_ASTNode_kind_binary_operator(node) && is_ASTNode_has_text(node, "!=");
}
gboolean is_ASTNode_add_assign_op(const ASTNode * node)
{
	return is_ASTNode_kind_compound_assign_operator(node) && is_ASTNode_has_text(node, "+=");
}
gboolean is_ASTNode_multiply_assign_op(const ASTNode * node)
{
	return is_ASTNode_kind_compound_assign_operator(node) && is_ASTNode_has_text(node, "*=");
}
gboolean is_ASTNode_subtract_assign_op(const ASTNode * node)
{
	return is_ASTNode_kind_compound_assign_operator(node) && is_ASTNode_has_text(node, "-=");
}
gboolean is_ASTNode_divide_assign_op(const ASTNode * node)
{
	return is_ASTNode_kind_compound_assign_operator(node) && is_ASTNode_has_text(node, "/=");
}
gboolean is_ASTNode_modulus_assign_op(const ASTNode * node)
{
	return is_ASTNode_kind_compound_assign_operator(node) && is_ASTNode_has_text(node, "%=");
}

gboolean is_ASTNode_if_expression(const ASTNode * node)
{
	if(node->parent)
	return  node->parent->kind == NodeKind_IfStmt && node->parent->children == node;

	return FALSE;
}

gboolean is_ASTNode_while_expression(const ASTNode * node)
{
	if(node->parent)
	return  node->parent->kind == NodeKind_WhileStmt && node->parent->children == node;

	return FALSE;
}

gboolean is_ASTNode_kind_decl_ref_expr(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_DeclRefExpr);
}

gboolean is_ASTNode_right_hand_var(const ASTNode * node)
{

	if(is_ASTNode_has_kind(node, NodeKind_DeclRefExpr))
	{
		if(is_ASTNode_assign_op(node->parent) && ASTNode_is_first_child(node->parent,node))
		return FALSE;
		return TRUE;
	}
	return FALSE;
}

gboolean is_ASTNode_int_var(const ASTNode * node)
{

	// cover int, short, long, longlong
	if(is_ASTNode_has_kind(node, NodeKind_DeclRefExpr))
	{
		if(node->type)
			if(strcmp(node->type->text, "int") == 0
			||strcmp(node->type->text, "short") == 0
			||strcmp(node->type->text, "long") == 0
			||strcmp(node->type->text, "longlong") == 0
		)
				return TRUE;
	}
	return FALSE;
}

gboolean is_ASTNode_float_var(const ASTNode * node)
{

	if(is_ASTNode_has_kind(node, NodeKind_DeclRefExpr))
	{
		if(node->type)
			if(strcmp(node->type->text, "float") == 0
			||strcmp(node->type->text, "double") == 0
)
				return TRUE;
	}
	return FALSE;
}

gboolean is_ASTNode_kind_integer_literal(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_IntegerLiteral);
}

gboolean is_ASTNode_kind_float_literal(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_FloatingLiteral);
}

gboolean is_ASTNode_prefix_increment_op(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_UnaryOperator) && is_ASTNode_has_text(node, "++");
}

gboolean is_ASTNode_prefix_decrement_op(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_UnaryOperator) && is_ASTNode_has_text(node, "--");
}

gboolean is_ASTNode_postfix_increment_op(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_UnaryOperator_Append) && is_ASTNode_has_text(node, "++");
}

gboolean is_ASTNode_postfix_decrement_op(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_UnaryOperator_Append) && is_ASTNode_has_text(node, "--");
}

ASTNode * ASTNode_new_if_node(ASTNode * expr, ASTNode * comp)
{

	ASTNode * ifnode = ASTNode_new(NodeKind_IfStmt, "if", NULL);
	ASTNode_append_child(ifnode, expr);
	ASTNode_append_child(ifnode, comp);
    return ifnode;
}

ASTNode * ASTNode_get_nth_child(const ASTNode * parent, guint n)
{
    ASTNode * child = parent->children;

    gint curr_n = 1;
    while(child)
    {
        if(curr_n == n)
            return child;

         child = child->next_sibling;
         curr_n++;
    }

    return NULL;
}

ASTNode * ASTNode_new_bop_node(gchar * text,  ASTNode * lop, ASTNode * rop)
{
	ASTNode * bop_node = ASTNode_new( NodeKind_BinaryOperator, text, NULL);
	ASTNode_append_child(bop_node, lop);
	ASTNode_append_child(bop_node, rop);
    return bop_node;
}

ASTNode * ASTNode_new_uop_node(gchar * text, ASTNode * sub_node)
{
	ASTNode * uop_node = ASTNode_new( NodeKind_UnaryOperator, text, NULL);
	ASTNode_append_child(uop_node, sub_node);
    return uop_node;
}


ASTNode * ASTNode_new_comp_stmt_node()
{
	ASTNode * cmp_node = ASTNode_new(NodeKind_CompoundStmt, "", NULL);
    return cmp_node;
}


ASTNode * ASTNode_new_milu_src_node(gchar * text)
{
	ASTNode * milu_text_node = ASTNode_new(NodeKind_MiluSource, text, NULL);
    return milu_text_node;
}

ASTNode * ASTNode_new_paren_node(ASTNode * node)
{
	ASTNode * paren_node = ASTNode_new(NodeKind_ParenExpr, "", NULL);
    ASTNode_append_child(paren_node, node);
    return paren_node;
}

ASTNode * ASTNode_new_parm_decl_node(gchar * name, NodeTypeKind * type)
{
	ASTNode * pama_node = ASTNode_new(NodeKind_ParmDecl, name, NULL);
	pama_node->type = type;
	return pama_node;
}
ASTNode * ASTNode_new_integer_literal_node(gchar * value)
{
	ASTNode * int_node = ASTNode_new(NodeKind_IntegerLiteral, value, NULL);
	return int_node;
}

ASTNode * ASTNode_new_empty_function_node(gchar * func_name, ASTNode * params )
{
	ASTNode * func_node = ASTNode_new(NodeKind_FunctionDecl, func_name, NULL);
	ASTNode * cp_node = ASTNode_new_comp_stmt_node();

	if(params)
		ASTNode_append_child(func_node, params);

    ASTNode_append_child(func_node, cp_node);
	func_node->type = ASTNodeType_new(NodeTypeKind_Void, NULL);
	return func_node;

}

gboolean is_ASTNode_null_assignment(const ASTNode * node)
{
	ASTNode * child;
	if(is_ASTNode_assign_op(node)){
		child = ASTNode_get_nth_child(node, 2);
		if(child && is_ASTNode_has_kind(child, NodeKind_UnexposedExpr)){
			child=child->children;
			if(child){
				// it could have ParenExpr or not
				if(is_ASTNode_has_kind(child, NodeKind_ParenExpr)){
					child=child->children;
					if(!child){
						return FALSE;
					}
				}
				if(is_ASTNode_has_kind(child, NodeKind_CStyleCastExpr)){
					child=child->children;
					if(child && is_ASTNode_has_kind(child, NodeKind_MiluSource) && is_ASTNode_has_text(child, " void *")){
						child=child->next_sibling;
						if(child && is_ASTNode_has_kind(child, NodeKind_IntegerLiteral) && is_ASTNode_has_text(child, "0")){
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

gboolean is_ASTNode_calloc_call(const ASTNode * node)
{
	ASTNode * child;
	if(is_ASTNode_has_kind(node, NodeKind_UnexposedExpr) && is_ASTNode_has_text(node, "calloc")){
		child=node->children;
		if(is_ASTNode_has_kind(child, NodeKind_CallExpr) && is_ASTNode_has_text(child, "calloc")){
			return TRUE;
		}
	}
	return FALSE;
}

gboolean is_ASTNode_malloc_call(const ASTNode * node)
{
	ASTNode * child;
	if(is_ASTNode_has_kind(node, NodeKind_UnexposedExpr) && is_ASTNode_has_text(node, "malloc")){
		child=node->children;
		if(is_ASTNode_has_kind(child, NodeKind_CallExpr) && is_ASTNode_has_text(child, "malloc")){
			return TRUE;
		}
	}
	return FALSE;
}

gboolean has_ASTNode_calloc_call(const ASTNode * node)
{
	ASTNode * child;
	child=node->children;
	while(child){
		if(is_ASTNode_calloc_call(child)) return TRUE;
		child=child->next_sibling;
	}
	return FALSE;
}

gboolean has_ASTNode_malloc_call(const ASTNode * node)
{
	ASTNode * child;
	child=node->children;
	while(child){
		if(is_ASTNode_malloc_call(child)){
			return TRUE;
		}
		child=child->next_sibling;
	}
	return FALSE;
}

ASTNode * ASTNode_new_null_pointer_node()
{
	ASTNode * unexposed_node = ASTNode_new(NodeKind_UnexposedExpr, "", NULL);
	ASTNode * cast_node = ASTNode_new_with_parent(unexposed_node, NodeKind_CStyleCastExpr, "", NULL);
	ASTNode * void_node = ASTNode_new_with_parent(cast_node, NodeKind_MiluSource, " void *", NULL);
	ASTNode * zero_node = ASTNode_new_with_parent(cast_node, NodeKind_IntegerLiteral, "0", NULL);
	return unexposed_node;
}

gboolean replace_subtree_with(ASTNode * ori, ASTNode * replace)
{
	if(ori->prev_sibling){
		ori->prev_sibling->next_sibling=replace;
	}
	else if(ori->parent){
		ori->parent->children=replace;
	}
	if(ori->next_sibling){
		ori->next_sibling->prev_sibling=replace;
	}
	replace->parent=ori->parent;
	replace->prev_sibling=ori->prev_sibling;
	replace->next_sibling=ori->next_sibling;
	ori->parent=NULL;
	ori->prev_sibling=NULL;
	ori->next_sibling=NULL;
	return TRUE;
}

gboolean is_ASTNode_miluSource_sizeof(const ASTNode * node)
{
	if(is_ASTNode_has_kind(node, NodeKind_MiluSource) && node->text!=NULL && g_str_has_prefix(node->text, "sizeof")){
		return TRUE;
	}
	return FALSE;
}

gboolean is_ASTNode_sizeof_pointer(const ASTNode * node)
{
	if(!is_ASTNode_miluSource_sizeof(node)){
		return FALSE;
	}
	gint length;
	for(length=0; node->text[length]!=0; length++);
	for(length=length-1; length>0 && node->text[length]!=')'; length--);
	while(--length>0){
		if(node->text[length]!=' '){
			if(node->text[length]=='*') return TRUE;
			else return FALSE;
		}
	}
	return FALSE;
}

gboolean is_ASTNode_sizeof_nonpointer(const ASTNode * node)
{
	if(!is_ASTNode_miluSource_sizeof(node)){
		return FALSE;
	}
	gint length;
	for(length=0; node->text[length]!=0; length++);
	for(length=length-1; length>0 && node->text[length]!=')'; length--);
	while(--length>0){
		if(node->text[length]!=' '){
			if(node->text[length]=='*') return FALSE;
			else return TRUE;
		}
	}
	return FALSE;
}

gboolean is_ASTNode_free_statement(const ASTNode * node)
{
	return is_ASTNode_has_kind(node, NodeKind_CallExpr) && is_ASTNode_has_text(node, "free");
}

ASTNode * ASTNode_new_malloc_substitution_for_calloc(ASTNode* left, ASTNode* right)
{
	left->parent=NULL;
	left->prev_sibling=NULL;
	left->next_sibling=NULL;
	right->parent=NULL;
	right->prev_sibling=NULL;
	right->next_sibling=NULL;
	ASTNode * call_node = ASTNode_new(NodeKind_CallExpr, "malloc", NULL);
	ASTNode * decl_ref_node = ASTNode_new_with_parent(call_node, NodeKind_DeclRefExpr, "malloc", NULL);
	
	ASTNode * unexposed_node_left = ASTNode_new(NodeKind_UnexposedExpr, "", NULL);
	ASTNode * unexposed_node_right = ASTNode_new(NodeKind_UnexposedExpr, "", NULL);
	ASTNode * paren_node_left = ASTNode_new_with_parent(unexposed_node_left, NodeKind_ParenExpr, "", NULL);
	ASTNode * paren_node_right = ASTNode_new_with_parent(unexposed_node_right, NodeKind_ParenExpr, "", NULL);
	ASTNode_append_child(paren_node_left, left);
	ASTNode_append_child(paren_node_right, right);
	ASTNode * multiply_node=ASTNode_new_bop_node("*", unexposed_node_left, unexposed_node_right);
	ASTNode_append_child(call_node, multiply_node);
	return call_node;
}

gboolean ASTNode_set_null_statement(ASTNode* node)
{
	node->kind=NodeKind_NullStmt;
	set_ASTNode_text(node, "");
	node->children=NULL;
	return TRUE;
}

gboolean ASTNode_restore_free_statement(ASTNode* node, ASTNode* children)
{
	node->kind=NodeKind_CallExpr;
	set_ASTNode_text(node, "free");
	node->children=children;
	children->parent=node;
	children->next_sibling->parent=node;
	return TRUE;
}
