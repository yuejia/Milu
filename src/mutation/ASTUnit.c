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

#include "ASTUnit.h"
#include "ASTPrinter.h"


static CXTranslationUnit * CurrTU  = NULL;
static ASTUnit * CurrASTUnit = NULL;
static gchar * curr_src = NULL;
static GPtrArray * DataTypes = NULL;
static CXCursor * CurrFunc = NULL;
static CXCursor * CurrNode = NULL;
static GPtrArray * FuncLists = NULL;
static GHashTable* localSymTable = NULL;

static unsigned prev_binary_line = 100;
static unsigned prev_binary_column=100;
static unsigned prev_binary_offset=100;
static void remove_function_signiture();
static void add_original_non_mutation(ASTNode * ast);

// Private function interface

/* call libclang to parse current file. */
static void libclang_parse_file(ASTUnit* au, int argc, char *argv[ ] );

/* parse libclang node to build AST. */
enum CXChildVisitResult visit_ast(CXCursor cursor, CXCursor parent, CXClientData client_data);

/* check if current is a binary operator */
static gint check_binary_op(const char * op);


static gboolean is_current_functions_to_mutate(gchar * func_name);

//TODO fix

extern gboolean	PARSING_UNITTESTS;

// milu 3.1
static gchar * fix_binary_op(CXToken * tokens ,unsigned tokens_size);
static void load_c_source_file(const gchar * path);
static gboolean search_curr_source_file(gchar * text);
static gchar * resolve_pointer(CXType type);
static gboolean is_unary_token(const gchar * token);
static gint get_unary_operator_id(CXToken * tokens ,unsigned tokens_size);
static void clean_ast(ASTNode * root);
static void fix_data_types_duplication();
static gchar * resolve_function_attribute();
static void fix_function_attribute();
static gboolean fix_function_ellipsis();
static gboolean fix_function_static();
static gboolean fix_decl_static();
static gchar * fix_decl_type(gchar * var);
static gboolean fix_function_pointer(ASTNode * func);
static gboolean reset_function_text(ASTNode * func);

static void parse_tree_fix_header(gchar * src_path, ASTNode * root);

//TODO: remove the tag system
//static void parse_tree_fix_functions(ASTNode * func);

extern gboolean MILU_AUSTIN_TRANSFORM ;
// Public function implementation

ASTUnit * ASTUnit_new(const gchar * src_path)
{
	g_assert(src_path != NULL);
	ASTUnit * au =  g_slice_new0 (ASTUnit);
	CurrASTUnit = au;
	gint argc = 1;
	gchar * argv[1];
	argv[0] = src_path;
	au->file_path = g_string_chunk_insert (MiluStringPool, src_path);
	au->file_name = g_string_chunk_insert(MiluStringPool, g_path_get_basename (src_path));
	load_c_source_file(src_path);

    	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Libclang parse file")  : 0 ;
	libclang_parse_file(au, argc, argv);

    	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"AST Clean tree")  : 0 ;
	parse_tree_node_clean(au->ast);

// TODO:remove the old tag system
//	parse_tree_fix_functions(au->ast);

    if(!PARSING_UNITTESTS) //Optimisation for non-Unit test case(non austin test style) 
    {
    	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"AST add non mutation src")  : 0 ;
        add_original_non_mutation(au->ast);
        //	parse_tree_fix_header(src_path, au->ast); //no need , will be removed
        //ASTNode * af =	ASTNode_new_milu_src_node("Austin__Assume(int a, ...){} \n"); //no need , will be removed
        //	ASTNode_insert_before(au->ast->children, af);//no need , will be removed
    }

    if(MILU_AUSTIN_TRANSFORM) //Transformation for Austin
    {
    	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Run austin transformation")  : 0 ;
        parse_tree_node_transform(au->ast);
        NodeTypeKind * param_type = ASTNodeType_new(NodeTypeKind_Int, NULL);
        ASTNode * param_node = ASTNode_new_parm_decl_node("mid", param_type);
        ASTNode * node = ASTNode_new_empty_function_node("killed_mutant", param_node);
        ASTNode_append_child(au->ast, node);
    }
	return au;
}

void ASTUnit_free(ASTUnit * au)
{
	g_assert(au);

	if(au->ast != NULL)
		ASTNode_free(au->ast);

	g_slice_free (ASTUnit, au);
	g_free(curr_src);
}

static gboolean is_current_functions_to_mutate(gchar * func_name)
{

	if(PARSING_UNITTESTS)
		return TRUE;
	gchar * func_path = milu_options_get_func_list_path();
	if(func_path)
	{
		GPtrArray * all_u_functions = milu_utility_load_text_file_to_gptrarray(func_path);

			for(int t = 0 ; t < all_u_functions->len; t++)
			{
				gchar * f = g_ptr_array_index(all_u_functions, t);
				if( g_strcmp0(func_name, f) == 0)
				{
					return TRUE;
				}
			}
			return FALSE;
	}

//	g_log("Parsing error",G_LOG_LEVEL_WARNING, "Unknown function to mutate: %s ", func_name);

//	exit(0);
	return TRUE;
}

/* Todo; TO be removed tag system
static void parse_tree_fix_functions(ASTNode * func)
{
	ASTNode * child = func->children;

	gchar * tag_path = milu_options_get_tag_list_path();
	if(tag_path)
	{
		GPtrArray * all_u_functions = milu_utility_load_text_file_to_gptrarray(tag_path);

		//for(gint i  = 0 ; i < all_u_functions->len; i++)
		//{
		//		g_printf("%s\n", g_ptr_array_index(all_u_functions, i));
		//	}
		while(child)
		{
			//printf("--%s\n",child->text);
			gboolean func_exists = FALSE;
			for(int t = 0 ; t < all_u_functions->len; t++)
			{
				gchar * f = g_ptr_array_index(all_u_functions, t);
				if( g_strcmp0(child->text, f) == 0)
				{
					func_exists = TRUE;

					break;
				}
			}

			if( !func_exists)
			{
				if(child->next_sibling)
				{
					child = child->next_sibling;
					ASTNode_unlink(child->prev_sibling);
				}
				else
					break;
			}
			else
			{
				child = child->next_sibling;
			}
		}

	}

}
*/

static void parse_tree_fix_header(gchar * src_path, ASTNode * root)
{
	GPtrArray * src =  milu_utility_load_text_file_to_gptrarray(src_path);
	for (gint i = 0 ; i < src->len; i++)
	{
		gchar * line = g_ptr_array_index(src,i);
		if(line)
		{
			if(line[0] == '#')
			{
				if(line[1]== 'i')
				{
					ASTNode * node = ASTNode_new_milu_src_node(line);
					ASTNode_insert_before(root->children, node);
				}
			}
		}
	}

}


ASTUnit * ASTUnit_get_current()
{
	return CurrASTUnit;
}

