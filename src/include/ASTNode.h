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

#ifndef NODE_H_
#define NODE_H_

/**@file
 * Implement ASTNode.
 */

/**@addtogroup Parser
 * @{
 */

#include <glib.h>
#include <stdio.h>


/**
 * \brief Alias for ASTNode
 **/
typedef struct ASTNode ASTNode;

/**
 * \brief Alias for ASTNodeKind
 **/
typedef enum ASTNodeKind NodeKind;

/**
 * \brief Alias for ASTNodeType
 **/
typedef struct ASTNodeType ASTNodeType;

/**
 * \brief Alias for NodeTypeKind
 **/
typedef enum ASTNodeTypeKind NodeTypeKind;

/**
 * \brief Alias for NodeLinkageKind
 **/
typedef enum NodeLinkageKind NodeLinkageKind;

/** \brief This value indicates that no linkage information is available for a provided node. This is mapped from the libclang header directly */
enum NodeLinkageKind {
NodeLinkage_Invalid, /** < This is the linkage for variables, parameters, and so on that  have automatic storage.  This covers normal (non-extern) local variables.
*/
NodeLinkage_NoLinkage,  /**< This is the linkage for static variables and static functions. */
NodeLinkage_Internal,   /**< This is the linkage for entities with external linkage that live in C++ anonymous namespaces.*/
NodeLinkage_UniqueExternal, /**< This is the linkage for entities with true, external linkage. */
NodeLinkage_External /**< This is the linkage for entities with true, external linkage. */
};


/**
 * \brief Define the type kind of ASTNode.
 *
 * This is mapped from libclang.
 **/
enum ASTNodeTypeKind {
	NodeTypeKind_Invalid = 0,
	NodeTypeKind_Unexposed = 1,
	NodeTypeKind_Void = 2,
	NodeTypeKind_Bool = 3,
	NodeTypeKind_Char_U = 4,
	NodeTypeKind_UChar = 5,
	NodeTypeKind_Char16 = 6,
	NodeTypeKind_Char32 = 7,
	NodeTypeKind_UShort = 8,
	NodeTypeKind_UInt = 9,
	NodeTypeKind_ULong = 10,
	NodeTypeKind_ULongLong = 11,
	NodeTypeKind_UInt128 = 12,
	NodeTypeKind_Char_S = 13,
	NodeTypeKind_SChar = 14,
	NodeTypeKind_WChar = 15,
	NodeTypeKind_Short = 16,
	NodeTypeKind_Int = 17,
	NodeTypeKind_Long = 18,
	NodeTypeKind_LongLong = 19,
	NodeTypeKind_Int128 = 20,
	NodeTypeKind_Float = 21,
	NodeTypeKind_Double = 22,
	NodeTypeKind_LongDouble = 23,
	NodeTypeKind_NullPtr = 24,
	NodeTypeKind_Overload = 25,
	NodeTypeKind_Dependent = 26,
	NodeTypeKind_ObjCId = 27,
	NodeTypeKind_ObjCClass = 28,
	NodeTypeKind_ObjCSel = 29,
	NodeTypeKind_FirstBuiltin = NodeTypeKind_Void,
	NodeTypeKind_LastBuiltin  = NodeTypeKind_ObjCSel,
	NodeTypeKind_Complex = 100,
	NodeTypeKind_Pointer = 101,
	NodeTypeKind_BlockPointer = 102,
	NodeTypeKind_LValueReference = 103,
	NodeTypeKind_RValueReference = 104,
	NodeTypeKind_Record = 105,
	NodeTypeKind_Enum = 106,
	NodeTypeKind_Typedef = 107,
	NodeTypeKind_ObjCInterface = 108,
	NodeTypeKind_ObjCObjectPointer = 109,
	NodeTypeKind_FunctionNoProto = 110,
	NodeTypeKind_FunctionProto = 111,
	NodeTypeKind_ConstantArray = 112
};


/**
 * \brief Define the type kind of ASTNode.
 *
 * This is mapped from libclang.
 **/
enum ASTNodeKind {

