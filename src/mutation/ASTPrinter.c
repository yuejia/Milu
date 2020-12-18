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

#include "Utility.h"
#include "ASTNode.h"
#include "ASTNodeUtil.h"

// Private function interface

static gboolean print_dot_connection(ASTNode * node,gpointer data);
static gboolean print_dot_label(ASTNode * pnode,gpointer data);
static void print_source_function_decl(ASTNode * parent, GString * buffer);
static void print_source_union_decl(ASTNode * parent, GString * buffer);
static void print_source_typedef_decl(ASTNode * parent, GString * buffer);
static void print_source_parm_decl(ASTNode * parent, GString * buffer);
static void	print_source_compound_stmt(ASTNode * parent, GString * buffer);
static void	print_source_decl_stmt(ASTNode * parent, GString * buffer);
static void	print_source_return_stmt(ASTNode * parent, GString * buffer);
static void	print_source_if_stmt(ASTNode * parent, GString * buffer);
static void	print_source_goto_stmt(ASTNode * parent, GString * buffer);
static void	print_source_label_stmt(ASTNode * parent, GString * buffer);
static void	print_source_var_decl(ASTNode * parent, GString * buffer, gboolean notype);
static void	print_source_unexposed_expr(ASTNode * parent, GString * buffer);
static void	print_source_decl_ref_expr(ASTNode * parent, GString * buffer);
static void	print_source_binary_operator(ASTNode * parent, GString * buffer, gboolean is_stmt);
static void	print_source_conditional_operator(ASTNode * parent, GString * buffer, gboolean is_stmt);
static void	print_source_unary_operator(ASTNode * parent, GString * buffer, gboolean is_stmt);
static void	print_source_unary_operator_append(ASTNode * parent, GString * buffer, gboolean is_stmt);
static void	print_source_expr(ASTNode * node, GString * buffer, gboolean is_stmt);
static void	print_source_integer_literal(ASTNode * parent, GString * buffer);
static void	print_source_float_literal(ASTNode * parent, GString * buffer);
static void	print_source_char_literal(ASTNode * parent, GString * buffer);
static void	print_source_array_subscript_expr(ASTNode * parent, GString * buffer);
static void	print_source_paren_stmt(ASTNode * parent, GString * buffer);
static void print_source_call_expr(ASTNode * parent, GString * buffer);
static void print_source_string_literal(ASTNode * node, GString * buffer);
static void print_source_type_ref(ASTNode * node, GString * buffer);
static void	print_source_switch_stmt(ASTNode * parent, GString * buffer);
static void	print_source_for_stmt(ASTNode * node, GString * buffer);
static void	print_source_stmt(ASTNode * node, GString * buffer);
static void print_source_member_ref_expr(ASTNode * node, GString * buffer);
static void	print_source_case_stmt(ASTNode * parent, GString * buffer);
static void print_source_init_list_expr(ASTNode * node, GString * buffer);
static void	print_source_while_stmt(ASTNode * parent, GString * buffer);
static void	print_source_do_stmt(ASTNode * parent, GString * buffer);
static void print_source_cstyle_cast_expr(ASTNode * parent, GString * buffer, gboolean is_stmt);
static void	print_source_default_stmt(ASTNode * parent, GString * buffer);

static void html_check_mutantion_node(ASTNode * node);
static gboolean html_check_mutantion_lines(int line);
// Public function implementation

static gint html_lines;
static GArray * html_mutation_lines;

static int check_milu_source_node_lines(ASTNode * node);

static int check_milu_source_node_lines(ASTNode * node)
{
	char * curr_pos = node->text;
	int i = 0;
	while(strstr (curr_pos, CR))
	{
		i++;
		curr_pos = strstr (curr_pos, CR) + 1;
	//	printf("%s%s", curr_pos, CR );
	}
	return i;
}

void ASTNode_print_dot(ASTNode * node, FILE * output)
{
	g_assert(node != NULL);
	g_assert(output != NULL);

	GString * mem_print = g_string_sized_new((gsize)102400);

	g_string_append_printf(mem_print,"%s%s","digraph G{ ", CR);
	parse_tree_node_traverse_pre_order (node, &print_dot_label, (gpointer) mem_print);
	parse_tree_node_traverse_pre_order (node, &print_dot_connection, (gpointer) mem_print);
	g_string_append_printf(mem_print,"%s}",CR);
	g_fprintf(output,"%s",mem_print->str);
	g_string_free(mem_print,TRUE);
}