gchar * ASTUnit_get_file_name(ASTUnit * au)
{
	return au->file_name;
}

// Private function implementation

static void libclang_parse_file(ASTUnit* au, int argc, char *argv[ ] )
{
	ASTNode * ast = ASTNode_new(NodeKind_TranslationUnit, NULL, NULL);
	au->ast = ast;

	CXIndex Index = clang_createIndex(0, 0);
	CXTranslationUnit TU = clang_parseTranslationUnit(Index, 0,
			argv, argc, 0, 0, CXTranslationUnit_None);

	//	CXTranslationUnit TU = clang_parseTranslationUnit(Index, 0,
	//			argv, argc, 0, 0,
	//			CXTranslationUnit_DetailedPreprocessingRecord);
	CurrTU = &TU;

    DataTypes = g_ptr_array_new();
    FuncLists = g_ptr_array_new();

    //TODO Free Datatypes

    CXCursor cursor =  clang_getTranslationUnitCursor(TU);

    clang_visitChildren(cursor, visit_ast, ast);

    if(localSymTable)
    {
	    g_hash_table_destroy(localSymTable);
	    localSymTable = NULL;
    }
    fix_function_attribute();
    clean_ast(ast);
    //	if(!PARSING_UNITTESTS)
    //    add_original_non_mutation(ast);

    clang_disposeTranslationUnit(TU);
    clang_disposeIndex(Index);
}

static gboolean parsing_in_func =FALSE;

enum CXChildVisitResult visit_ast(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
	NodeKind curr_type = (NodeKind)cursor.kind;
	CXString cstr = clang_getCursorSpelling(cursor);
	ASTNode * node = NULL;

	if(curr_type == NodeKind_FunctionDecl)
	{
		gchar * func_name = (gchar *)clang_getCString(cstr);
    	        MILU_GLOBAL_VERBOSE ? g_log ("Milu-PF",G_LOG_LEVEL_MESSAGE,func_name)  : 0 ;
                
                
		if(is_current_functions_to_mutate(func_name))
		{
			parsing_in_func = TRUE;
			localSymTable = g_hash_table_new(g_str_hash,g_str_equal);
		}
		else
		{
			if(localSymTable)
			{
				g_hash_table_destroy(localSymTable);
				localSymTable = NULL;
			}
			parsing_in_func = FALSE;
		}
	}
	if(parsing_in_func)
	{
		node = ASTNode_new_with_parent((ASTNode *) client_data, curr_type, (gchar *)clang_getCString(cstr), (gpointer)cursor.data[0]);

	//	ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);

		CXSourceLocation loc =	clang_getCursorLocation (cursor);
		CXSourceRange  ran =	clang_getCursorExtent (cursor);

		unsigned line;
		unsigned line1;
		unsigned column;
		unsigned column1;
		unsigned offset;
		unsigned offset1;

		clang_getSpellingLocation (loc, NULL, &line, &column, &offset);
		clang_getSpellingLocation ( clang_getRangeEnd(ran), NULL, &line1, &column1, &offset1);
		node->line_start = line;
		node->line_end = line1;
//		printf("-%u %u %u\n", line, column, offset);
//		printf("-%u %u %u\n\n", line1, column1, offset1);
	}
	else
	{
		return  CXChildVisit_Continue;
		//	node = ASTNode_new(curr_type,(gchar *)clang_getCString(cstr),(gpointer)cursor.data[0]);
	}

	switch(curr_type)
	{

	case NodeKind_StringLiteral:
	{
		ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);
		CXSourceRange  ran =	clang_getCursorExtent (cursor);

		CXToken * tokens = NULL;
                unsigned tokens_size = 0;
                clang_tokenize	((*CurrTU), ran, &tokens, &tokens_size);
                GString * tmp_string = g_string_new("");

                for (gint i = 0; i < tokens_size; i ++)
                {
                    CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[i]);
                    char * curr_token = clang_getCString(token_cstr);

                    if(curr_token[0] == '"')
                    {
                        g_string_append_printf(tmp_string, "%s\n", curr_token);
                        clang_disposeString(token_cstr);
                    }
                    else 
                    {
                        clang_disposeString(token_cstr);
                        break;
                    }
                }

                ASTNode_set_text(node , tmp_string->str);
                g_string_free(tmp_string,TRUE);

                clang_disposeTokens (*CurrTU, tokens, tokens_size);
                clang_visitChildren(cursor, visit_ast, node);

		break;
	}

	case NodeKind_UnaryOperator:
	{
		ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);

		CXSourceRange  ran =	clang_getCursorExtent (cursor);

		CXToken * tokens = NULL;
		unsigned tokens_size = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_size);
        gint u_id = get_unary_operator_id(tokens,tokens_size);
        //Quick fix
        if(u_id > 0)
            node->kind = NodeKind_UnaryOperator_Append;

		CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[u_id]);
		char * curr_token = clang_getCString(token_cstr);

		ASTNode_set_text(node , curr_token);

		clang_disposeString(token_cstr);

		clang_disposeTokens (*CurrTU, tokens, tokens_size);
		clang_visitChildren(cursor, visit_ast, node);

		break;
	}

	case NodeKind_CompoundAssignOperator:
	{

		ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);

		CXSourceLocation loc =	clang_getCursorLocation (cursor);
		CXSourceRange  ran =	clang_getCursorExtent (cursor);


        /*
		unsigned line;
		unsigned column;
		unsigned offset;

		clang_getSpellingLocation (loc, NULL, &line, &column, &offset);

*/

		CXToken * tokens = NULL;
		unsigned tokens_size = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_size);
	//		printf("%s: %d\n", ASTNodeKindNames[curr_type],i, clang_getTokenKind (tokens[0]), clang_getTokenKind (tokens[1]));

		for (gint i = 0 ; i < tokens_size; i++)
        {
            gchar * curr_text =  clang_getCString( clang_getTokenSpelling(*CurrTU,tokens[i]));
            if(g_strcmp0(curr_text,"+=") == 0
                || g_strcmp0(curr_text,"-=") == 0
                || g_strcmp0(curr_text,"*=") == 0
                || g_strcmp0(curr_text,"/=") == 0
                || g_strcmp0(curr_text,"%=") == 0
                || g_strcmp0(curr_text,"^=") == 0
                || g_strcmp0(curr_text,"&=") == 0
                || g_strcmp0(curr_text, "<<=") == 0 
                || g_strcmp0(curr_text, ">>=") == 0 
                || g_strcmp0(curr_text, "&=") == 0 
                || g_strcmp0(curr_text, "^=") == 0 
                || g_strcmp0(curr_text, "|=") == 0) 
            // printf("%s ", clang_getCString( clang_getTokenSpelling(*CurrTU,tokens[i])));
		    {
            	ASTNode_set_text(node , curr_text);
		        break;
		    }
        }


//		gchar * big_op = fix_binary_op(tokens, tokens_size);
//		g_free(big_op);