  NodeKind_UnexposedDecl                 = 1,
  NodeKind_StructDecl                    = 2,
  NodeKind_UnionDecl                     = 3,
  NodeKind_ClassDecl                     = 4,
  NodeKind_EnumDecl                      = 5,
  NodeKind_FieldDecl                     = 6,
  NodeKind_EnumConstantDecl              = 7,
  NodeKind_FunctionDecl                  = 8,
  NodeKind_VarDecl                       = 9,
  NodeKind_ParmDecl                      = 10,
  NodeKind_ObjCInterfaceDecl             = 11,
  NodeKind_ObjCCategoryDecl              = 12,
  NodeKind_ObjCProtocolDecl              = 13,
  NodeKind_ObjCPropertyDecl              = 14,
  NodeKind_ObjCIvarDecl                  = 15,
  NodeKind_ObjCInstanceMethodDecl        = 16,
  NodeKind_ObjCClassMethodDecl           = 17,
  NodeKind_ObjCImplementationDecl        = 18,
  NodeKind_ObjCCategoryImplDecl          = 19,
  NodeKind_TypedefDecl                   = 20,
  NodeKind_CXXMethod                     = 21,
  NodeKind_Namespace                     = 22,
  NodeKind_LinkageSpec                   = 23,
  NodeKind_Constructor                   = 24,
  NodeKind_Destructor                    = 25,
  NodeKind_ConversionFunction            = 26,
  NodeKind_TemplateTypeParameter         = 27,
  NodeKind_NonTypeTemplateParameter      = 28,
  NodeKind_TemplateTemplateParameter     = 29,
  NodeKind_FunctionTemplate              = 30,
  NodeKind_ClassTemplate                 = 31,
  NodeKind_ClassTemplatePartialSpecialization = 32,
  NodeKind_NamespaceAlias                = 33,
  NodeKind_UsingDirective                = 34,
  NodeKind_UsingDeclaration              = 35,
  NodeKind_TypeAliasDecl                 = 36,
  NodeKind_ObjCSynthesizeDecl            = 37,
  NodeKind_ObjCDynamicDecl               = 38,
  NodeKind_CXXAccessSpecifier            = 39,

  NodeKind_FirstDecl                     = NodeKind_UnexposedDecl,
  NodeKind_LastDecl                      = NodeKind_CXXAccessSpecifier,

  NodeKind_FirstRef                      = 40, /* Decl references */
  NodeKind_ObjCSuperClassRef             = 40,
  NodeKind_ObjCProtocolRef               = 41,
  NodeKind_ObjCClassRef                  = 42,

  NodeKind_TypeRef                       = 43,
  NodeKind_CXXBaseSpecifier              = 44,

  NodeKind_TemplateRef                   = 45,
  NodeKind_NamespaceRef                  = 46,
  NodeKind_MemberRef                     = 47,
  NodeKind_LabelRef                      = 48,
  NodeKind_OverloadedDeclRef             = 49,

  NodeKind_LastRef                       = NodeKind_OverloadedDeclRef,

  /* Error conditions */
  NodeKind_FirstInvalid                  = 70,
  NodeKind_InvalidFile                   = 70,
  NodeKind_NoDeclFound                   = 71,
  NodeKind_NotImplemented                = 72,
  NodeKind_InvalidCode                   = 73,
  NodeKind_LastInvalid                   = NodeKind_InvalidCode,

