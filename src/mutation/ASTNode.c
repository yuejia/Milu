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

#include "ASTNode.h"

// -----------init
gchar * ASTNodeKindNames[1024] ;
gchar * ASTNodeTypeKindNames[1024] ;
extern  GStringChunk *  MiluStringPool;
static gsize ASTNode_ID = 1;

void ASTNode_init()
{
	ASTNodeKindNames_init();
	ASTNodeTypeKindNames_init();
}

void ASTNodeTypeKindNames_init()
{
	 ASTNodeTypeKindNames[0] =   "Invalid";
	 ASTNodeTypeKindNames[1] =  "Unexposed";
	 ASTNodeTypeKindNames[2] =  "void";
	 ASTNodeTypeKindNames[3] =  "Bool";
	 ASTNodeTypeKindNames[4] =   "Char_U";
	 ASTNodeTypeKindNames[5] =  "unsigned char";
	 ASTNodeTypeKindNames[6] =  "char16";
	 ASTNodeTypeKindNames[7] =  "char32";
	 ASTNodeTypeKindNames[8] =   "unsigned short";
	 ASTNodeTypeKindNames[9] =  "unsigned int";
	 ASTNodeTypeKindNames[10] =  "unsigned long";
	 ASTNodeTypeKindNames[11] =  "unsigned long long";
	 ASTNodeTypeKindNames[12] =   "uint128";
	 ASTNodeTypeKindNames[13] =  "char";//"Char_S";
	 ASTNodeTypeKindNames[14] =  "SChar";
	 ASTNodeTypeKindNames[15] =  "WChar";
	 ASTNodeTypeKindNames[16] =   "short";
	 ASTNodeTypeKindNames[17] =  "int";
	 ASTNodeTypeKindNames[18] =  "long";
	 ASTNodeTypeKindNames[19] =  "long long";
	 ASTNodeTypeKindNames[20] =   "int128";
	 ASTNodeTypeKindNames[21] =  "float";
	 ASTNodeTypeKindNames[22] =  "double";
	 ASTNodeTypeKindNames[23] =  "long double";
	 ASTNodeTypeKindNames[24] =   "NullPtr";
	 ASTNodeTypeKindNames[25] =  "Overload";
	 ASTNodeTypeKindNames[26] =  "Dependent";
	 ASTNodeTypeKindNames[27] =  "ObjCId";
	 ASTNodeTypeKindNames[28] =   "ObjCClass";
	 ASTNodeTypeKindNames[29] =  "ObjCSel";
//	 ASTNodeTypeKindNames[Void] =  "FirstBuiltin";
//	 ASTNodeTypeKindNames[ObjCSel] =  "LastBuiltin";
	 ASTNodeTypeKindNames[100] =   "Complex";
	 ASTNodeTypeKindNames[101] =  "Pointer";
	 ASTNodeTypeKindNames[102] =  "BlockPointer";
	 ASTNodeTypeKindNames[103] =  "LValueReference";
	 ASTNodeTypeKindNames[104] =   "RValueReference";
	 ASTNodeTypeKindNames[105] =  "Record";
	 ASTNodeTypeKindNames[106] =  "Enum";
	 ASTNodeTypeKindNames[107] =  "Typedef";
	 ASTNodeTypeKindNames[108] =   "ObjCInterface";
	 ASTNodeTypeKindNames[109] =  "ObjCObjectPointer";
	 ASTNodeTypeKindNames[110] =  "FunctionNoProto";
	 ASTNodeTypeKindNames[111] =  "FunctionProto";
	 ASTNodeTypeKindNames[112] =   "ConstantArray";
}
void ASTNodeKindNames_init()
{
	ASTNodeKindNames[0] = "";
	ASTNodeKindNames[1] = "UnexposedDecl";
	ASTNodeKindNames[2] = "StructDecl";
	ASTNodeKindNames[3] = "UnionDecl";
	ASTNodeKindNames[4] = "ClassDecl";
	ASTNodeKindNames[5] = "EnumDecl ";
	ASTNodeKindNames[6] = "FieldDecl";
	 ASTNodeKindNames[7] =  "EnumConstantDecl";
	 ASTNodeKindNames[8] =  "FunctionDecl";
	 ASTNodeKindNames[9] =  "VarDecl";
	 ASTNodeKindNames[10] =  "ParmDecl";
	 ASTNodeKindNames[11] =  "ObjCInterfaceDecl";
	 ASTNodeKindNames[12] =  "ObjCCategoryDecl";
	 ASTNodeKindNames[13] =  "ObjCProtocolDecl";
	 ASTNodeKindNames[14] =  "ObjCPropertyDecl";
	 ASTNodeKindNames[15] =  "ObjCIvarDecl";
	 ASTNodeKindNames[16] =  "ObjCInstanceMethodDecl";
	 ASTNodeKindNames[17] =  "ObjCClassMethodDecl";
	 ASTNodeKindNames[18] =  "ObjCImplementationDecl";
	 ASTNodeKindNames[19] =  "ObjCCategoryImplDecl";
	 ASTNodeKindNames[20] =  "TypedefDecl";
	 ASTNodeKindNames[21] =  "CXXMethod";
	 ASTNodeKindNames[22] =  "Namespace";
	 ASTNodeKindNames[23] =  "LinkageSpec";
	 ASTNodeKindNames[24] =  "Constructor";
	 ASTNodeKindNames[25] =  "Destructor";
	 ASTNodeKindNames[26] =  "ConversionFunction";
	 ASTNodeKindNames[27] =  "TemplateTypeParameter";
	 ASTNodeKindNames[28] =  "NonTypeTemplateParameter";
	 ASTNodeKindNames[29] =  "TemplateTemplateParameter";
	 ASTNodeKindNames[30] =  "FunctionTemplate";
	 ASTNodeKindNames[31] =  "ClassTemplate";
	 ASTNodeKindNames[32] =  "ClassTemplatePartialSpecialization";
	 ASTNodeKindNames[33] =  "NamespaceAlias";
	 ASTNodeKindNames[34] =  "UsingDirective";
	 ASTNodeKindNames[35] =  "UsingDeclaration";
	 ASTNodeKindNames[36] =  "TypeAliasDecl";
	 ASTNodeKindNames[37] =  "ObjCSynthesizeDecl";
	 ASTNodeKindNames[38] =  "ObjCDynamicDecl";
	 ASTNodeKindNames[39] =  "CXXAccessSpecifier";
//	 ASTNodeKindNames[UnexposedDecl] =  "FirstDecl";
//	 ASTNodeKindNames[CXXAccessSpecifier] =  "LastDecl";
	 ASTNodeKindNames[40] =  "FirstRef";
	 ASTNodeKindNames[40] =  "ObjCSuperClassRef";
	 ASTNodeKindNames[41] =  "ObjCProtocolRef";
	 ASTNodeKindNames[42] =  "ObjCClassRef";
	 ASTNodeKindNames[43] =  "TypeRef";
	 ASTNodeKindNames[44] =  "CXXBaseSpecifier";
	 ASTNodeKindNames[45] =  "TemplateRef";
	 ASTNodeKindNames[46] =  "NamespaceRef";
	 ASTNodeKindNames[47] =  "MemberRef";
	 ASTNodeKindNames[48] =  "LabelRef";
	 ASTNodeKindNames[49] =  "OverloadedDeclRef";
//	 ASTNodeKindNames[OverloadedDeclRef] =  "LastRef";
	 ASTNodeKindNames[70] =  "FirstInvalid";
	 ASTNodeKindNames[70] =  "InvalidFile";
	 ASTNodeKindNames[71] =  "NoDeclFound";
	 ASTNodeKindNames[72] =  "NotImplemented";
	 ASTNodeKindNames[73] =  "InvalidCode";
//	 ASTNodeKindNames[InvalidCode] =  "LastInvalid";
	 ASTNodeKindNames[100] =  "FirstExpr";
	 ASTNodeKindNames[100] =  "UnexposedExpr";
	 ASTNodeKindNames[101] =  "DeclRefExpr";
	 ASTNodeKindNames[102] =  "MemberRefExpr";
	 ASTNodeKindNames[103] =  "CallExpr";
	 ASTNodeKindNames[104] =  "ObjCMessageExpr";
	 ASTNodeKindNames[105] =  "BlockExpr";
	 ASTNodeKindNames[106] =  "IntegerLiteral";
	 ASTNodeKindNames[107] =  "FloatingLiteral";
	 ASTNodeKindNames[108] =  "ImaginaryLiteral";
	 ASTNodeKindNames[109] =  "StringLiteral";
	 ASTNodeKindNames[110] =  "CharacterLiteral";
	 ASTNodeKindNames[111] =  "ParenExpr";
	 ASTNodeKindNames[112] =  "UnaryOperator";
	 ASTNodeKindNames[113] =  "ArraySubscriptExpr";
	 ASTNodeKindNames[114] =  "BinaryOperator";
	 ASTNodeKindNames[115] =  "CompoundAssignOperator";
	 ASTNodeKindNames[116] =  "ConditionalOperator";
	 ASTNodeKindNames[117] =  "CStyleCastExpr";
	 ASTNodeKindNames[118] =  "CompoundLiteralExpr";
	 ASTNodeKindNames[119] =  "InitListExpr";
	 ASTNodeKindNames[120] =  "AddrLabelExpr";
	 ASTNodeKindNames[121] =  "StmtExpr";
	 ASTNodeKindNames[122] =  "GenericSelectionExpr";
	 ASTNodeKindNames[123] =  "GNUNullExpr";
	 ASTNodeKindNames[124] =  "CXXStaticCastExpr";
	 ASTNodeKindNames[125] =  "CXXDynamicCastExpr";
	 ASTNodeKindNames[126] =  "CXXReinterpretCastExpr";
	 ASTNodeKindNames[127] =  "CXXConstCastExpr";
	 ASTNodeKindNames[128] =  "CXXFunctionalCastExpr";
	 ASTNodeKindNames[129] =  "CXXTypeidExpr";
	 ASTNodeKindNames[130] =  "CXXBoolLiteralExpr";
	 ASTNodeKindNames[131] =  "CXXNullPtrLiteralExpr";
	 ASTNodeKindNames[132] =  "CXXThisExpr";
	 ASTNodeKindNames[133] =  "CXXThrowExpr";
	 ASTNodeKindNames[134] =  "CXXNewExpr";
	 ASTNodeKindNames[135] =  "CXXDeleteExpr";
	 ASTNodeKindNames[136] =  "UnaryExpr";
	 ASTNodeKindNames[137] =  "ObjCStringLiteral";
	 ASTNodeKindNames[138] =  "ObjCEncodeExpr";
	 ASTNodeKindNames[139] =  "ObjCSelectorExpr";
	 ASTNodeKindNames[140] =  "ObjCProtocolExpr";
	 ASTNodeKindNames[141] =  "ObjCBridgedCastExpr";
	 ASTNodeKindNames[142] =  "PackExpansionExpr";
	 ASTNodeKindNames[143] =  "SizeOfPackExpr";
//	 ASTNodeKindNames[SizeOfPackExpr] =  "LastExpr";
	 ASTNodeKindNames[200] =  "FirstStmt";
	 ASTNodeKindNames[200] =  "UnexposedStmt";
	 ASTNodeKindNames[201] =  "LabelStmt";
	 ASTNodeKindNames[202] =  "CompoundStmt";
	 ASTNodeKindNames[203] =  "CaseStmt";
	 ASTNodeKindNames[204] =  "DefaultStmt";
	 ASTNodeKindNames[205] =  "IfStmt";
	 ASTNodeKindNames[206] =  "SwitchStmt";
	 ASTNodeKindNames[207] =  "WhileStmt";
	 ASTNodeKindNames[208] =  "DoStmt";
	 ASTNodeKindNames[209] =  "ForStmt";
	 ASTNodeKindNames[210] =  "GotoStmt";
	 ASTNodeKindNames[211] =  "IndirectGotoStmt";
	 ASTNodeKindNames[212] =  "ContinueStmt";
	 ASTNodeKindNames[213] =  "BreakStmt";
	 ASTNodeKindNames[214] =  "ReturnStmt";
	 ASTNodeKindNames[215] =  "AsmStmt";
	 ASTNodeKindNames[216] =  "ObjCAtTryStmt";
	 ASTNodeKindNames[217] =  "ObjCAtCatchStmt";
	 ASTNodeKindNames[218] =  "ObjCAtFinallyStmt";
	 ASTNodeKindNames[219] =  "ObjCAtThrowStmt";
	 ASTNodeKindNames[220] =  "ObjCAtSynchronizedStmt";
	 ASTNodeKindNames[221] =  "ObjCAutoreleasePoolStmt";
	 ASTNodeKindNames[222] =  "ObjCForCollectionStmt";
	 ASTNodeKindNames[223] =  "CXXCatchStmt";
	 ASTNodeKindNames[224] =  "CXXTryStmt";
	 ASTNodeKindNames[225] =  "CXXForRangeStmt";
	 ASTNodeKindNames[226] =  "SEHTryStmt";
	 ASTNodeKindNames[227] =  "SEHExceptStmt";
	 ASTNodeKindNames[228] =  "SEHFinallyStmt";
	 ASTNodeKindNames[230] =  "NullStmt";
	 ASTNodeKindNames[231] =  "DeclStmt";
//	 ASTNodeKindNames[DeclStmt] =  "LastStmt";
	 ASTNodeKindNames[300] =  "TranslationUnit";
	 ASTNodeKindNames[400] =  "FirstAttr";
	 ASTNodeKindNames[400] =  "UnexposedAttr";
	 ASTNodeKindNames[401] =  "IBActionAttr";
	 ASTNodeKindNames[402] =  "IBOutletAttr";
	 ASTNodeKindNames[403] =  "IBOutletCollectionAttr";
	 ASTNodeKindNames[404] =  "CXXFinalAttr";
	 ASTNodeKindNames[405] =  "CXXOverrideAttr";
	 ASTNodeKindNames[406] =  "AnnotateAttr";
//	 ASTNodeKindNames[AnnotateAttr] =  "LastAttr";
	 ASTNodeKindNames[500] =  "PreprocessingDirective";
	 ASTNodeKindNames[501] =  "MacroDefinition";
	 ASTNodeKindNames[502] =  "MacroExpansion";
//	 ASTNodeKindNames[MacroExpansion] =  "MacroInstantiation";
	 ASTNodeKindNames[503] =  "InclusionDirective";
//	 ASTNodeKindNames[PreprocessingDirective] =  "FirstPreprocessing";
//	 ASTNodeKindNames[InclusionDirective ] =  "LastPreprocessing";
	 ASTNodeKindNames[700] = "MiluSource";
	 ASTNodeKindNames[701] = "MiluSourceInteger";
	 ASTNodeKindNames[702] = "NodeKind_UnaryOperator_Append";
	 ASTNodeKindNames[703] = "NodeKind_MiluSourceFloat";
}