/*
		prev_binary_line = line;
		prev_binary_column = column;
		prev_binary_offset = offset;
*/
		clang_disposeTokens (*CurrTU, tokens, tokens_size);
		clang_visitChildren(cursor, visit_ast, node);

		break;
	}
	case NodeKind_BinaryOperator:
	{

		ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);

		CXSourceLocation loc =	clang_getCursorLocation (cursor);
		CXSourceRange  ran =	clang_getCursorExtent (cursor);


		unsigned line;
		unsigned column;
		unsigned offset;

		clang_getSpellingLocation (loc, NULL, &line, &column, &offset);


		CXToken * tokens = NULL;
		unsigned tokens_size = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_size);
		//	printf("%s: %d, %d, %d\n", ASTNodeKindNames[curr_type],i, clang_getTokenKind (to[0]), clang_getTokenKind (to[1]));

		gchar * big_op = fix_binary_op(tokens, tokens_size);
		ASTNode_set_text(node , big_op);
		g_free(big_op);

		/*        for (gint i = 0 ; i < tokens_size; i++)
        {
		    printf("%s ", clang_getCString( clang_getTokenSpelling(*CurrTU,tokens[i])));
        }
        printf(": taken %s\n", node->text);
		 */


		prev_binary_line = line;
		prev_binary_column = column;
		prev_binary_offset = offset;
		clang_disposeTokens (*CurrTU, tokens, tokens_size);
		clang_visitChildren(cursor, visit_ast, node);

		break;
	}

	case NodeKind_FunctionDecl:
	{
        CurrFunc = &cursor;
        CurrNode = &cursor;
        g_ptr_array_add(FuncLists, node);
		ASTNode_add_type(node, (NodeTypeKind) clang_getCursorResultType(cursor).kind, NULL);
        if(clang_getCursorResultType(cursor).kind == NodeTypeKind_Pointer)
        {
            gchar * p = resolve_pointer(clang_getCursorResultType(cursor));
		    ASTNode_new_with_parent(node, NodeKind_MiluSource, p, NULL);
            g_free(p);
        }


{
	CXSourceLocation loc;	
		unsigned line;
		unsigned column;
		unsigned offset;
	CXSourceRange  ran = clang_getCursorExtent (*CurrFunc);
	CXToken * tokens = NULL;
	unsigned tokens_num = 0;
	clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);

	if (tokens_num > 0)
	{
		loc = clang_getTokenLocation(*CurrTU,tokens[0]);
	}
	clang_getSpellingLocation (loc, NULL, &line, &column, &offset);
//	printf("+%u %u %u\n", line, column, offset);
	clang_disposeTokens(*CurrTU, tokens, tokens_num);
	node->line_start = line;
}



		clang_visitChildren(cursor, visit_ast, node);

		if(fix_function_ellipsis())
            ASTNode_new_with_parent(node, NodeKind_MiluSource, "...", NULL);
		if(fix_function_static())
            ASTNode_set_static(node);
        if(fix_function_pointer(node))
            reset_function_text(node);
        parsing_in_func = FALSE;
		break;
	}

	case NodeKind_ReturnStmt:
	case NodeKind_VarDecl:
	case NodeKind_ParmDecl:
	{
                CurrNode = &cursor;

//TODO fix linkage		ASTNode_set_linkage(node,clang_getCursorLinkage(cursor));
        ASTNode_set_linkage(node,0);
        if(clang_isConstQualifiedType (clang_getCursorType(cursor)))
            ASTNode_set_const(node);
        if (clang_getCursorType(cursor).kind == NodeTypeKind_Unexposed)
        {
            CXCursor tmpc = clang_getTypeDeclaration(clang_getCursorType(cursor));
            ASTNode * link_node = ASTNode_search_node_by_cx(CurrASTUnit->ast, tmpc.data[0]);

            if(link_node)

                ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, link_node);

            else
            {
                GString * tmp_string = g_string_new("");
                CXSourceRange  ran =	clang_getCursorExtent (cursor);

                CXToken * tokens = NULL;
                unsigned tokens_size = 0;
                clang_tokenize	((*CurrTU), ran, &tokens, &tokens_size);
                for(gint i = 0; i < tokens_size-1; i++)
                {
                    CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[i]);
                    char * curr_token = clang_getCString(token_cstr);
                    g_string_append_printf(tmp_string, "%s ", curr_token);
                    clang_disposeString(token_cstr);
                }

                clang_disposeTokens (*CurrTU, tokens, tokens_size);

                ASTNode_add_type(node, NodeTypeKind_Unexposed, NULL);
                ASTNodeType_set_text(node,tmp_string->str);
                g_string_free(tmp_string,TRUE);
            }
        }
        else if (clang_getCursorType(cursor).kind == NodeTypeKind_ConstantArray)
        {

            // fix type
            CXSourceLocation loc =	clang_getCursorLocation (cursor);
            CXSourceRange  ran =	clang_getCursorExtent (cursor);

            CXToken * tokens = NULL;
            unsigned tokens_size = 0;
            clang_tokenize	((*CurrTU), ran, &tokens, &tokens_size);

            ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);

            int ti = 0;
            //Todo: not good, should make if more general
            CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
            gchar * token_fix_text = clang_getCString(token_cstr);

            if(g_strcmp0(token_fix_text,"static")==0)
            {
                ASTNode_set_static(node);

                token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                token_fix_text = clang_getCString(token_cstr);

            }
            if(g_strcmp0(token_fix_text,"const")==0)
            {
                ASTNode_set_const(node);

                token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                token_fix_text = clang_getCString(token_cstr);
            }


            if(g_strcmp0(token_fix_text,"struct")==0)
            {
                token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                token_fix_text = clang_getCString(token_cstr);
                
                gchar * tt = g_strdup_printf("struct %s", token_fix_text);
                node->type->text = g_string_chunk_insert (MiluStringPool, tt);
                free(tt);
            }
            else
            {


                GString * tmp_string = g_string_new(token_fix_text);

                while (ti < tokens_size)
                {
                    token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                    token_fix_text = clang_getCString(token_cstr);

//                    printf("---%s \n", token_fix_text);
                    if( g_strcmp0(token_fix_text, "*")  == 0 )
                    {
		        g_string_append_printf(tmp_string, " %s", token_fix_text);
                    }
                    else if ( g_strcmp0(tmp_string->str, "unsigned")  == 0 &&
                        g_strcmp0(token_fix_text, "char") == 0)
                    {
		        g_string_append_printf(tmp_string, " %s", token_fix_text);
                    }
                    else
                        break;
                }

                node->type->text = g_string_chunk_insert (MiluStringPool, tmp_string->str);
                g_string_free(tmp_string, TRUE);

 //               printf("-+-%s ", node->type->text);
            }

            clang_disposeString(token_cstr);
            clang_disposeTokens (*CurrTU, tokens, tokens_size);
        }
        else if(clang_getCursorType(cursor).kind == NodeTypeKind_Pointer)
        {


            // fix type
            CXSourceLocation loc =	clang_getCursorLocation (cursor);
            CXSourceRange  ran =	clang_getCursorExtent (cursor);

            CXToken * tokens = NULL;
            unsigned tokens_size = 0;
            clang_tokenize	((*CurrTU), ran, &tokens, &tokens_size);

            ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);

            int ti = 0;
            //Todo: not good, should make if more general
            CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
            gchar * token_fix_text = clang_getCString(token_cstr);

            if(g_strcmp0(token_fix_text,"static")==0)
            {
                ASTNode_set_static(node);

                token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                token_fix_text = clang_getCString(token_cstr);

            }
            if(g_strcmp0(token_fix_text,"const")==0)
            {
                ASTNode_set_const(node);

                token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                token_fix_text = clang_getCString(token_cstr);
            }


            if(g_strcmp0(token_fix_text,"struct")==0)
            {
                token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                token_fix_text = clang_getCString(token_cstr);
                
                gchar * tt = g_strdup_printf("struct %s", token_fix_text);
                node->type->text = g_string_chunk_insert (MiluStringPool, tt);
                free(tt);
            }