void ASTNode_print_source(ASTNode* parent, FILE * output)
{
	g_assert(parent != NULL);
	g_assert(output != NULL);

	GString * mem_print = g_string_sized_new((gsize)102400);

	ASTNode * node = parent->children;
	while(node)
	{

		switch(node->kind)
		{
		case NodeKind_FunctionDecl:
		{
			print_source_function_decl(node, mem_print);
			break;
		}
		case NodeKind_TypedefDecl:
		{
			print_source_typedef_decl(node, mem_print);
			break;
		}
		case NodeKind_UnionDecl:
		{
			print_source_union_decl(node, mem_print);
			break;
		}
		case NodeKind_StructDecl:
		{
			print_source_union_decl(node, mem_print); // same as union
			break;
		}
		case NodeKind_VarDecl:
		{
			print_source_var_decl(node, mem_print, 0);
			g_string_append_printf(mem_print,";%s",CR);
			html_lines++;
            break;
		}
		case NodeKind_MiluSource:
		{
			if(node->text[0] == '#')
			{
				g_string_append_printf(mem_print,"%s %s", node->text,CR);
			}
			else
			g_string_append_printf(mem_print,"%s ;%s", node->text,CR);

		//	printf("%s",node->text);
			html_lines++;
            break;
		}
		default:
			g_string_append_printf(mem_print,"****%s ;%s", node->text,CR);
			break;

		}

		/*if (strstr(mem_print->str, "ulonglong")!=NULL)
			printf("%d", node->kind);*/

		node = node->next_sibling;
	}
	g_fprintf(output,"%s",mem_print->str);
	g_string_free(mem_print,TRUE);
}

void ASTNode_print_html(ASTNode* parent, FILE * output)
{
	g_assert(parent != NULL);
	g_assert(output != NULL);

	GString * mem_print = g_string_sized_new((gsize)102400);
	html_mutation_lines = g_array_new (FALSE, FALSE, sizeof (gint));
	html_lines = 0;
	ASTNode * node = parent->children;
	//printf("xx%d");
	while(node)
	{
        html_check_mutantion_node(node);

		switch(node->kind)
		{
		case NodeKind_FunctionDecl:
		{
			print_source_function_decl(node, mem_print);
			break;
		}
		case NodeKind_TypedefDecl:
		{
			print_source_typedef_decl(node, mem_print);
			break;
		}
		case NodeKind_UnionDecl:
		{
			print_source_union_decl(node, mem_print);
			break;
		}
		case NodeKind_StructDecl:
		{
			print_source_union_decl(node, mem_print); // same as union
			break;
		}
		case NodeKind_VarDecl:
		{
			print_source_var_decl(node, mem_print, 0);
			g_string_append_printf(mem_print,";%s",CR);
			html_lines++;
            break;
		}
		case NodeKind_MiluSource:
		{
			g_string_append_printf(mem_print,"%s ;%s", node->text,CR);
			html_lines += check_milu_source_node_lines(node)+1;
            break;
		}
		default:
			break;

		}
		node = node->next_sibling;
	}
    //GString *  html_mem_print = milu_utility_source_to_html(mem_print);
    // covert html
    gboolean flag = FALSE;
	mem_print = g_string_prepend(mem_print, "<html><head><style type=\"text/css\"> .mut { color:red; } </style> </head> <body>"); int cline = 0;
    for(gint i = 0 ; i < mem_print->len; i++)
    {
    	if (mem_print->str[i] == '\n')
    	{
    	//	if (i < mem_print->len - 1 && mem_print->str[i+1]=='\n')
    	//		continue;
            cline++;
            mem_print->str[i] = ' ';

            if(flag && html_check_mutantion_lines(cline))
            {
                mem_print = g_string_insert (mem_print, i, "</div><a name=\"mut\">&nbsp;</a><br /><div class=\"mut\">");
            }
            else if(flag)
            {
                flag = FALSE;
                mem_print = g_string_insert (mem_print, i, "</div><br />");

            }
            else if (html_check_mutantion_lines(cline))
            {
                   flag = TRUE;
                   mem_print =  g_string_insert (mem_print, i, "<a name=\"mut\">&nbsp;</a> <br /><div class=\"mut\">");
            }
            else
            {
                   mem_print =  g_string_insert (mem_print, i, "<br />");
            }

         /*
            if(flag && html_check_mutantion_lines(i))
            {
            	flag = FALSE;
                mem_print = g_string_insert (mem_print, i, "</strong><br /><strong>");
            }
            else
            if (html_check_mutantion_lines(i))
            {
                   flag = TRUE;
                   mem_print =  g_string_insert (mem_print, i, "<br /><strong>");
            }
          //  else
           // if (flag)
            //      {
             //            flag = FALSE;
             //         g_string_insert (mem_print, i, "</strong><br />");
             //     }
    	 */
    	}

    }

	g_string_append_printf(mem_print, "</body></html>");


	g_fprintf(output,"%s",mem_print->str);
	//g_fprintf(output,"%s",html_mem_print->str);
	g_string_free(mem_print,TRUE);
	g_array_free (html_mutation_lines, TRUE);
}