  /* Expressions */
  NodeKind_FirstExpr                     = 100,
  NodeKind_UnexposedExpr                 = 100,
  NodeKind_DeclRefExpr                   = 101,
  NodeKind_MemberRefExpr                 = 102,
  NodeKind_CallExpr                      = 103,
  NodeKind_ObjCMessageExpr               = 104,
  NodeKind_BlockExpr                     = 105,
  NodeKind_IntegerLiteral                = 106,
  NodeKind_FloatingLiteral               = 107,
  NodeKind_ImaginaryLiteral              = 108,
  NodeKind_StringLiteral                 = 109,
  NodeKind_CharacterLiteral              = 110,
  NodeKind_ParenExpr                     = 111,
  NodeKind_UnaryOperator                 = 112,
  NodeKind_ArraySubscriptExpr            = 113,
  NodeKind_BinaryOperator                = 114,
  NodeKind_CompoundAssignOperator        = 115,
  NodeKind_ConditionalOperator           = 116,
  NodeKind_CStyleCastExpr                = 117,
  NodeKind_CompoundLiteralExpr           = 118,
  NodeKind_InitListExpr                  = 119,
  NodeKind_AddrLabelExpr                 = 120,
  NodeKind_StmtExpr                      = 121,
  NodeKind_GenericSelectionExpr          = 122,
  NodeKind_GNUNullExpr                   = 123,
  NodeKind_CXXStaticCastExpr             = 124,
  NodeKind_CXXDynamicCastExpr            = 125,
  NodeKind_CXXReinterpretCastExpr        = 126,
  NodeKind_CXXConstCastExpr              = 127,
  NodeKind_CXXFunctionalCastExpr         = 128,
  NodeKind_CXXTypeidExpr                 = 129,
  NodeKind_CXXBoolLiteralExpr            = 130,
  NodeKind_CXXNullPtrLiteralExpr         = 131,
  NodeKind_CXXThisExpr                   = 132,
  NodeKind_CXXThrowExpr                  = 133,
  NodeKind_CXXNewExpr                    = 134,
  NodeKind_CXXDeleteExpr                 = 135,
  NodeKind_UnaryExpr                     = 136,
  NodeKind_ObjCStringLiteral             = 137,
  NodeKind_ObjCEncodeExpr                = 138,
  NodeKind_ObjCSelectorExpr              = 139,
  NodeKind_ObjCProtocolExpr              = 140,
  NodeKind_ObjCBridgedCastExpr           = 141,
  NodeKind_PackExpansionExpr             = 142,
  NodeKind_SizeOfPackExpr                = 143,
  NodeKind_LastExpr                      = NodeKind_SizeOfPackExpr,
  NodeKind_FirstStmt                     = 200,
  NodeKind_UnexposedStmt                 = 200,
  NodeKind_LabelStmt                     = 201,
  NodeKind_CompoundStmt                  = 202,
  NodeKind_CaseStmt                      = 203,
  NodeKind_DefaultStmt                   = 204,
  NodeKind_IfStmt                        = 205,
  NodeKind_SwitchStmt                    = 206,
  NodeKind_WhileStmt                     = 207,
  NodeKind_DoStmt                        = 208,
  NodeKind_ForStmt                       = 209,
  NodeKind_GotoStmt                      = 210,
  NodeKind_IndirectGotoStmt              = 211,
  NodeKind_ContinueStmt                  = 212,
  NodeKind_BreakStmt                     = 213,
  NodeKind_ReturnStmt                    = 214,
  NodeKind_AsmStmt                       = 215,
  NodeKind_ObjCAtTryStmt                 = 216,
  NodeKind_ObjCAtCatchStmt               = 217,
  NodeKind_ObjCAtFinallyStmt             = 218,
  NodeKind_ObjCAtThrowStmt               = 219,
  NodeKind_ObjCAtSynchronizedStmt        = 220,
  NodeKind_ObjCAutoreleasePoolStmt       = 221,
  NodeKind_ObjCForCollectionStmt         = 222,
  NodeKind_CXXCatchStmt                  = 223,
  NodeKind_CXXTryStmt                    = 224,
  NodeKind_CXXForRangeStmt               = 225,
  NodeKind_SEHTryStmt                    = 226,
  NodeKind_SEHExceptStmt                 = 227,
  NodeKind_SEHFinallyStmt                = 228,
  NodeKind_NullStmt                      = 230,
  NodeKind_DeclStmt                      = 231,
  NodeKind_LastStmt                      = NodeKind_DeclStmt,
  NodeKind_TranslationUnit               = 300,

  NodeKind_FirstAttr                     = 400,
  NodeKind_UnexposedAttr                 = 400,