/*
            else
            {


                GString * tmp_string = g_string_new(token_fix_text);

                while (ti < tokens_size)
                {
                    token_cstr = clang_getTokenSpelling(*CurrTU,tokens[ti++]);
                    token_fix_text = clang_getCString(token_cstr);

                    if( g_strcmp0(token_fix_text, "*")  == 0 )
                    {
		        g_string_append_printf(tmp_string, " %s", token_fix_text);
                    }
                    else
                        break;
                }

                node->type->text = g_string_chunk_insert (MiluStringPool, tmp_string->str);
                g_string_free(tmp_string, TRUE);

         //       printf("-+-%s ", node->type->text);
            }
*/

            clang_disposeString(token_cstr);
            clang_disposeTokens (*CurrTU, tokens, tokens_size);


            ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);
            gchar * p = resolve_pointer(clang_getCursorType(cursor));
            fflush(stdout);
//            printf ("***--%s\n", p);
            ASTNode_new_with_parent(node, NodeKind_MiluSource, p, NULL);
            g_free(p);


        }
        else
        {

            ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);
            if (clang_getCursorType(cursor).kind > 113 )
            {
                //printf("---%s %d\n",fix_decl_type(node->text), clang_getCursorType(cursor).kind);

                //                ASTNodeType_set_text(node, fix_decl_type(node->text));
                ASTNode_set_text(node,  fix_decl_type(node->text));
                node->kind = NodeKind_MiluSource; 
//            printf ("***--%s\n", "00");
            }
        }

        //TODO need to deal with same declarion with block


		g_hash_table_insert(localSymTable, node->text, ASTNodeTypeKindNames[node->type->kind]);
		//printf("-----%s %s\n", node->text, ASTNodeTypeKindNames[node->type->kind]);

                    if(fix_decl_static())
                        ASTNode_set_static(node);

                clang_visitChildren(cursor, visit_ast, node);
                break;
	}
	case NodeKind_TypedefDecl:
	case NodeKind_UnionDecl:
	case NodeKind_StructDecl:
	{
        if(cursor.kind == NodeKind_UnionDecl || cursor.kind == NodeKind_StructDecl)
        {
               g_ptr_array_add(DataTypes, node);
        }
		if (clang_getCursorType(cursor).kind == NodeTypeKind_Unexposed)
		{
			CXCursor tmpc = clang_getTypeDeclaration(clang_getCursorType(cursor));
			ASTNode * link_node = ASTNode_search_node_by_cx(CurrASTUnit->ast, tmpc.data[0]);
			ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, link_node);
		}
		else
		{
			ASTNode_add_type(node, (NodeTypeKind) clang_getCursorType(cursor).kind, NULL);
		}

		CXSourceRange  ran = clang_getCursorExtent (cursor);
		CXToken * tokens = NULL;
		unsigned tokens_num = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
		GString * tmp_string = g_string_new("");


		if (tokens_num > 0)
		{
			g_string_append_printf(tmp_string, "%s", clang_getCString( clang_getTokenSpelling(*CurrTU, tokens[0]) ));
			for (gint j = 1 ; j < tokens_num-1; j++)
			{
				g_string_append_printf(tmp_string, " %s", clang_getCString( clang_getTokenSpelling(*CurrTU, tokens[j]) ));
			}
			g_string_append_printf(tmp_string, "%s", clang_getCString( clang_getTokenSpelling(*CurrTU, tokens[tokens_num-1]) ));
		}

		if(search_curr_source_file(tmp_string->str))
			ASTNode_new_with_parent(node, NodeKind_MiluSource, tmp_string->str, NULL);

		clang_disposeTokens(*CurrTU, tokens, tokens_num);

		g_string_free(tmp_string, TRUE);

		clang_visitChildren(cursor, visit_ast, node);

		break;
	}

	case NodeKind_CharacterLiteral:
	{
		CXType ct = clang_getCursorType (cursor);
				CXSourceRange  ran = clang_getCursorExtent (cursor);
				CXToken * tokens = NULL;
				unsigned tokens_num = 0;
				clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
				GString * tmp_string = g_string_new("");

		        gboolean find_type = FALSE;


		        if (tokens_num > 0)
		        {
		        		CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[0]);
		        		const char * curr_token = clang_getCString(token_cstr);
		        		g_string_append_printf(tmp_string, "%s ", curr_token);
                        clang_disposeString(token_cstr);
		        }
				ASTNode_new_with_parent(node, NodeKind_MiluSource, tmp_string->str, NULL);

		        clang_disposeTokens(*CurrTU, tokens, tokens_num);

		        g_string_free(tmp_string, TRUE);

				clang_visitChildren(cursor, visit_ast, node);
		        break;
	}
	case NodeKind_CStyleCastExpr:
	{
		CXType ct = clang_getCursorType (cursor);
		CXSourceRange  ran = clang_getCursorExtent (cursor);
		CXToken * tokens = NULL;
		unsigned tokens_num = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
		GString * tmp_string = g_string_new("");

        gboolean find_type = FALSE;


        if (tokens_num > 0)
        {
        	for (gint j = 0 ; j < tokens_num; j++)
        	{
        		CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[j]);
        		const char * curr_token = clang_getCString(token_cstr);
                if(g_strcmp0(")", curr_token) == 0)
                {
        		clang_disposeString(token_cstr);
                    break;
                }
                if(find_type)
        		    g_string_append_printf(tmp_string, " %s", clang_getCString( clang_getTokenSpelling(*CurrTU, tokens[j]) ));

                if(g_strcmp0("(", curr_token) == 0)
                    find_type = TRUE;

        	}
        }
        //	if(search_curr_source_file(tmp_string->str))
        //			ASTNode_new_with_parent(node, NodeKind_MiluSource, tmp_string->str, NULL);
		ASTNode_new_with_parent(node, NodeKind_MiluSource, tmp_string->str, NULL);

        clang_disposeTokens(*CurrTU, tokens, tokens_num);

        g_string_free(tmp_string, TRUE);

		clang_visitChildren(cursor, visit_ast, node);
        break;
	}
	case NodeKind_UnexposedAttr:
	{
		CXSourceRange  ran = clang_getCursorExtent (cursor);
		CXToken * tokens = NULL;
		unsigned tokens_num = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
		GString * tmp_string = g_string_new("");

        gchar * att = resolve_function_attribute();

        g_string_append_printf(tmp_string, "%s ", att);

		ASTNode_new_with_parent(node, NodeKind_MiluSource, tmp_string->str, NULL);

		clang_disposeTokens(*CurrTU, tokens, tokens_num);

		g_string_free(tmp_string, TRUE);

		clang_visitChildren(cursor, visit_ast, node);
		break;
	}
	case NodeKind_MemberRefExpr:
	{
		CXSourceRange  ran = clang_getCursorExtent (cursor);
		CXToken * tokens = NULL;
		unsigned tokens_num = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);

		CXString token_cstri = clang_getTokenSpelling(*CurrTU,tokens[tokens_num - 3]);
		const char * curr_tokeni = clang_getCString(token_cstri);
		if(g_strcmp0(curr_tokeni, ".") == 0)
		{
			ASTNode_set_ext_3(node, TRUE);
		}

		clang_disposeString(token_cstri);
		clang_disposeTokens(*CurrTU, tokens, tokens_num);

		clang_visitChildren(cursor, visit_ast, node);
		break;
	}


	//	case NodeKind_MacroDefinition:
	//	case NodeKind_MacroExpansion:
	case NodeKind_MemberRef:
        case NodeKind_StmtExpr:

	case NodeKind_SwitchStmt:
	case NodeKind_DefaultStmt:
	case NodeKind_CaseStmt:
	case NodeKind_BreakStmt:
	case NodeKind_GotoStmt:
	case NodeKind_LabelStmt:
	case NodeKind_LabelRef:
	//case NodeKind_FirstAttr:
	case NodeKind_NullStmt:
	case NodeKind_TypeRef:
	case NodeKind_ArraySubscriptExpr:
	case NodeKind_DeclStmt:
	case NodeKind_FieldDecl:
	case NodeKind_IfStmt:
	case NodeKind_WhileStmt:
	case NodeKind_DoStmt:
	case NodeKind_ContinueStmt:
	case NodeKind_ForStmt:
	case NodeKind_ParenExpr:
	case NodeKind_CallExpr :
	case NodeKind_ConditionalOperator :
	case NodeKind_InitListExpr :
        case NodeKind_CompoundLiteralExpr:
		clang_visitChildren(cursor, visit_ast, node);
		break;

	case NodeKind_DeclRefExpr:
		ASTNode_add_type(node, (NodeTypeKind) 1, NULL);
		if(g_hash_table_lookup(localSymTable, node->text))
		{
			ASTNodeType_set_text(node, g_hash_table_lookup(localSymTable, node->text));
		}
		else
		{
			ASTNodeType_set_text(node, "unknown");

		}

		
		clang_visitChildren(cursor, visit_ast, node);
		
	//	printf("-%s: %s\n", node->text, g_hash_table_lookup(localSymTable, node->text));
		break;

	case NodeKind_UnexposedExpr:
	{
		//TODO: this is for fix the bug the clang which not handle size of
		if( clang_getCursorType(cursor).kind == NodeTypeKind_ULong  )
		{
			CXType ct = clang_getCursorType (cursor);
			CXSourceRange  ran = clang_getCursorExtent (cursor);
			CXToken * tokens = NULL;
			unsigned tokens_num = 0;
			clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
			GString * tmp_string = g_string_new("");

			gboolean find_type = FALSE;

                        if(tokens_num>0)
                        {
                            CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[0]);
                            const char * curr_token = clang_getCString(token_cstr);
                            if(g_strcmp0(curr_token, "sizeof") == 0)
                            {
                                int i = 0;
                                while(i < tokens_num)
                                {
                                    CXString token_cstri = clang_getTokenSpelling(*CurrTU,tokens[i]);
                                    const char * curr_tokeni = clang_getCString(token_cstri);
                        
            
                                    if(
                                        g_strcmp0(curr_tokeni, "+") == 0 ||
                                        g_strcmp0(curr_tokeni, "-") == 0 ||
//                                          g_strcmp0(curr_tokeni, "*") == 0 ||
                                        g_strcmp0(curr_tokeni, "/") == 0 ||
                                        g_strcmp0(curr_tokeni, "=") == 0 ||
                                        g_strcmp0(curr_tokeni, ";") == 0 ||
                                        g_strcmp0(curr_tokeni, "<=") == 0
                                    )
                                    {
                                        i = tokens_num;
                                        clang_disposeString(token_cstri);
                                        i++;
                                        continue;
                                    }

                                    g_string_append_printf(tmp_string , "%s ", curr_tokeni);

                                    if(g_strcmp0(curr_tokeni, ")") == 0)
                                    {
                                        i = tokens_num;
                                    }
                                    clang_disposeString(token_cstri);
                                    i++;
                                }

			// switch off the warning of testing
			g_log("Parsing warning",G_LOG_LEVEL_WARNING, "Find sizeof:  %s ", tmp_string->str);
			ASTNode_new_with_parent(node, NodeKind_MiluSource, tmp_string->str, NULL);

			}
			clang_disposeString(token_cstr);

			clang_disposeTokens(*CurrTU, tokens, tokens_num);

			g_string_free(tmp_string, TRUE);
			}
			}
		clang_visitChildren(cursor, visit_ast, node);
		break;
	}
	case NodeKind_CompoundStmt:
	{
		clang_visitChildren(cursor, visit_ast, node);
		break;
	}

	case NodeKind_FloatingLiteral:
	case NodeKind_IntegerLiteral:
	{
		//g_log("Integer",G_LOG_LEVEL_WARNING, "Unknown: %d, %s ", cursor.kind, clang_getCString(cstr));
		CXCursor tmpc = clang_getTypeDeclaration(clang_getCursorType(cursor));

		CXSourceRange  ran = clang_getCursorExtent (cursor);
		CXToken * tokens = NULL;
		unsigned tokens_num = 0;
		clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
		GString * tmp_string = g_string_new("");

		int j = 0;

		if(tokens)
			g_string_append_printf(tmp_string, "%s", clang_getCString( clang_getTokenSpelling(*CurrTU, tokens[j]) ));

		if(curr_type == NodeKind_FloatingLiteral)
		{
		ASTNode * tmp_node = ASTNode_new_with_parent(node, NodeKind_MiluSourceFloat, tmp_string->str, NULL);
		}
		else if(curr_type == NodeKind_IntegerLiteral)
		{
		ASTNode * tmp_node = ASTNode_new_with_parent(node, NodeKind_MiluSourceInteger, tmp_string->str, NULL);
		}
		else
		{
			g_assert_not_reached();
		}
		clang_disposeTokens(*CurrTU, tokens, tokens_num);
		g_string_free(tmp_string, TRUE);

		clang_visitChildren(cursor, visit_ast, node);
		break;
	}