static gboolean print_dot_label(ASTNode * node,gpointer data)
{
	switch(node->kind)
	{
	case NodeKind_VarDecl:
	case NodeKind_ParmDecl:
	case NodeKind_TypedefDecl:
	{
		if (node->type->kind != NodeTypeKind_Unexposed)
		{
			g_string_append_printf((GString *) data,"%lu [label=\"%s:%s, type:%s\"];%s",node->id, ASTNodeKindNames[node->kind], node->text, ASTNodeTypeKindNames[node->type->kind],CR );
		}
		else
		{
            if(node->type->node)
			g_string_append_printf((GString *) data,"%lu [label=\"%s:%s, type:%s\"];%s",node->id, ASTNodeKindNames[node->kind], node->text, node->type->node->text,CR );
            else
			g_string_append_printf((GString *) data,"%lu [label=\"%s:%s, type:unknown\"];%s",node->id, ASTNodeKindNames[node->kind], node->text,CR);

		}
			html_lines++;
		break;
	}

	case NodeKind_FunctionDecl:
	{
		g_string_append_printf((GString *) data,"%lu [label=\"%s:%s, type:%s\"];%s",node->id, ASTNodeKindNames[node->kind], node->text, ASTNodeTypeKindNames[node->type->kind],CR );
		html_lines++;
		break;
	}
//	case NodeKind_MiluSource:
//	{
//		break;
//	}
	default:
	{

		g_string_append_printf((GString *) data,"%lu [label=\"%s:%.8s\"];%s",node->id, ASTNodeKindNames[node->kind], node->text,CR);
		html_lines++;
		break;
	}
	}
	/*
	if(pnode->type != MILU_STRING_LITERAL)
	{
		g_string_append_printf((GString *) data,"%d [label=\"%s\"];%s",pnode->id,"String Literal",CR);
	}
	if (is_token_node(pnode))
	{
//		g_string_append_printf((GString *) data,"%d [label=\"%d\"];%s",pnode->id,pnode->type,CR);
		g_string_append_printf((GString *) data,"%d [label=\"%d\"];%s",pnode->id,pnode->text,CR);
	}
	else if(pnode->type != MILU_STRING_LITERAL)
	{
		g_string_append_printf((GString *) data,"%d [label=\"%s\"];%s",pnode->id,pnode->text->str,CR);
	}
		*/
	return FALSE;
}

static gboolean print_dot_connection(ASTNode * node,gpointer data)
{
	ASTNode * curr_node = node->children;

	while(curr_node)
	{
		g_string_append_printf((GString *) data,"%lu", node->id);
		g_string_append_printf((GString *) data,"->");
		g_string_append_printf((GString *) data,"%lu%s",curr_node->id,CR);
		html_lines++;
		curr_node = curr_node->next_sibling;
	}
	return FALSE;
}



//----------------- source printer


static void print_source_string_literal(ASTNode * node, GString * buffer)
{
    html_check_mutantion_node(node);
	g_string_append_printf(buffer,"%s ",  node->text);
}

static void print_source_parm_decl(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;

	if (parent->type->kind == NodeTypeKind_Unexposed &&
			node != NULL && node->kind == NodeKind_MiluSource) {
		g_string_append(buffer, node->text);
		return;
	}

	// should not reach here!
	g_string_append_printf(buffer,"%s %s ",  ASTNodeTypeKindNames[parent->type->kind],  parent->text);
}

static void print_source_init_list_expr(ASTNode * node, GString * buffer)
{
	ASTNode * children = node->children;

	g_string_append_printf(buffer,"{ ");

	while(children)
	{
		print_source_expr(children, buffer, FALSE);

		if(children->next_sibling) {
			g_string_append_printf(buffer,", ");
		}

		children = children->next_sibling;
	}

	g_string_append_printf(buffer,"} ");
}

static void	print_source_var_decl(ASTNode * parent, GString * buffer, gboolean notype)
{
    html_check_mutantion_node(parent);
    ASTNode * node = parent->children;

    if(parent->type->kind == NodeTypeKind_Unexposed)
    {
        g_string_append_printf(buffer,"%s ",   parent->type->text);
        return;
    }

    if(ASTNode_get_linkage(parent) == NodeLinkage_External)
        g_string_append_printf(buffer,"extern ",   parent->text);

    if(ASTNode_is_const(parent))
        g_string_append_printf(buffer,"const ",   parent->text);

    if(ASTNode_is_static(parent))
    {
        g_string_append_printf(buffer,"static ",   parent->text);
    }

	if(parent->type->kind == NodeTypeKind_ConstantArray)
	{
		// leading type
		if (!notype) {
			g_string_append_printf(buffer,"%s ",  parent->type->text);
		}

		// variable name
		g_string_append_printf(buffer,"%s ",  parent->text);

		// skip array element type (already handled in parent->type->text)
		node = node->next_sibling;

		// array size(s)
		while (node != NULL && node->kind != NodeKind_InitListExpr) {
			g_string_append_printf(buffer,"[");
			print_source_expr(node, buffer, FALSE);
			g_string_append_printf(buffer,"]");

			node = node->next_sibling;
		}

		// optional init list expr
		if (node != NULL && node->kind == NodeKind_InitListExpr) {
			g_string_append_printf(buffer, " = ");

			print_source_init_list_expr(node, buffer);

			node = NULL;
		}
	}
	else if(parent->type->kind == NodeTypeKind_Typedef)
	{
		if (!notype) {
			print_source_type_ref(node, buffer);
		}

		g_string_append_printf(buffer,"%s ",  parent->text);
		node = node->next_sibling;
	}
	else if(parent->type->kind == NodeTypeKind_Pointer)
	{
		// NodeKind_MiluSource
		ASTNode* source = node;
		node = node->next_sibling;
		if (source->kind != NodeKind_MiluSource) {
			g_string_append(buffer, "!Internal error: Expected NodeKind_MiluSource!");
			return;
		}

		ASTNode* type = node;
		node= node->next_sibling;

		if (!notype) {
			print_source_type_ref(type, buffer);
		}

		g_string_append_printf(buffer, "%s ", source->text);
		g_string_append_printf(buffer, "%s ", parent->text);
	}
	else if(node && node->kind == NodeKind_TypeRef)
	{
		if (!notype) {
			print_source_type_ref(node, buffer);
		}

		g_string_append_printf(buffer,"%s ",  parent->text);
		node= node->next_sibling;
	}
	else
	{
		if (!notype) {
			g_string_append_printf(buffer,"%s ",  ASTNodeTypeKindNames[parent->type->kind]);
		}
		g_string_append_printf(buffer,"%s ",  parent->text);
	}


    while(node) 
    {
        if(node->kind != NodeKind_UnexposedAttr)
            g_string_append_printf(buffer,"%s ", "=");
        else
            g_string_append_printf(buffer,"%s", " ");

        print_source_expr(node,buffer, 0);
        node = node->next_sibling;
    }

}