  NodeKind_IBActionAttr                  = 401,
  NodeKind_IBOutletAttr                  = 402,
  NodeKind_IBOutletCollectionAttr        = 403,
  NodeKind_CXXFinalAttr                  = 404,
  NodeKind_CXXOverrideAttr               = 405,
  NodeKind_AnnotateAttr                  = 406,
  NodeKind_LastAttr                      = NodeKind_AnnotateAttr,
  NodeKind_AsmLabelAttr						= 407,
  /* Preprocessing */
  NodeKind_PreprocessingDirective        = 500,
  NodeKind_MacroDefinition               = 501,
  NodeKind_MacroExpansion                = 502,
  NodeKind_MacroInstantiation            = NodeKind_MacroExpansion,
  NodeKind_InclusionDirective            = 503,
  NodeKind_FirstPreprocessing            = NodeKind_PreprocessingDirective,
  NodeKind_LastPreprocessing             = NodeKind_InclusionDirective,

  NodeKind_MiluSource = 700,
  NodeKind_MiluSourceInteger = 701,
  NodeKind_UnaryOperator_Append = 702,
  NodeKind_MiluSourceFloat= 703
};

/**
 * \brief Stores print name of ASTNodeKinds.
 **/
extern gchar * ASTNodeKindNames[1024] ;

/**
 * \brief Stores print name of ASTNodeTypeKinds.
 **/
extern gchar * ASTNodeTypeKindNames[1024] ;

/**
 * \brief Define node for the abstract syntax tree.
 **/
struct ASTNode
{
	gsize id;                     /**< The id of the node. */
	gchar * text;                 /**< The text of the node. */
	gchar * ori_text;             /**< The original text of the node, used for soem kind of mutation operator. */
	NodeKind ori_kind;             /**< The original kind of the node, used for soem kind of mutation operator. */
	NodeKind kind;                /**<  The kind of the node. */
    //NodeLinkageKind linkage ;     /**< The linkage of the node. */
    gint ext[4] ; /** < The extending infomration e.g. linkage, const. */
	struct ASTNodeType * type;    /**<  The type of the node. */
	gint line_start; 				/**< Record the start line number in the original file. */
	gint line_end;					/**< Record the end line number in the original file.*/
	ASTNode * parent;             /**< The parent node of the node. */
	ASTNode * next_sibling;       /**< The next node of the node. */
	ASTNode * prev_sibling;       /**< The previous node of the node. */
	ASTNode * children;           /**< The children of node */
	gpointer cx;                  /**< A Pointer to libclang Node */
    gboolean is_mutant;           /**< If the ASTNode is mutated */
};

/**
 * \brief Define types of ASTNode.
 **/
struct ASTNodeType
{
	NodeTypeKind kind;
    gchar * text;
	struct ASTNode * node;
};


/**
 * \brief Create a new ASTNode and set a given parent node.
 *
 * \param parent The give parent node.
 * \param type The type of new node.
 * \param text The text of new node.
 * \param cx The pointer to the original libclang AST node.
 *
 **/
ASTNode * ASTNode_new_with_parent(ASTNode * parent, NodeKind kind, gchar * text, gpointer cx);

/**
 * \brief Create a new ASTNode.
 *
 * \param type The type of new node.
 * \param text The text of new node.
 * \param cx The pointer to the original libclang AST node.
 *
 **/
ASTNode * ASTNode_new(NodeKind type, gchar * text, gpointer cx);

/**
 * \brief Free the given ASTNode.
 *
 * \param node The give ASTNode.
 *
 **/
void ASTNode_free(ASTNode * node);

/**
 * \Define the a hook function pointer for traversing AST.
 */
typedef gboolean (*ASTNodeTraverseFunc) (ASTNode * node, gpointer data);

/**
 * \brief Set a given text to a ASTNode.
 *
 * \param node The given node.
 * \param text The given text.
 *
 */
void ASTNode_set_text(ASTNode * node, gchar * text);

/**
 * \brief Save the original kind of a ASTNode.
 *
 * \param node The given node.
 *
 */
void ASTNodeType_save_original_kind(ASTNode * node);


/**
 * \brief Restore the original kind of a ASTNode.
 *
 * \param node The given node.
 *
 */
void ASTNodeType_restore_original_kind(ASTNode * node);

/**
 * \brief Save the original text of a ASTNode.
 *
 * \param node The given node.
 *
 */
void ASTNodeType_save_original_text(ASTNode * node);