ASTNode * ASTNode_new_with_parent(ASTNode * parent, NodeKind kind, gchar * text, gpointer cx)
{
    g_assert(parent);
	ASTNode * node = ASTNode_new(kind, text, cx);

	if (parent->children)
	{
		ASTNode * sibling = parent->children;
		while (sibling->next_sibling)
			sibling = sibling->next_sibling;
		node->prev_sibling = sibling;
		sibling->next_sibling = node;
	}
	else
		parent->children = node;


	node->parent = parent;
	return node;
}


void ASTNodeType_set_text(ASTNode * node, gchar * text)
{
    g_assert(node && text && "Invalid args!");
	node->type->text = g_string_chunk_insert (MiluStringPool, text);
}

void ASTNodeType_save_original_kind(ASTNode * node)
{
    g_assert(node && "Invalid args!");
	node->ori_kind = node->kind;
}

void ASTNodeType_restore_original_kind(ASTNode * node)
{
    g_assert(node && "Invalid args!");
    node->kind = node->ori_kind;
}


void ASTNodeType_save_original_text(ASTNode * node)
{
    g_assert(node && "Invalid args!");
    if(!node->ori_text)
	node->ori_text = g_string_chunk_insert (MiluStringPool, node->text);
}

void ASTNodeType_restore_original_text(ASTNode * node)
{
    g_assert(node && "Invalid args!");
	node->text = g_string_chunk_insert (MiluStringPool, node->ori_text);
	node->ori_text = NULL;
}