static void print_source_call_expr (ASTNode * parent, GString * buffer)
{

    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	print_source_expr(node, buffer, 0);
	node = node->next_sibling;

	g_string_append_printf(buffer,"( ");

	while(node)
	{
		print_source_expr(node, buffer, 0);
		if(node->next_sibling != NULL)
			g_string_append_printf(buffer,", ");

		node = node->next_sibling;
	}

	g_string_append_printf(buffer,") ");

}

static void	print_source_decl_ref_expr(ASTNode * node, GString * buffer)
{
    html_check_mutantion_node(node);
	g_string_append_printf(buffer,"%s ",   node->text);
}

static void	print_source_paren_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"( ");
	while(node)
	{

            print_source_expr(node,buffer,0);
		node = node->next_sibling;
	}

	g_string_append_printf(buffer,") ");
}


static void	print_source_compound_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"{%s",CR);
	html_lines++;
	while(node)
	{
        print_source_stmt(node, buffer);
		node = node->next_sibling;
	}
	// print (
	g_string_append_printf(buffer,"}%s",CR);
	html_lines++;
}

static void print_source_function_decl(ASTNode * parent, GString * buffer)
{
#if 0
	if (strcmp(parent->text, "CalcEffDruStatistik") == 0) {
		// set break point to stop when function
		int brk = 0;
	}
#endif

    html_check_mutantion_node(parent);
	//TODO fix type

	// print return value
	// print name
	// print (
	ASTNode * node = parent->children;
	ASTNode * node_unknown_attr = NULL;

	int n = ASTNode_get_children_number(parent);

	ASTNode * lastnode=NULL;
	ASTNode * child = parent->children;

	    gint curr_n = 1;
	    while(child)
	    {
	        if(curr_n == n)
	        	lastnode= child;

	         child = child->next_sibling;
	         curr_n++;
	    }

	//ASTNode * lastnode = ASTNode_get_nth_child(parent, n);
	// ASTNode * lastnode = ASTNode_get_nth_child(parent,ASTNode_get_children_number(parent));


	if(ASTNode_is_static(parent))
	{
		g_string_append_printf(buffer,"static ");
	}

	if(parent->type->kind == NodeTypeKind_Typedef)
	{
		g_string_append_printf(buffer,"%s %s ( ",  node->text,  parent->text);
		node= node->next_sibling;
	}
	else if(parent->type->kind == NodeTypeKind_Pointer)
	{
		if(node->text[1] != '*')
		{
			g_string_append_printf(buffer,"%s %s ( ",  node->text,  parent->text);
			node= node->next_sibling;
		}
		else
		{
			print_source_type_ref(node->next_sibling, buffer);
			g_string_append_printf(buffer,"%s %s (",  node->text,  parent->text);
			node= node->next_sibling;
			node= node->next_sibling;
		}
	}
	else
		g_string_append_printf(buffer,"%s %s ( ",  ASTNodeTypeKindNames[parent->type->kind],  parent->text);

	if(!node)
	{
		g_string_append_printf(buffer,");%s",CR);
		html_lines++;
	}


	while(node)
	{
		switch(node->kind)
		{
		// print par
		case NodeKind_ParmDecl:
			print_source_parm_decl(node, buffer);
			if(node->next_sibling)
			{
				if (node->next_sibling->kind == NodeKind_ParmDecl || node->next_sibling->kind == NodeKind_MiluSource)
					g_string_append_printf(buffer,", ");
			}
			if(!node->next_sibling)
			{
				g_string_append_printf(buffer,");%s",CR);
				html_lines++;
			}
			break;
		case NodeKind_MiluSource:
		{
                    if (node != lastnode)
                    {
			g_string_append_printf(buffer,"%s ", node->text);

			if(!node->next_sibling)
			{
				g_string_append_printf(buffer,");%s",CR);
				html_lines++;
			}
                    }
			break;
		}


		case NodeKind_CompoundStmt:
		{
                    if (lastnode->kind == NodeKind_MiluSource)
                    {
			g_string_append_printf(buffer,", %s ", lastnode->text);
			g_string_append_printf(buffer,")%s",CR);

                    }
                    else
                    {
			g_string_append_printf(buffer,")%s",CR);
                    }
			html_lines++;
			print_source_compound_stmt(node, buffer);
			break;
		}
		case NodeKind_UnexposedAttr:
		{
			g_string_append_printf(buffer,") ");
			g_string_append_printf(buffer,"%s ;%s", node->children->text,CR);
			html_lines++;
			break;
		}
		default:
		//	g_assert_not_reached();
			break;

		}
		node = node->next_sibling;
	}
}