/*	case NodeKind_AsmLabelAttr:
	{
		clang_visitChildren(cursor, visit_ast, node);
		g_log("Parsing error",G_LOG_LEVEL_WARNING, "Unknown: %d, %s ", cursor.kind, clang_getCString(cstr));
		break;
	}
	*/
	//TODO : 5 may bee to be fixed.
	case 407:
	case 5:
			break;
	default:
	{
		CXString cstr = clang_getCursorSpelling(cursor);
		//TODO:fix parsing
		g_log("Parsing error",G_LOG_LEVEL_WARNING, "Unknown: %d, %s ", cursor.kind, clang_getCString(cstr));

		//exit(0);
		break;
	}
	}
	//TODO fix return value
	return  CXChildVisit_Continue;
}

void ASTUnit_print(ASTUnit * au, PrintMode mode, FILE * output)
{
	ASTNode  * ast = au->ast;

	switch(mode)
	{
	case PrintDot:
	{
		ASTNode_print_dot(ast, output);
		break;
	}
	case PrintSource:
	{
		ASTNode_print_source(ast, output);
		break;
	}
	case PrintHtml:
	{
		ASTNode_print_html(ast, output);
		break;
	}

	default:
	{

		g_log ("Milu",G_LOG_LEVEL_WARNING,"Unknown print mode") ;

		break;
	}
	}
}