ASTNode * ASTNode_new(NodeKind kind, gchar * text, gpointer cx)
{
	ASTNode * node = g_slice_new0(ASTNode);
	node->id = ASTNode_ID++;
	node->kind = kind;
	node->type = NULL;
	if(text)
		node->text = g_string_chunk_insert (MiluStringPool, text) ;
	else
		node->text = g_strdup("");
	//node->text = text ? g_string_chunk_insert (MiluStringPool, text) : NULL ;
	node->children = NULL;
	node->next_sibling = NULL;
	node->parent = NULL;
	node->prev_sibling = NULL;
	node->cx = cx;
    node->is_mutant = FALSE;
    node->ori_text = NULL;
    node->ext[0] = 0;
    node->ext[1] = 0;
    node->ext[2] = 0;
	return node;
}

void ASTNode_set_text(ASTNode * node, gchar * text)
{
	node->text = text ? g_string_chunk_insert (MiluStringPool, text) : NULL ;
}

static gboolean ASTNode_free_(ASTNode *node, gpointer data)
{
	ASTNodeType_free(node->type);
	g_slice_free (ASTNode, node);
	return TRUE;
}

void ASTNode_clean_link(ASTNode * node)
{
	node->parent = NULL;
	node->next_sibling = NULL;
	node->prev_sibling = NULL;
	node->children = NULL;
}