static void	print_source_return_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"return ");
    if(node)
    print_source_expr(node, buffer, 0);
    /*
	while(node)
	{
		switch(node->kind)
		{
		case NodeKind_UnexposedExpr:
		{
			print_source_unexposed_expr(node, buffer);
			break;
		}
		case NodeKind_IntegerLiteral:
		{
        	print_source_integer_literal(node, buffer);
            break;

		}

		default:
			break;

		}
		node = node->next_sibling;
	}
    */
	g_string_append_printf(buffer,";%s",CR);
	html_lines++;

}

static void	print_source_if_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
    ASTNode * node = parent->children;
    gboolean is_else = FALSE;
    g_string_append_printf(buffer,"if ( ");
    while(node)
    {
        if(ASTNode_get_children_number(parent) > 2 && ASTNode_is_last_child(parent,node))
        {
            g_string_append_printf(buffer,"else ");
            is_else = TRUE;
        }
        if(node == parent->children)
            print_source_expr(node, buffer, 0);
        else
        {
            switch(node->kind)
            {

                case NodeKind_CompoundAssignOperator:
                case NodeKind_BinaryOperator:
                    {
                        if(!is_else)
                        {
                            g_string_append_printf(buffer,")%s",CR);
                            html_lines++;
                        }
                        print_source_binary_operator(node, buffer, TRUE);

                        break;
                    }

                    //TODO : potiential bug may need a statement cheking function
                case NodeKind_ForStmt:
                case NodeKind_CallExpr:
                case NodeKind_ReturnStmt:
                case NodeKind_IfStmt:
                case NodeKind_SwitchStmt:
                case NodeKind_ContinueStmt:
                case NodeKind_CStyleCastExpr:
                case NodeKind_CompoundStmt:
                case NodeKind_BreakStmt:
                case NodeKind_GotoStmt:
                case NodeKind_ParenExpr: //added a expression  type
                case NodeKind_DoStmt: //added a expression  type
	        case NodeKind_UnaryOperator_Append:
	        case NodeKind_UnaryOperator:
                case NodeKind_WhileStmt:
		case NodeKind_NullStmt:
                    {
                        if(!is_else)
                        {
                            g_string_append_printf(buffer,") ");
                        }
                        print_source_stmt(node, buffer);
                        break;

                    }

/*
	        case NodeKind_UnaryOperator_Append:
	        {
		    print_source_unary_operator_append(node, buffer, TRUE);
		    break;
	        }
*/

                default:
                    g_printf("-:%d%s", node->kind,CR);
                    g_assert_not_reached();
                    html_lines++;
                    break;

            }
        }
        node = node->next_sibling;
    }
    g_string_append_printf(buffer,CR);
    html_lines++;

}

static void	print_source_float_literal(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
//	g_string_append_printf(buffer,"%s ",  parent->children->text);

    //for the simple ast
	g_string_append_printf(buffer,"%s ",  parent->text);
}

static void	print_source_integer_literal(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
//	g_string_append_printf(buffer,"%s ",  parent->children->text);

    //for the simple ast
	g_string_append_printf(buffer,"%s ",  parent->text);
}

static void	print_source_char_literal(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	g_string_append_printf(buffer,"%s ",  parent->children->text);
}

static void print_source_type_ref(ASTNode * node, GString * buffer)
{
    if(node)
    {
    html_check_mutantion_node(node);
	g_string_append_printf(buffer,"%s ",  node->text);
    }
}

static void	print_source_unexposed_expr(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
    ASTNode * node = parent->children;
    if (node->text[0]=='s' && node->text[1]=='i' && node->text[5]=='f')
    {
        //fix duplication of sizeof
        print_source_expr(node, buffer, 0);
    }
    else
    {
        while(node)
        {
            print_source_expr(node, buffer, 0);
            node = node->next_sibling;
        }
    }
}

static void	print_source_array_subscript_expr(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;

		switch(node->kind)
		{
		case NodeKind_UnexposedExpr:
		{
			print_source_unexposed_expr(node, buffer);
			break;
		}
		case NodeKind_DeclRefExpr:
		{
			print_source_decl_ref_expr(node, buffer);
			break;
		}
                case NodeKind_ParenExpr:
                {
		    print_source_paren_stmt(node, buffer);
                    break;
                }
		default:
		{
                printf("%d --%s", node->kind,CR);
            g_assert_not_reached();
			break;
		}
		}
		node = node->next_sibling;

		g_string_append_printf(buffer,"[ ");
        print_source_expr(node, buffer, 0);
		g_string_append_printf(buffer,"] ");
}