static gint check_binary_op(const char * op)
{
    if (g_strcmp0(op, "*") ==0||
            g_strcmp0(op, "/") ==0||
            g_strcmp0(op, "%") == 0)
        return 5;
    if (g_strcmp0(op, "+") == 0|| 
            g_strcmp0(op, "-") == 0)
        return 6;
    if (g_strcmp0(op, ">>") == 0 ||
            g_strcmp0(op, "<<") == 0)
        return 7;
    if (g_strcmp0(op,">") ==0 ||
            g_strcmp0(op,">=") ==0 ||
            g_strcmp0(op,"<") ==0 ||
            g_strcmp0(op,"<=") ==0)
        return 8;
    if (g_strcmp0(op, "==") == 0|| 
            g_strcmp0(op, "!=") == 0)
        return 9;
    if (g_strcmp0(op, "&") == 0)
        return 10;
    if (g_strcmp0(op, "^") == 0)
        return 11;
    if (g_strcmp0(op, "|") == 0)
        return 12;
    if (g_strcmp0(op, "&&") == 0)
        return 13;
    if (g_strcmp0(op, "||") == 0)
        return 14;
    if ( g_strcmp0(op, "+=") == 0 ||
            g_strcmp0(op, "-=") == 0 ||
            g_strcmp0(op, "*=") == 0 ||
            g_strcmp0(op, "/=") == 0 ||
            g_strcmp0(op, "%=") == 0 ||
            g_strcmp0(op, "<<=") == 0 ||
            g_strcmp0(op, ">>=") == 0 ||
            g_strcmp0(op, "&=") == 0 ||
            g_strcmp0(op, "^=") == 0 ||
            g_strcmp0(op, "|=") == 0 ||
            g_strcmp0(op, "=") == 0 )
        return 15;
    if (g_strcmp0(op, ",") == 0)
        return 17;

    return 0;
}

static gchar * fix_binary_op(CXToken * tokens ,unsigned tokens_size)
{
    gchar * big_op = NULL;
    gint big_weight = -819;
    gint skip_par = 0;
    gint skip_spar = 0;
    for (gint i = tokens_size-2; i >=0; i--)
    {
        CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[i]);
        const char * curr_token = clang_getCString(token_cstr);

        if (g_strcmp0(curr_token,")") ==0)
            skip_par++;
        if (g_strcmp0(curr_token,"(") ==0)
            skip_par--;

        if (g_strcmp0(curr_token,"]") ==0)
            skip_spar++;
        if (g_strcmp0(curr_token,"[") ==0)
            skip_spar--;

        if(skip_par == 0 && skip_spar == 0)
        {
            gint curr_token_weight = check_binary_op(curr_token);
            if(curr_token_weight)
            {
                if (curr_token_weight == 10)
                {
                    if ((i-1) >= 0)
                    {
                        CXString tmp_token_cstr = clang_getTokenSpelling(*CurrTU,tokens[i-1]);
                        const char * tmp_curr_token = clang_getCString(tmp_token_cstr);
                        gint tmp_curr_token_weight = check_binary_op(tmp_curr_token);
                        clang_disposeString(tmp_token_cstr);
                        if(tmp_curr_token_weight)
                        {
                            continue;
                        }

                    }
                }

//                printf("currweight: %d\n", curr_token_weight);

                if (curr_token_weight > big_weight)
                {
                    // fix for unary -
                    if (!(curr_token_weight == 6 && i == 0))
                    {                    
                    if (big_op)
                        g_free(big_op);
                    big_op = g_strdup(curr_token);
                    big_weight = curr_token_weight;
                    }
                }
            }
        }
        clang_disposeString(token_cstr);
    }

 //   printf("op: %s\n", big_op);
    return big_op;
}

static gboolean search_curr_source_file(gchar * text)
{

    //	if(g_strstr_len (curr_src, -1, text))
//		return TRUE;
    if (g_strstr_len (text, -1, "__va_list"))
	return FALSE;
    return TRUE;
}
static void load_c_source_file(const gchar * path)
{
	g_assert(path && "Invalid arguments!");
	gchar * contents;
	gsize len;
	if(!g_file_get_contents(path, &contents, &len, NULL))
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot load the test inputs file.") ;
	}

	curr_src = contents;
}

static gchar * resolve_pointer(CXType type)
{
    GString * pointer = g_string_new("");
    CXType curr_type = type;
    while(curr_type.kind == NodeTypeKind_Pointer)
    {
		curr_type = clang_getPointeeType ( curr_type);
        g_string_append_printf(pointer," *");

    }
//	CXString cstr = clang_getTypeKindSpelling (curr_type.kind);
    if(curr_type.kind != NodeTypeKind_Unexposed && curr_type.kind != NodeTypeKind_Typedef)
	g_string_prepend(pointer, ASTNodeTypeKindNames[curr_type.kind]);
//    clang_disposeString(cstr);

    return g_string_free(pointer, FALSE);
}