void ASTNode_free(ASTNode * node)
{
	// TODO replace broken implementation
	// this function deletes the given node only,
	// it does not delete the children
	// see ASTNode_free_tree() for correct implementation
    parse_tree_node_traverse_pre_order(node, ASTNode_free_, NULL);
}

static gboolean ASTNode_free_tree_(ASTNode *node, gpointer data)
{
	ASTNodeType_free(node->type);
	g_slice_free (ASTNode, node);
	return FALSE;
}

void ASTNode_free_tree(ASTNode * tree)
{
	parse_tree_node_traverse_post_order(tree, ASTNode_free_tree_, NULL);
}

ASTNodeType * ASTNodeType_new(NodeTypeKind kind, ASTNode * node)
{
	ASTNodeType * type = g_slice_new0(ASTNodeType);
	type->kind = kind;
	type->node = node;
    type->text = NULL;
	return type;
}

void ASTNodeType_free(ASTNodeType * type)
{
	g_slice_free(ASTNodeType, type);
}

void ASTNode_add_type(ASTNode * node, NodeTypeKind kind, ASTNode * link_node)
{
	ASTNodeType * type = ASTNodeType_new(kind,link_node);
	node->type = type;
}

gboolean parse_tree_node_traverse_pre_order (ASTNode * node, ASTNodeTraverseFunc func, gpointer data)
{
	if (node->children)
	{
		ASTNode * child;

		if (func (node, data))
			return TRUE;
		child = node->children;
		while (child)
		{
			ASTNode * current;
			current = child;
			child = current->next_sibling;
			if (parse_tree_node_traverse_pre_order (current, func, data))
				return TRUE;
		}
	}
	else
	{
		if (func (node, data))
			return TRUE;
	}
	return FALSE;
}