static void	print_source_decl_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	while(node)
	{
		switch(node->kind)
		{
			case NodeKind_VarDecl:
			{
				if(node == parent->children)
					print_source_var_decl(node, buffer, 0);
                else
                	print_source_var_decl(node, buffer, 1);
				break;
			}

			case NodeKind_StructDecl:
            {
            	print_source_union_decl(node, buffer); // same as union
            	break;
            }

            case NodeKind_MiluSource:
            {
		        g_string_append_printf(buffer,"%s %s",  node->text,CR);
		        html_lines++;
		        break;
            }

		default:
			break;
		}
		node = node->next_sibling;
        if(node)
        	g_string_append_printf(buffer,", ");
	}
	g_string_append_printf(buffer,";%s",CR);
	html_lines++;
}




static void	print_source_expr(ASTNode * node, GString * buffer, gboolean is_stmt)
{
    html_check_mutantion_node(node);
	switch (node->kind)
	{
	case NodeKind_DeclRefExpr:
		{
			print_source_decl_ref_expr(node, buffer);
			break;
		}

	case NodeKind_ConditionalOperator:
	{
		print_source_conditional_operator(node, buffer, is_stmt);
        break;
	}
	case NodeKind_CompoundAssignOperator:
	case NodeKind_BinaryOperator:
	{
		print_source_binary_operator(node, buffer, is_stmt);
		break;
	}
	case NodeKind_UnaryOperator:
	{
		print_source_unary_operator(node, buffer, is_stmt);
		break;
	}
	case NodeKind_UnaryOperator_Append:
	{
		print_source_unary_operator_append(node, buffer, is_stmt);
		break;
	}
	case NodeKind_UnexposedExpr:
	{
		print_source_unexposed_expr(node,buffer);
		break;
	}
        case NodeKind_UnexposedAttr:
        {
		g_string_append_printf(buffer,"%s ;%s", node->children->text,CR);
		html_lines++;
		break;
        } 
	case NodeKind_CharacterLiteral:
	{
		print_source_char_literal(node,buffer);
		break;
	}
	case NodeKind_FloatingLiteral:
	{
		print_source_float_literal(node,buffer);
		break;
	}
	case NodeKind_IntegerLiteral:
	{
		print_source_integer_literal(node,buffer);
		break;
	}
	case NodeKind_ArraySubscriptExpr:
	{
    	print_source_array_subscript_expr(node ,buffer);
		break;
	}
	case NodeKind_ParenExpr:
	{
		print_source_paren_stmt(node, buffer);
        break;
	}
	case NodeKind_CallExpr:
	{
		print_source_call_expr(node,buffer);
        break;
	}
	case NodeKind_CStyleCastExpr:
	{
		print_source_cstyle_cast_expr(node,buffer,is_stmt);
        break;
	}
	case NodeKind_MemberRefExpr:
	{
		print_source_member_ref_expr(node, buffer);
        break;
	}
	case NodeKind_StringLiteral:
	{
		print_source_string_literal(node, buffer);
        break;
	}
	case NodeKind_TypeRef:
	{
		print_source_type_ref(node, buffer);
        break;
	}
	case NodeKind_LabelRef:
	{
		g_string_append_printf(buffer,"%s ", node->text);
		break;
	}
        case NodeKind_StmtExpr:
        {
            print_source_stmt(node->children,buffer);
            break;
        }
//	case NodeKind_IntegerLiteral
/*
        case NodeKind_CompoundLiteralExpr:
        {
            break;
        }
*/
	case NodeKind_MiluSource:
	 {
		        g_string_append_printf(buffer,"%s %s",  node->text,CR);
		        html_lines++;
		        break;
	}
        case NodeKind_InitListExpr:
        {
	        g_string_append_printf(buffer," ");
                print_source_init_list_expr(node, buffer);
		break;
        }
	default:
       g_printf("-%d %s%s", node->kind, node->text,CR);
        g_printf("%s %s", buffer->str,CR);
        g_assert_not_reached();
		break;
	}
}


static void	print_source_binary_operator(ASTNode * parent, GString * buffer, gboolean is_stmt)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;

	//print_source_expr(node, buffer, is_stmt);
	print_source_expr(node, buffer, FALSE);

	g_string_append_printf(buffer,"%s ", parent->text);
	node = node->next_sibling;

	print_source_expr(node, buffer, is_stmt);

	//TODO fix other statement
	if(parent->parent->kind != NodeKind_BinaryOperator 
            && parent->parent->kind != NodeKind_ConditionalOperator
            && is_stmt)
	{
		g_string_append_printf(buffer,";%s",CR);
		html_lines++;
	}
}

static void	print_source_unary_operator(ASTNode * parent, GString * buffer, gboolean is_stmt)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;

	g_string_append_printf(buffer,"%s ", parent->text);

	if(node)
		print_source_expr(node, buffer, is_stmt);

//	g_string_append_printf(buffer,"%s ", node->text);

	//TODO fix other statement
	if(parent->parent->kind != NodeKind_BinaryOperator && is_stmt)
	{
		g_string_append_printf(buffer,";%s",CR);
		html_lines++;
	}
}

static void	print_source_unary_operator_append(ASTNode * parent, GString * buffer, gboolean is_stmt)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;


	if(node)
		print_source_expr(node, buffer, is_stmt);

	g_string_append_printf(buffer,"%s ", parent->text);

	//TODO fix other statement
	if(parent->parent->kind != NodeKind_BinaryOperator && is_stmt)
	{
		g_string_append_printf(buffer,";%s",CR);
		html_lines++;
	}
}