static gboolean is_unary_token(const gchar * token)
{
	if(g_strcmp0(token, "++")==0)
	    return TRUE;
	else if(g_strcmp0(token, "--")==0)
	    return TRUE;
	else if(g_strcmp0(token, "!")==0)
	    return TRUE;
	else if(g_strcmp0(token, "*")==0)
	    return TRUE;
	else if(g_strcmp0(token, "&")==0)
	    return TRUE;
    else if(g_strcmp0(token, "-")==0)
        return TRUE;
    else if(g_strcmp0(token, "~")==0)
        return TRUE;
    else
	    return FALSE;

}
static gint get_unary_operator_id(CXToken * tokens ,unsigned tokens_size)
{
	for (gint i = 0; i < tokens_size; i++)
	{
		CXString token_cstr = clang_getTokenSpelling(*CurrTU,tokens[i]);
		const char * curr_token = clang_getCString(token_cstr);

     //    g_printf(curr_token);
    //    g_printf(" ");
		if(is_unary_token(curr_token))
		{
   //     g_printf("\n");
            return i;
		}
		clang_disposeString(token_cstr);
	}
  //      g_printf("\n");

	//TODO: This should be switch on for most of time, but may cos unknown stop
//    g_assert_not_reached();
            return 0;
}

static void add_original_non_mutation(ASTNode * ast)
{
	ASTNode * child = ast->children;
	int curr_line= 1;


	gchar * contents;
	gsize len;
	if(!g_file_get_contents(milu_options_get_src_path(), &contents, &len, NULL))
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot load the inputs file. %s", milu_options_get_src_path()) ;
	}
	gchar ** inputs  = g_strsplit (contents, "\n", -1);
	gchar ** curr_input = inputs;

    GString * buffer = g_string_sized_new((gsize)102400);
    gint flen = 0;
    while(*curr_input)
    {
//    	g_string_append_printf(buffer,"%s\n",  *curr_input);
    	flen++;
    	curr_input++;
    }


  //  printf("%d-\n",flen);

    int i = 0 ;
	while(child)
	{
		//printf("c: %s", child->text);
		//fflush(stdout);
		if(child->line_start > curr_line)
		{
			g_string_printf(buffer,"%s", "");
			for(int j = curr_line-1; j < child->line_start-1; j++)
			{
				g_string_append_printf(buffer,"%s\n",  *(inputs+j));
			}
			ASTNode * node = ASTNode_new_milu_src_node(buffer->str);
			ASTNode_insert_before(child, node);
//			printf("--  ");
//			printf("%d, %d\n", curr_line , child->line_start-1);
//			printf("%s",buffer);
			//printf("i: %d\n ", i);
			curr_line = child->line_end+1;
		}

	//	printf("-i: %d %s\n ", i, child->text);
	//	printf("%d, %d\n", child->line_start, child->line_end);
		curr_line = child->line_end+1;

		if(!child->next_sibling )
			break;
		child = child->next_sibling;
		i++;

	}

	if(child->line_end+1 < flen)
	{
		g_string_printf(buffer,"%s", "");
		for(int j = child->line_end; j < flen; j++)
		{
			g_string_append_printf(buffer,"%s\n",  *(inputs+j));
		}

		//		printf("--");
		//printf("%d, %d\n", child->line_end+1, flen);
		//		printf("%s",buffer);
		ASTNode * node = ASTNode_new_milu_src_node(buffer->str);
		ASTNode_append_child(child->parent, node);

	}
	g_strfreev (inputs);
    g_free(contents);
}

static void clean_ast(ASTNode * root)
{
    fix_data_types_duplication();
    remove_function_signiture(root);
}

static void remove_function_signiture(ASTNode * root)
{
	ASTNode * child = root->children;
	ASTNode * tmp = NULL;
	while(child)
	{
		if(!is_ASTNode_func_decl_with_body(child))
		{
			tmp = child;
			child = child->next_sibling;
			ASTNode_unlink(tmp);
			ASTNode_free(tmp);
		}
		else
		child = child->next_sibling;
	}
}

// some function contains unexposed attr which are redundant 
// libclang dependent
static void fix_function_attribute()
{
    for(gint i = 0 ; i < FuncLists-> len ; i ++)
    {
    gboolean fixed = FALSE;
    	ASTNode * curr_func = g_ptr_array_index(FuncLists, i);
        while (ASTNode_search_children(curr_func, NodeKind_UnexposedAttr))
        {
            ASTNode * last_attr_node = ASTNode_get_nth_child(curr_func, ASTNode_get_children_number(curr_func));
            if(last_attr_node->kind == NodeKind_UnexposedAttr)
            {
        				ASTNode_unlink(last_attr_node);
                         continue;
           }

            
        	ASTNode * curr_attr_node = curr_func->children;

        	while(curr_attr_node)
        	{

        		if(curr_attr_node->kind == NodeKind_UnexposedAttr)
        		{
        			if(fixed && curr_attr_node->next_sibling != NULL)
        			{

        				ASTNode * unode = curr_attr_node;
        				curr_attr_node = curr_attr_node->next_sibling;
        				ASTNode_unlink(unode);
        				//TODO:Free unode
        			}
        			else
        			{
        				ASTNode * unode = curr_attr_node;
        				curr_attr_node = curr_attr_node->next_sibling;
        				while(unode->next_sibling)
        				{
        					ASTNode_swap_nodes(unode, unode->next_sibling);
        				}
        				fixed = TRUE;
        			}
        		}
        		else
        			curr_attr_node = curr_attr_node->next_sibling;
        	}
        }
    }
}
static gchar * resolve_function_attribute()
{
	CXSourceRange  ran = clang_getCursorExtent (*CurrFunc);
	CXToken * tokens = NULL;
	unsigned tokens_num = 0;
	clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
	GString * tmp_string = g_string_new("");

    GString * buff = g_string_new("");
    gboolean found = FALSE;
	if (tokens_num > 0)
	{
		for (gint j = 0 ; j < tokens_num; j++)
		{
			CXString curr_token = clang_getTokenSpelling(*CurrTU, tokens[j]) ;
            const gchar * curr_str = clang_getCString(curr_token);
            if(g_strcmp0("__asm", curr_str) == 0)
            {
                g_string_append(buff, "__asm ");
                found = TRUE;
            }

            else if(g_strcmp0("__attribute__", curr_str) == 0)
            {
                g_string_append(buff, "__attribute__ ");
                found = TRUE;
            }
            else if(g_strcmp0(";", curr_str) == 0)
            {
                found = FALSE;
            }
            else
            {
            	if(found)
                    g_string_append_printf(buff, "%s ", curr_str);
            }
           clang_disposeString(curr_token);
		}
	}

	clang_disposeTokens(*CurrTU, tokens, tokens_num);

                return g_string_free(buff, FALSE);
}