gboolean parse_tree_node_traverse_post_order (ASTNode * node, ASTNodeTraverseFunc func, gpointer data)
{
	ASTNode* next_sibling = NULL;
	for (ASTNode * child = node->children; child != NULL; child = next_sibling)
	{
		// already get next sibling to avoid invalid pointer access,
		// in case 'func' deletes the child
		next_sibling = child->next_sibling;

		if (parse_tree_node_traverse_post_order (child, func, data))
			return TRUE;
	}

	return func (node, data);
}

// TODO,  only at top level, can improve to a deep search
ASTNode * ASTNode_search_node_by_cx(ASTNode * parent, gpointer cx)
{
	ASTNode * node = parent->children;
	while(node)
	{
		if (node->cx == cx)
			return (gpointer)node;
		node = node->next_sibling;
	}
	return node;
}

void ASTNode_unlink (ASTNode * pnode)
{
  g_return_if_fail (pnode != NULL);

  if (pnode->prev_sibling)
    pnode->prev_sibling->next_sibling = pnode->next_sibling;

  if (pnode->parent && pnode->parent->children) //if it's the first one
  if (pnode->parent->children == pnode) //if it's the first one
    pnode->parent->children = pnode->next_sibling;

  if (pnode->next_sibling)
    {
      pnode->next_sibling->prev_sibling = pnode->prev_sibling;
    }
  pnode->next_sibling = NULL;
  pnode->prev_sibling = NULL;
  pnode->parent = NULL;
}

gboolean ASTNode_search_children (ASTNode * node, NodeKind kind)
{
    ASTNode * child = node->children;
    while(child)
    {
    	if (kind == child->kind)
            return TRUE;
        child = child->next_sibling;
    }
    return FALSE;
}


ASTNode * ASTNode_search_parents(ASTNode * node, NodeKind kind)
{
    ASTNode * parent = node->parent;
    while(parent)
    {
    	if (kind == parent->kind)
            return parent;
        parent = parent->parent;
    }
    return NULL;
}