static void	print_source_conditional_operator(ASTNode * parent, GString * buffer, gboolean is_stmt)
{
    html_check_mutantion_node(parent);

	ASTNode * node = parent->children;

	g_string_append_printf(buffer,"%s ", parent->text);

	if(node)
		print_source_expr(node, buffer, is_stmt);

	g_string_append_printf(buffer,"? ");
	node=node->next_sibling;

	if(node)
		print_source_expr(node, buffer, FALSE);

	g_string_append_printf(buffer,": ");
	node=node->next_sibling;

	if(node)
		print_source_expr(node, buffer, is_stmt);

	//TODO fix other statement
	if(parent->parent->kind != NodeKind_BinaryOperator && is_stmt)
	{
		g_string_append_printf(buffer,";%s",CR);
		html_lines++;
	}
}

static void print_source_typedef_decl(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
    ASTNode * children = parent->children;
    while(children)
    {
        if(children->kind == NodeKind_MiluSource)
        {
	        g_string_append_printf(buffer,"%s %s",  children->text,CR);
	        html_lines++;
        }
        children = children->next_sibling;
    }
}

static void print_source_union_decl(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
    ASTNode * children = parent->children;
    while(children)
    {
        if(children->kind == NodeKind_MiluSource)
        {
	        g_string_append_printf(buffer,"%s %s",  children->text,CR);
	        html_lines++;
        }
        children = children->next_sibling;
    }
}
static void	print_source_stmt(ASTNode * node, GString * buffer)
{
    html_check_mutantion_node(node);
	switch(node->kind)
		{
		case NodeKind_DeclStmt:
			print_source_decl_stmt(node, buffer);
			break;
	        case NodeKind_CompoundAssignOperator:
		case NodeKind_BinaryOperator:
			print_source_binary_operator(node,buffer,TRUE);
			break;
		case NodeKind_ReturnStmt:
			print_source_return_stmt(node, buffer);
			break;
		case NodeKind_IfStmt:
			print_source_if_stmt(node, buffer);
			break;
		case NodeKind_SwitchStmt:
			print_source_switch_stmt(node, buffer);
			break;
		case NodeKind_CallExpr:
		{
			print_source_call_expr(node,buffer);
	        g_string_append_printf(buffer,";%s",CR);
	        html_lines++;
            break;
		}
		case NodeKind_ForStmt:
			print_source_for_stmt(node,buffer);
            break;
		case NodeKind_WhileStmt:
			print_source_while_stmt(node,buffer);
            break;
		case NodeKind_DoStmt:
			print_source_do_stmt(node,buffer);
            break;
		case NodeKind_DefaultStmt:
		{
			print_source_default_stmt(node,buffer);
            break;
		}
		case NodeKind_BreakStmt:
		{
			g_string_append_printf(buffer,"%s ;%s", "break",CR);
			html_lines++;
            break;
		}
		case NodeKind_ContinueStmt:
		{
			g_string_append_printf(buffer,"%s ;%s", "continue",CR);
			html_lines++;
            break;
		}
		case NodeKind_CompoundStmt:
                {
			print_source_compound_stmt(node,buffer);
                        break;
                }
		case NodeKind_CaseStmt:
		{
			print_source_case_stmt(node,buffer);
            break;
		}
		case NodeKind_LabelStmt:
		{
			print_source_label_stmt(node,buffer);
            break;
		}
		case NodeKind_GotoStmt:
		{
			print_source_goto_stmt(node,buffer);
            break;
		}
		case NodeKind_MiluSource:
		{
			g_string_append_printf(buffer,"%s ;%s", node->text,CR);
			html_lines++;
            break;
		}
		case NodeKind_NullStmt:
		{
			g_string_append_printf(buffer,";%s",CR);
			html_lines++;
            break;
		}
		case NodeKind_CStyleCastExpr:
		{
			print_source_cstyle_cast_expr(node,buffer,TRUE);
            break;
		}
		case NodeKind_UnaryOperator:
		{
			print_source_unary_operator(node,buffer,TRUE);
			 break;
		}
		case NodeKind_UnaryOperator_Append:
		{
			print_source_unary_operator_append(node,buffer,TRUE);
			 break;
		}
		case NodeKind_ParenExpr:
		{
			print_source_paren_stmt(node,buffer);
			g_string_append_printf(buffer,";%s",CR);
						html_lines++;
			break;
		}
		case 215:
		{

			//TODO : fix for asm
			g_string_append_printf(buffer,";%s",CR);
			html_lines++;
            break;

		}
		default:
			g_printf("--%d%s", node->kind,CR);
			html_lines++;
			g_assert_not_reached();
			break;

		}
}

static void	print_source_switch_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"switch ( ");
	print_source_expr(node, buffer, 0);

	    node=node->next_sibling;
		g_string_append_printf(buffer,")%s",CR);
		html_lines++;

    print_source_stmt(node, buffer);

	g_string_append_printf(buffer,CR);
    html_lines++;

}