static void fix_data_types_duplication()
{
    for(gint i = 0 ; i < DataTypes->len; i++)
    {
	    ASTNode * node = g_ptr_array_index(DataTypes, i);
	    for(gint j = i+1 ; j < DataTypes->len; j++)
	       {
	   	    ASTNode * curr_node = g_ptr_array_index(DataTypes, j);
               if(g_strcmp0(node->text, curr_node->text) == 0)
               {
                   if(node->parent && node->parent->kind == NodeKind_TypedefDecl)
                   {
                        ASTNode_unlink (curr_node);
                        //TODO free node
                   }
                   else if(curr_node->parent && curr_node->parent->kind == NodeKind_TypedefDecl)
                   {
                        ASTNode_unlink (node);
                        //TODO free node
                   }
                   else if (ASTNode_search_children (node, NodeKind_FieldDecl))
                   {
                        ASTNode_unlink (curr_node);
                        //TODO free node
                   }
                   else if (ASTNode_search_children (curr_node, NodeKind_FieldDecl))
                   {
                        ASTNode_unlink (node);
                        //TODO free node
                   }
                   else
                   {
                	          ;
                   }
    	          // printf("%s\n", curr_node->text);
               }
	       }
    }
}


static gboolean fix_function_ellipsis()
{
	CXSourceRange  ran = clang_getCursorExtent (*CurrFunc);
	CXToken * tokens = NULL;
	unsigned tokens_num = 0;
	clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
	GString * tmp_string = g_string_new("");

	if (tokens_num > 0)
	{
		for (gint j = 0 ; j < tokens_num; j++)
		{
			CXString curr_token = clang_getTokenSpelling(*CurrTU, tokens[j]) ;
            const gchar * curr_str = clang_getCString(curr_token);

            if(g_strcmp0("...", curr_str) == 0)
            {
                return TRUE;
//            	ASTNode_new_with_parent(*CurrFunc, NodeKind_MiluSource, "...", NULL);
            }
           clang_disposeString(curr_token);
		}
	}

	clang_disposeTokens(*CurrTU, tokens, tokens_num);

	return FALSE;
}

static gboolean fix_function_static()
{
    CXSourceRange  ran = clang_getCursorExtent (*CurrFunc);
    CXToken * tokens = NULL;
    unsigned tokens_num = 0;
    clang_tokenize  ((*CurrTU), ran, &tokens, &tokens_num);
    GString * tmp_string = g_string_new("");

    if (tokens_num > 0)
    {
        for (gint j = 0 ; j < tokens_num; j++)
        {
            CXString curr_token = clang_getTokenSpelling(*CurrTU, tokens[j]) ;
            const gchar * curr_str = clang_getCString(curr_token);

       //     printf("%d ", tokens_num);
//            printf("%s ", curr_str);

            if(g_strcmp0("static", curr_str) == 0)
            {
                return TRUE;
//              ASTNode_new_with_parent(*CurrFunc, NodeKind_MiluSource, "...", NULL);
            }
            if(j > 5)
            {
                // a quick fix
                return FALSE;
            }
           clang_disposeString(curr_token);
        }
    }

    clang_disposeTokens(*CurrTU, tokens, tokens_num);

    return FALSE;
}

static gboolean fix_decl_static()
{
	CXSourceRange  ran = clang_getCursorExtent (*CurrNode);
	CXToken * tokens = NULL;
	unsigned tokens_num = 0;
	clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
	GString * tmp_string = g_string_new("");
	if (tokens_num > 0)
	{
		for (gint j = 0 ; j < tokens_num; j++)
		{
			CXString curr_token = clang_getTokenSpelling(*CurrTU, tokens[j]) ;
            const gchar * curr_str = clang_getCString(curr_token);
            //printf("-%d ", tokens_num);
            //printf("-%s ", curr_str);

            if(g_strcmp0("static", curr_str) == 0)
            {
                return TRUE;
            }
            if(g_strcmp0("{", curr_str) == 0)
            {
                return FALSE;
            }
            if(j > 5)
            {
            	return FALSE;
            }
           clang_disposeString(curr_token);
	    }
	}
	clang_disposeTokens(*CurrTU, tokens, tokens_num);

	return FALSE;
}

static gchar * fix_decl_type(gchar * var)
{
	CXSourceRange  ran = clang_getCursorExtent (*CurrNode);
	CXToken * tokens = NULL;
	unsigned tokens_num = 0;
	clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
	GString * tmp_string = g_string_new("");
        GString * buff = g_string_new("");

	if (tokens_num > 0)
	{
		for (gint j = 0 ; j < tokens_num; j++)
		{
			CXString curr_token = clang_getTokenSpelling(*CurrTU, tokens[j]) ;
            const gchar * curr_str = clang_getCString(curr_token);

//            printf("%d ", tokens_num);
 //           printf("%s ", curr_str);

            if (    g_strcmp0(";", curr_str) == 0 )
            {
                g_string_append(buff, curr_str);
                clang_disposeString(curr_token);
                break;
            }
            if (g_strcmp0(")", curr_str) == 0)
            {
                clang_disposeString(curr_token);
                break;
            }
            
            g_string_append(buff, curr_str);
            g_string_append(buff, " ");

            clang_disposeString(curr_token);
	    }
	}

	clang_disposeTokens(*CurrTU, tokens, tokens_num);

        return g_string_free(buff, FALSE);
}

static gboolean fix_function_pointer(ASTNode * func)
{
    ASTNode * node = func->children;
    if(node)
    {
	while(node->next_sibling)
	{
        if(ASTNode_check_type_kind(node, NodeTypeKind_Pointer))
        {
             if(ASTNode_search_children_type(node, NodeTypeKind_Pointer))
             {
            	 //g_printf("Found func pointer\n");
                 return TRUE;
             }
        }
		node = node->next_sibling;
	}
    }

    return FALSE;
}

static gboolean reset_function_text(ASTNode * func)
{
	CXSourceRange  ran = clang_getCursorExtent (*CurrFunc);
	CXToken * tokens = NULL;
	unsigned tokens_num = 0;
	clang_tokenize	((*CurrTU), ran, &tokens, &tokens_num);
	GString * tmp_string = g_string_new("");

	if (tokens_num > 0)
	{
		for (gint j = 0 ; j < tokens_num-1; j++)
		{
			CXString curr_token = clang_getTokenSpelling(*CurrTU, tokens[j]) ;
            const gchar * curr_str = clang_getCString(curr_token);

	        g_string_append_printf(tmp_string, "%s ", curr_str);
            clang_disposeString(curr_token);
		}
	}

    ASTNode_set_text(func,tmp_string->str);
    func->kind = NodeKind_MiluSource;

	clang_disposeTokens(*CurrTU, tokens, tokens_num);
    g_string_free(tmp_string, TRUE);

	return FALSE;

}

GPtrArray * ASTUnit_get_current_func_list()
{
	return FuncLists;
}