ASTNode * ASTNode_search_for_parent_node_with_parent(ASTNode * node, NodeKind kind)
{
    ASTNode * parent = node->parent;
    ASTNode * curr_node = node;
    while(parent)
    {
    	if (kind == parent->kind)
            return curr_node;
        curr_node = parent;
        parent = parent->parent;
    }
    return NULL;
}

void ASTNode_insert_before(ASTNode * curr_node, ASTNode * new_node)
{
    if(curr_node->prev_sibling)
        curr_node->prev_sibling->next_sibling = new_node;
    if(curr_node == curr_node->parent ->children)
        curr_node->parent->children = new_node;
    new_node->prev_sibling = curr_node->prev_sibling;
    new_node->parent = curr_node->parent;
    new_node->next_sibling = curr_node;
    curr_node->prev_sibling = new_node;
}

void ASTNode_replace_with_ori_links(ASTNode * nodeA , ASTNode * nodeB, ASTNode * ori_parent, ASTNode * ori_next, ASTNode * ori_prev)
{
   g_assert(nodeA && nodeB && "Node value is NULL");

	if(ori_parent->children == nodeA)
			 ori_parent->children = nodeB;

			if(ori_prev)
			{
				ori_prev->next_sibling = nodeB;
			}
			if(ori_next)
			{
				ori_next->prev_sibling = nodeB;
			}

			nodeB->prev_sibling = ori_prev;
			nodeB->next_sibling = ori_next;
			nodeB->parent = ori_parent;

//	nodeA->parent = pnode;
	nodeA->prev_sibling = NULL;
	nodeA->next_sibling = NULL;

}

void ASTNode_clean_replace_ori_links(ASTNode * nodeA , ASTNode * nodeB, ASTNode * ori_parent, ASTNode * ori_next, ASTNode * ori_prev)
{
		nodeA->parent = ori_parent;
		nodeA->next_sibling = ori_next;
		nodeA->prev_sibling = ori_prev;


		if(ori_parent->children == nodeB)
				 ori_parent->children = nodeA;

				if(ori_prev)
				{
					ori_prev->next_sibling = nodeA;
				}
				if(ori_next)
				{
					ori_next->prev_sibling = nodeA;
				}


}


 void ASTNode_replace(ASTNode * nodeA , ASTNode * nodeB)
 {
	// no child
    g_assert(nodeA && nodeB && "Node value is NULL");

		if(nodeA->parent->children == nodeA)
		    nodeA->parent->children = nodeB;

		if(nodeA->prev_sibling)
		{
			nodeA->prev_sibling->next_sibling = nodeB;
		}
		if(nodeA->next_sibling)
		{
			nodeA->next_sibling->prev_sibling = nodeB;
		}

		nodeB->prev_sibling = nodeA->prev_sibling;
		nodeB->next_sibling = nodeA->next_sibling;
		nodeB->parent = nodeA->parent;

 }

void ASTNode_swap_nodes(ASTNode * nodeA , ASTNode * nodeB)
{
	// keep all the child and not on the same tree branch
    g_assert(nodeA && nodeB && "Node value is NULL");

	ASTNode * parent = nodeA->parent;
    ASTNode * prev_sibling = NULL;
    ASTNode * next_sibling =  NULL;

		if(nodeA->parent->children == nodeA)
		    nodeA->parent->children = nodeB;

		if(nodeB->parent->children == nodeB)
		    nodeB->parent->children = nodeA;



	if(nodeA->next_sibling == nodeB)
    {
	    prev_sibling = nodeA->prev_sibling;
	    next_sibling = nodeA->next_sibling;

	    nodeA->next_sibling= nodeB->next_sibling;
	    nodeA->prev_sibling= nodeB;
	    nodeB->next_sibling= nodeA;
	    nodeB->prev_sibling= prev_sibling;

    }
	else if(nodeA->prev_sibling == nodeB)
    {
	    prev_sibling = nodeA->prev_sibling;
	    next_sibling = nodeA->next_sibling;

	    nodeA->next_sibling= nodeB;
	    nodeA->prev_sibling= nodeB->prev_sibling;
	    nodeB->next_sibling= next_sibling;
	    nodeB->prev_sibling= nodeA;
    }
	else
    {

	    prev_sibling = nodeA->prev_sibling;
	    next_sibling = nodeA->next_sibling;

	    /*
	    if(nodeB->parent == nodeA)
	    {
	    	parent = nodeA->parent;
	    	nodeA->parent = nodeB;
	    	nodeB->parent = parent;

	    }
	    else if (nodeA->parent == nodeB)
	    {
	    	parent = nodeB->parent;
	    	nodeB->parent = nodeA;
	    	nodeA->parent = parent;

	    }
	    else{
	    */
	    nodeA->parent = nodeB->parent;
	    nodeB->parent = parent;

//	    }

	    nodeA->next_sibling= nodeB->next_sibling;
	    nodeA->prev_sibling= nodeB->prev_sibling;

	    nodeB->next_sibling= next_sibling;
	    nodeB->prev_sibling= prev_sibling;
    }


	if(nodeA->next_sibling)
        nodeA->next_sibling->prev_sibling = nodeA;
	if(nodeB->next_sibling)
        nodeB->next_sibling->prev_sibling = nodeB;

	if(nodeA->prev_sibling)
        nodeA->prev_sibling->next_sibling = nodeA;
	if(nodeB->prev_sibling)
        nodeB->prev_sibling->next_sibling = nodeB;

}


