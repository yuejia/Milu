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
#ifndef MUTATIONOPERATORS_H_
#define MUTATIONOPERATORS_H_

/**@file
 * Define mutation operator for Milu.
 */

/**@addtogroup mut_operators_gpr Mutation Operator
 * @{
 */

#include <glib.h>
#include "Mutator.h"

/**
 * \brief Alias for MiluMutationOperator
 **/
typedef struct MiluMutationOperator Operator;


/**
 * \brief Define mutation operator for Milu.
 **/
struct MiluMutationOperator
{
	gchar * name; /**< The name of the mutation operator. */
	gchar * info; /**< The information of the mutation operator. */
	GPtrArray * mutators; /**< The set of mutators including. */
};


/**
 * \brief Create a new mutation operator.
 *
 * \param name The name of the mutation operator.
 * \param info The info of the mutation operator.
 *
 * \return A new mutation operator.
 */
Operator * mutation_operator_new(const gchar * name, const gchar * info);

/**
 * \brief Free the given mutation operator.
 *
 * \param op The given mutation operator.
 */
 void mutation_operator_free(Operator * op);

/**
 * \brief Return the size of the given mutation operator.
 *
 * \param op The given mutation operator.
 *
 * \return The size of mutation operator.
 */
guint mutation_operator_size(Operator * op);

/**
 * \brief Print the given mutation operator.
 *
 * \param op The given mutation operator.
 * \param output The output stream.
 */
void mutation_operator_print(Operator * op, FILE * output);

/**
 * \brief Load the all defined mutation operators.
 *
 * @param mut_op_path The mutation operators path.
 *
 * \return All mutation operators defined.
 */
GPtrArray * mutation_operators_load(const gchar * mut_op_path);


/**
 * \brief Free the given set of mutation operators.
 *
 * \param mut_operators The given set of mutation operators.
 */
void mutation_operators_free(GPtrArray * mut_operators);

/**
 * \brief Print the set of given mutation operators.
 *
 * \param mut_operators The given set of mutation operators.
 * \param output The output stream.
 */
void mutation_operators_print(GPtrArray * mut_operators, FILE * output);


/* User defined mutation operator. */
void USER_DEFINED_MUTATION_OPERATOR_BELOW();

Operator * get_OAAN_operator();
Operator * get_OAAA_operator();
Operator * get_ORRN_operator();
Operator * get_OLLN_operator();
Operator * get_OLNG_operator();
Operator * get_OBBN_operator();
Operator * get_OBBA_operator();
Operator * get_CRCR_operator();
Operator * get_OCNG_operator();
Operator * get_OIDO_operator();
Operator * get_SBRC_operator();


Operator * get_Austin_ORRN_operator();
Operator * get_Austin_OAAN_operator();
Operator * get_Austin_OLLN_operator();
Operator * get_Austin_OLNG_operator();
Operator * get_Austin_CRCR_operator();

Operator * get_SSDL_operator();
Operator * get_ABS_operator();
// free

#endif /* MUTATIONOPERATORS_H_ */