static void	print_source_do_stmt(ASTNode * parent, GString * buffer)
{

    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;

	g_string_append_printf(buffer,"do ");

    print_source_stmt(node, buffer);

	//g_string_append_printf(buffer,CR);
	//html_lines++;

    node=node->next_sibling;

    g_string_append_printf(buffer,"while( ");
    print_source_expr(node, buffer,0);
	g_string_append_printf(buffer,");%s",CR);
    html_lines++;
}

static void	print_source_while_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"while ( ");
    print_source_expr(node, buffer, 0);

    node=node->next_sibling;
	g_string_append_printf(buffer,")%s",CR);
	html_lines++;

    print_source_stmt(node, buffer);

	g_string_append_printf(buffer,CR);
    html_lines++;

}
static void	print_source_for_stmt(ASTNode * parent, GString * buffer)
{
        html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"for ( ");

//deal with for (;;)
if(ASTNode_get_children_number(parent) == 1 && node->kind == NodeKind_CompoundStmt)
{
	g_string_append_printf(buffer,"; ; ) %s",CR);
	html_lines++;

        print_source_stmt(node, buffer);

	g_string_append_printf(buffer,CR);
         html_lines++;
        return;
}


if(ASTNode_get_children_number(parent) == 3 && g_strcmp0(node->text, "=") != 0)
{
	g_string_append_printf(buffer,"; ");
}
else
{
    print_source_expr(node, buffer, 0);
    node=node->next_sibling;
    g_string_append_printf(buffer,"; ");
}


if(ASTNode_get_children_number(parent) == 3 && (node->kind == NodeKind_UnaryOperator_Append || node->kind == NodeKind_UnaryOperator))
{
    g_string_append_printf(buffer,"; ");
}
else
{

    print_source_expr(node, buffer, 0);
    node=node->next_sibling;

    g_string_append_printf(buffer,"; ");
}

    print_source_expr(node, buffer, 0);

    node=node->next_sibling;
	g_string_append_printf(buffer,")%s",CR);
	html_lines++;

    print_source_stmt(node, buffer);

	g_string_append_printf(buffer,CR);
    html_lines++;

}

static void	print_source_goto_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"goto ");
    print_source_expr(node, buffer, 0);
	g_string_append_printf(buffer,";%s",CR);
    html_lines++;
}

static void	print_source_default_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"%s :%s", " default",CR);
    html_lines++;
    print_source_stmt(node, buffer);
}

static void	print_source_case_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"%s ", "case");
    print_source_expr(node, buffer, 0);
	g_string_append_printf(buffer,": %s", CR);
    html_lines++;
    print_source_stmt(node->next_sibling, buffer);
}

static void	print_source_label_stmt(ASTNode * parent, GString * buffer)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;
	g_string_append_printf(buffer,"%s :", parent->text);
    print_source_stmt(node, buffer);
//	g_string_append_printf(buffer,";%s",CR);
//    html_lines++;

}

/*
static void print_source_cstyle_cast_expr(ASTNode * parent, GString * buffer, gboolean is_stmt)
{
    html_check_mutantion_node(parent);
	ASTNode * node = parent->children;

	g_string_append_printf(buffer,"(%s) ", node->text);
	node = node->next_sibling;

	print_source_expr(node, buffer, is_stmt);
}

*/
static void print_source_cstyle_cast_expr(ASTNode * parent, GString * buffer, gboolean is_stmt)
{
    html_check_mutantion_node(parent);
 	ASTNode * node = parent->children;
	while(node)
	{
		switch(node->kind)
		{
		case NodeKind_MiluSource:
		{
			g_string_append_printf(buffer,"( ");
			g_string_append_printf(buffer,"%s ",  node->text);
			g_string_append_printf(buffer,") ");
			// TODO : aquick fix, maybe need to clean the tree in the future
			if (node->next_sibling && node->next_sibling->kind == NodeKind_TypeRef)
			{
				node = node->next_sibling;
			}
			break;
		}
		default:
		{
			print_source_expr(node, buffer, is_stmt);
			break;
		}
		}

		node = node->next_sibling;
	}

	if(is_stmt)
	{
		g_string_append_printf(buffer,";%s",CR);
	    html_lines++;
	}

}

static void print_source_member_ref_expr(ASTNode * parent, GString * buffer)
{
   html_check_mutantion_node(parent);
   ASTNode * node = parent->children;

   gboolean dot = ASTNode_check_ext_3(parent);

   while(node)
   {
	     print_source_expr(node, buffer, 0);
         node= node->next_sibling;
   }

   if(dot)
   g_string_append_printf(buffer,". %s ", parent->text);
   else
   g_string_append_printf(buffer,"-> %s ", parent->text);
}

static void html_check_mutantion_node(ASTNode * node)
{
  //  g_assert(node && "Invalid node");
    if (node)
    {
    if(html_mutation_lines && node->is_mutant)
    {
        g_array_append_val (html_mutation_lines, html_lines);
    }
    }
    else
    {
    g_printf("Invalid node when producing html%s",CR	);
    }
}

static gboolean html_check_mutantion_lines(int line)
{
    for(gint i = 0 ; i < html_mutation_lines->len; i++)
    {
    	if(g_array_index (html_mutation_lines, gint, i) == line)
    	return TRUE;
    }
    return FALSE;
}