/**
 * \brief Restore the original text of a ASTNode.
 *
 * \param node The given node.
 *
 */
void ASTNodeType_restore_original_text(ASTNode * node);
/**
 * \brief Globe initialization for ASTNode.
 *
 **/
void ASTNode_init();


/**
 * \brief Initialize the print names of ASTNodeTypeKind.
 **/
void ASTNodeTypeKindNames_init();

/**
 * \brief Initialize the print names of ASTNodeTypeKind.
 **/
void ASTNodeKindNames_init();

/**
 * \brief Search ASTNode by the pointer of the libclang node.
 *
 * \param parent Root of the given AST.
 * \param cx The given pointer to a libclang ASTnode.
 *
 * \return ASTNode if found; NULL if not.
 */
ASTNode * ASTNode_search_node_by_cx(ASTNode * parent, gpointer cx);

/**
 *  \brief Interface to traverse the abstract syntax tree.
 *
 *  \param node The root of the tree.
 *  \param func The hook function to call when traversing the nodes.
 *  \param data The user defined data.
 *
 */
gboolean parse_tree_node_traverse_pre_order (ASTNode * node, ASTNodeTraverseFunc func, gpointer data);

/**
 * \brief Create a new ASTNodeType.
 *
 * \param kind The given ASTNode kind.
 * \param node The given node to add.
 *
 **/
ASTNodeType * ASTNodeType_new(NodeTypeKind kind, ASTNode * node);

/**
 * \brief Free the given ASTNodeType.
 *
 * \param type The give ASTNodeType.
 *
 **/
void ASTNodeType_free(ASTNodeType * type);

/**
 * \brief Add a ASTNodeType to ASTNode.
 *
 * \param node The given node to add.
 * \param kind The given ASTNode kind.
 * \param link_node The node which links to the definition node.
 **/
void ASTNode_add_type(ASTNode * node, NodeTypeKind kind, ASTNode * link_node);

/**
 * \brief Add a ASTNodeType to ASTNode.
 *
 * \param node The given node to add.
 * \param kind The given ASTNode kind.
 * \param link_node The node which links to the definition node.
 **/
void ASTNode_add_type(ASTNode * node, NodeTypeKind kind, ASTNode * link_node);

/**
 * \brief Unlinks a node from a tree, resulting in two separate trees.
 * \param node The node to unlink, which becomes the root of a new tree.
 */
void ASTNode_unlink (ASTNode * pnode);

/**
 * \brief Search the children of given node for the given kind.
 *
 * \param node The node to search.
 * \param kind The Node kind to search for.
 *
 * \return True if found node with the kind.
 */
gboolean ASTNode_search_children (ASTNode * node, NodeKind kind);


/**
 * \brief Search the parents of given node for the given kind.
 *
 * \param node The node to search.
 * \param kind The Node kind to search for.
 *
 * \return the first found parent node or NULL.
 */
ASTNode * ASTNode_search_parents(ASTNode * node, NodeKind kind);



/**
 * \brief Search the node with parent of given node for the given kind.
 *
 * \param node The node to search.
 * \param kind The Node kind to search for.
 *
 * \return the first found such node or NULL.
 */

ASTNode * ASTNode_search_for_parent_node_with_parent(ASTNode * node, NodeKind kind);

/**
 * \brief replace node B with node A;
 *
 *
 * \param nodeA The original node.
 * \param nodeB The node to replace.
 *
 */
void ASTNode_replace(ASTNode * nodeA , ASTNode * nodeB);



/**
 * \brief replace node B with node A, with ori links;
 *
 *
 * \param nodeA The original node.
 * \param nodeB The node to replace.
 * \param ori_parent parent link
 * \param ori_next next link
 * \param ori_prev prev link
 *
 */
void ASTNode_replace_with_ori_links(ASTNode * nodeA , ASTNode * nodeB, ASTNode * ori_parent, ASTNode * ori_next, ASTNode * ori_prev);

/**
 * \brief reverse the replace ment;
 *
 *
 * \param nodeA The node to be reverse.
 * \param nodeB The original node.
 * \param ori_parent parent link
 * \param ori_next next link
 * \param ori_prev prev link
 *
 */