void ASTNode_set_linkage(ASTNode * node, NodeLinkageKind linkage)
{
    node->ext[0] = (gint)linkage;
}

NodeLinkageKind ASTNode_get_linkage(ASTNode * node)
{
    return (NodeLinkageKind) node->ext[0];
}

void ASTNode_set_const(ASTNode * node)
{
    node->ext[1] = (gint)TRUE;
}

gboolean ASTNode_is_const(ASTNode * node)
{
    return (gboolean) node->ext[1];
}

void ASTNode_set_static(ASTNode * node)
{
    node->ext[2] = (gint)TRUE;
}

gboolean ASTNode_is_static(ASTNode * node)
{
    return (gboolean) node->ext[2];
}

void ASTNode_set_ext_3(ASTNode * node, gint ext)
{
    node->ext[3] = (gint)ext;
}

gboolean ASTNode_check_ext_3(ASTNode * node)
{
    return node->ext[3];
}


gboolean ASTNode_check_type_kind(ASTNode * node, NodeTypeKind kind)
{
    if(!node->type)
        return FALSE;
    if(node->type->kind == kind)
        return TRUE;
    else return FALSE;
}

gboolean ASTNode_search_children_type (ASTNode * node, NodeTypeKind kind)
{
    ASTNode * child = node->children;
    while(child)
    {
    	if (ASTNode_check_type_kind(child,kind))
            return TRUE;
        child = child->next_sibling;
    }
    return FALSE;
}

void ASTNode_append_child(ASTNode * node, ASTNode * child)
{
	ASTNode * curr_child = node->children;

	if(curr_child)
	{
		while (curr_child->next_sibling)
		{
			curr_child = curr_child->next_sibling;
		}
		curr_child->next_sibling = child;
		child->prev_sibling = curr_child;
	}

	else
	{
		node->children = child;
	}

	child->parent = node;

}
/*
ASTNode * ASTNode_deep_copy(ASTNode * node, gboolean copy_child)
{
    ASTNode * copy = ASTNode_new(node->kind, node->text, node->cx);
    ASTNode * child = node->children;

    while(child && copy_child)
    {
    	ASTNode_append_child(copy,ASTNode_shallow_copy( child, copy_child));
        child = child->next_sibling;
    }

    return copy;

}
*/

ASTNode * ASTNode_shallow_copy(ASTNode * node, gboolean copy_child)
{
	ASTNode * copy = ASTNode_new(node->kind, node->text, node->cx);

	if(node->type)
	{
		ASTNode_add_type(copy,node->type->kind,node->type->node);
		copy->type->text = node->type->text;
	}

	ASTNode * child = node->children;
	while(child && copy_child)
	{
		ASTNode_append_child(copy,ASTNode_shallow_copy( child, copy_child));
		child = child->next_sibling;
	}

	return copy;
}

