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

#ifndef MUTATOR_H_
#define MUTATOR_H_

/**@file
 * Define mutation operator for Milu.
 */

/**@addtogroup mut_operators_gpr Mutation Operator
 * @{
 */

#include "ASTNode.h"
#include "ASTNodeUtil.h"
#include <glib.h>
#include "glib/gprintf.h"

/**
 * \brief Alias for Mutator
 **/
typedef struct MiluMutator Mutator;

/**
 * \brief Check the given Node, return TRUE to mutate, FALSE not to.
 **/
typedef gboolean (*MutatorNodeChecking)(ASTNode * ) ;

/**
 * \brief Mutate the given node.
 **/
typedef gboolean (*MutatorMutate)(ASTNode * , gint) ;

/**
 * \brief Clean the given node's mutation.
 **/
typedef gboolean (*MutatorClean)(ASTNode * , gint) ;


/**
 * \brief Define MiluMutator to mutate a give node.
 **/
struct MiluMutator
{
	gchar * name;                     /**<  The name of the mutator. */
	gchar * info;                     /**<  The detail information of the mutation. */
	MutatorNodeChecking node_checking; /**<  The method to check whether mutate the node.*/
	MutatorMutate mutate;             /**<  The method to check mutate the node. */
	MutatorClean clean;               /**<  The method to clean mutate the node. */
	gint size;                        /**<  The number of type of mutation*/
};

/**
 * \brief Create a new mutator.
 *
 * \param name The name of the mutator.
 * \param info The detail information of the mutator.
 *
 * \return A new mutator. Free it use mutator_free()
 *
 * @see mutator_free()
 * @see mutator_print()
 */
Mutator * mutator_new(const gchar * name, const gchar * info);

/**
 * \brief Free a mutator.
 *
 * \param mut The mutator to free.
 */
void mutator_free(Mutator * mut);

/**
 * \brief Print a given set mutators.
 *
 * \param muts The set of mutators to print.
 * \param output The output stream.
 */
void mutators_print(GPtrArray * muts, FILE * output);

/**
 * \brief Print a given mutator.
 *
 * \param muts The set of mutators to print.
 * \param output The output stream.
 */
void mutator_print(Mutator * mut, FILE * output);

/**
 * \brief Generate mutator bio node for milu austin.
 *
 * \param node The current mutation node.
 * \param mut_text The mutation text.
 *
 * \return the new austin mutation node.
 */
ASTNode * mutator_bio_node_generator_for_austin_milu(ASTNode * node, gchar * mut_text);

/**
 * \brief Generate mutator for any node only need reach condition for milu austin.
 *
 * \param node The current mutation node.
 * \param mut_text The mutation text.
 *
 * \return the new austin mutation node.
 */
ASTNode * mutator_reaching_node_only_generator_for_austin_milu(ASTNode * node, gchar * mut_text);


/**
 * \brief Generate mutator for integer node need reach condition for milu austin.
 *
 * \param node The current mutation node.
 * \param mut_text The mutation text.
 *
 * \return the new austin mutation node.
 */
ASTNode * mutator_integer_constant_node_generator_for_austin_milu(ASTNode * node, gchar * mut_text);

/* User defined mutation operator. */
void USER_DEFINED_MUTATOR_BELOW();

//OAAN
Mutator * mutator_milu_add_arithmetic_replacement();
Mutator * mutator_milu_subtract_arithmetic_replacement();
Mutator * mutator_milu_multiply_arithmetic_replacement();
Mutator * mutator_milu_divide_arithmetic_replacement();
Mutator * mutator_milu_modulus_arithmetic_replacement();

//OAAA
Mutator * mutator_milu_add_arithmetic_assignment_replacement();
Mutator * mutator_milu_subtract_arithmetic_assignment_replacement();
Mutator * mutator_milu_multiply_arithmetic_assignment_replacement();
Mutator * mutator_milu_divide_arithmetic_assignment_replacement();
Mutator * mutator_milu_modulus_arithmetic_assignment_replacement();

//ORRN
Mutator * mutator_milu_equal_relational_replacement();
Mutator * mutator_milu_not_equal_relational_replacement();
Mutator * mutator_milu_greater_equal_relational_replacement();
Mutator * mutator_milu_less_relational_replacement();
Mutator * mutator_milu_greater_relational_replacement();
Mutator * mutator_milu_less_equal_relational_replacement();

//OLLN
Mutator * mutator_milu_and_logical_replacement();
Mutator * mutator_milu_or_logical_replacement();

//OLNG
Mutator * mutator_milu_and_logical_negation();
Mutator * mutator_milu_or_logical_negation();

//OBBN
Mutator * mutator_milu_bitwise_and_logical_replacement();
Mutator * mutator_milu_bitwise_or_logical_replacement();

//OBBA
Mutator * mutator_milu_bitwise_and_logical_assignment_replacement();
Mutator * mutator_milu_bitwise_or_logical_assignment_replacement();

//CRCR
Mutator * mutator_milu_integer_constant_replacement();
Mutator * mutator_milu_float_constant_replacement();

//OCNG
Mutator * mutator_milu_if_expression_negation();
Mutator * mutator_milu_while_expression_negation();

//OIDO
Mutator * mutator_milu_prefix_increment_replacement();
Mutator * mutator_milu_postfix_increment_replacement();
Mutator * mutator_milu_prefix_decrement_replacement();
Mutator * mutator_milu_postfix_decrement_replacement();

//SBRC
Mutator * mutator_milu_break_continue_replacement();
//ABS
Mutator *  mutator_milu_integer_var_abs_insertion();
Mutator *  mutator_milu_float_var_abs_insertion();
//UOI
Mutator *  mutator_milu_integer_var_uoi_insertion();

//AUSTIN
Mutator * mutator_austin_milu_greater_relational_replacement();
Mutator * mutator_austin_milu_less_relational_replacement();
Mutator *  mutator_austin_milu_less_equal_relational_replacement();
Mutator * mutator_austin_milu_greater_equal_relational_replacement();
Mutator * mutator_austin_milu_equal_relational_replacement();
Mutator * mutator_austin_milu_not_equal_relational_replacement();

Mutator * mutator_austin_milu_add_arithmetic_replacement();
Mutator * mutator_austin_milu_subtract_arithmetic_replacement();
Mutator * mutator_austin_milu_multiply_arithmetic_replacement();
Mutator * mutator_austin_milu_divide_arithmetic_replacement();
Mutator * mutator_austin_milu_modulus_arithmetic_replacement();

Mutator * mutator_austin_milu_and_logical_replacement();
Mutator * mutator_austin_milu_or_logical_replacement();

Mutator * mutator_austin_milu_and_logical_negation();
Mutator * mutator_austin_milu_or_logical_negation();

Mutator * mutator_austin_milu_integer_constant_replacement();

Mutator * mutator_milu_statement_deletion();

#endif /* MUTATOR_H_ */
/**@}*/