void ASTNode_clean_replace_ori_links(ASTNode * nodeA , ASTNode * nodeB, ASTNode * ori_parent, ASTNode * ori_next, ASTNode * ori_prev);

/**
 * \brief Swap the position of two given nodes.
 *
 * \param nodeA The node to swap.
 * \param nodeB The node to swap.
 *
 */
//TODO: bugy
void ASTNode_swap_nodes(ASTNode * nodeA , ASTNode * nodeB);

/**
 * \brief Set linkage to a node.
 *
 * \param node The given node.
 * \param linkage The given linkage.
 */
void ASTNode_set_linkage(ASTNode * node, NodeLinkageKind linkage);

/**
 * \brief Get the linkage from node.
 *
 * \param The give node.
 * \return the linkage.
 */
NodeLinkageKind ASTNode_get_linkage(ASTNode * node);

/**
 * \brief Set text to the type of a given node.
 *
 * \param node The given node.
 * \param text The text to set.
 */
void ASTNodeType_set_text(ASTNode * node, gchar * text);


/**
 * \brief Set const to a node.
 *
 * \param node The given node.
 */
void ASTNode_set_const(ASTNode * node);

/**
 * \brief Check if a node is const.
 *
 * \param The give node.
 * \return TRUE is the node is const.
 */
gboolean ASTNode_is_const(ASTNode * node);

/**
 * \brief Set static to a node.
 *
 * \param node The given node.
 */
void ASTNode_set_static(ASTNode * node);

/**
 * \brief Check if a node is static.
 *
 * \param The given node.
 * \return TRUE is the node is static.
 */
gboolean ASTNode_is_static(ASTNode * node);


/**
 * \brief Set static to a node.
 *
 * \param node The given node.
 */
void ASTNode_set_ext_3(ASTNode * node, int ext);

/**
 * \brief Check if a node is static.
 *
 * \param The given node.
 * \return TRUE is the node is static.
 */
int ASTNode_check_ext_3(ASTNode * node);


/**
 * \brief Check if a node type kind is the given kind.
 *
 * \param The given node.
 * \param The given node type kind.
 *
 * return TRUE is node's type has same kind.
 */
gboolean ASTNode_check_type_kind(ASTNode * node, NodeTypeKind kind);

/**
 * \brief Search the children of given node for the given type kind.
 *
 * \param node The node to search.
 * \param kind The Node type kind to search for.
 *
 * \return True if found node with the type kind.
 */
gboolean ASTNode_search_children_type (ASTNode * node, NodeTypeKind kind);

/**
 * \brief Insert a new node under the parent before the child node.
 *
 * \param curr_node The node before which to be inserted.
 * \param new_node The node to be insert.
 *
 */
void ASTNode_insert_before(ASTNode * curr_node, ASTNode * new_node);



/**
 * \brief Copy the given ASTNode with child only.
 *
 * \param node The given ASTNode node to copy.
 * \param node if copy child
 * \return the copied ASTNode.
 *
 **/
ASTNode * ASTNode_shallow_copy(ASTNode * node, gboolean copy_child);


/**
 * \brief Append child node to ASTNode's children.
 *
 * \param node The given ASTNode node.
 * \param child The given node to add.
 *
 **/
void ASTNode_append_child(ASTNode * node, ASTNode * child);


/**
 * \brief Clean Already AST
 *
 * Including remove UnexposedExpr
 * \param node The given ASTNode node.
 *
 **/
void parse_tree_node_clean(ASTNode * node);


/**
 * \brief Do some source code transformation
 *
 * add compound node to else source code
 * \param node The given ASTNode node.
 *
 **/
void parse_tree_node_transform(ASTNode * node);


 /**
  * \brief Get root node
  *
  * \param node The given ASTNode node.
  *
  * \return the root node.
  **/
ASTNode * ASTNode_get_root(ASTNode * node);


 /**
  * \brief Get clean the node link
  *
  * \param node The given ASTNode node.
  *
  **/
void ASTNode_clean_link(ASTNode * node);

#endif /* NODE_H_ */
/**@}*/