void parse_tree_node_transform(ASTNode * node)
{
	if (node->children)
	{
		ASTNode * child;
		if(node->kind == NodeKind_IfStmt)
		{

			ASTNode * el = node->children->next_sibling->next_sibling;
			if(el)
			{
				if(el->kind != NodeKind_CompoundStmt)
				{
					ASTNode_unlink(el);
					ASTNode * cp_node = ASTNode_new(NodeKind_CompoundStmt, "",NULL);
					node->children->next_sibling->next_sibling = cp_node;
					cp_node->prev_sibling = node->children->next_sibling;
					cp_node->parent = node;
					cp_node->children = el;

					el->parent = cp_node;
					el->prev_sibling = NULL;
					el->next_sibling = NULL;
				}
			}

		}

		child = node->children;
		while (child)
		{
			ASTNode * current;
			current = child;
			child = current->next_sibling;
			parse_tree_node_transform(current);
		}
	}
	else
	{
		if(node->kind == NodeKind_IfStmt)
		{

			ASTNode * el = node->children->next_sibling->next_sibling;
			if(el)
			{
				if(el->kind != NodeKind_CompoundStmt)
				{
					ASTNode_unlink(el);
					ASTNode * cp_node = ASTNode_new(NodeKind_CompoundStmt, "",NULL);
					node->children->next_sibling->next_sibling = cp_node;
					cp_node->prev_sibling = node->children->next_sibling;
					cp_node->parent = node;
					cp_node->children = el;

					el->parent = cp_node;
					el->prev_sibling = NULL;
					el->next_sibling = NULL;
				}
			}

		}
	}

}

/** Clean the redundent node in libclang AST to save more space. 
    This is libclang dependent.*/
void parse_tree_node_clean(ASTNode * node)
{

//   MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,node->text)  : 0 ;
	if (node->children)
	{
		ASTNode * child;
		if(node->kind == NodeKind_FloatingLiteral)
		{
			if(node->children)
			{
				if(node->children->kind == NodeKind_MiluSourceFloat && node->children->next_sibling == NULL)
				{
					ASTNode * new_node = ASTNode_shallow_copy(node->children,FALSE);
					new_node->kind = NodeKind_FloatingLiteral;
					ASTNode_replace(node, new_node);
					//TODO: FREE the original nodes whichs are shallow copied
					return ;
				}
			}
		}
		if(node->kind == NodeKind_IntegerLiteral)
		{
			if(node->children)
			{
				if(node->children->kind == NodeKind_MiluSourceInteger && node->children->next_sibling == NULL)
				{
					ASTNode * new_node = ASTNode_shallow_copy(node->children,FALSE);
					new_node->kind = NodeKind_IntegerLiteral;
					ASTNode_replace(node, new_node);
					//TODO: FREE the original nodes whichs are shallow copied
					return ;
				}
			}
		}
		if(node->kind == NodeKind_UnexposedExpr)
		{
			if(node->children)
			{
				if(node->children->kind == NodeKind_DeclRefExpr && node->children->next_sibling == NULL)
				{
					ASTNode * new_node = ASTNode_shallow_copy(node->children,FALSE);
					ASTNode_replace(node, new_node);
					//TODO: FREE the original nodes whichs are shallow copied
					return ;
				}
			}
		}
		child = node->children;
		while (child)
		{
			ASTNode * current;
			current = child;
			child = current->next_sibling;
			parse_tree_node_clean(current);
		}
	}
	else
	{
		if(node->kind == NodeKind_IntegerLiteral)
		{
			if(node->children->kind == NodeKind_MiluSourceInteger && node->children->next_sibling == NULL)
			{
				ASTNode * new_node = ASTNode_shallow_copy(node->children,FALSE);
				new_node->kind = NodeKind_IntegerLiteral;
				ASTNode_replace(node, new_node);
				//TODO: FREE the original nodes whichs are shallow copied
				return ;
			}
		}
		if(node->kind == NodeKind_UnexposedExpr)
		{
			if(node->children)
			{
				if(node->children->kind == NodeKind_DeclRefExpr && node->children->next_sibling == NULL)
				{
					ASTNode * new_node = ASTNode_shallow_copy(node->children,FALSE);
					ASTNode_replace(node, new_node);
				    //TODO: FREE the original nodes whichs are shallow copied
					return ;
				}
			}
		}
	}
}


ASTNode * ASTNode_get_root(ASTNode * node)
{
	ASTNode * root = node;
	while(root->parent)
	{
		root = root->parent ;
	}
	return root;
}

gchar * ASTNode_get_func(ASTNode * node)
{
	ASTNode * root = node;
	while(root->parent)
	{
		root = root->parent ;
                if(root->kind == NodeKind_FunctionDecl)
                    return root->text;
	}
	return NULL;
}
