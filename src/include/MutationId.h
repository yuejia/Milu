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

/**@file
 * Define IDs of mutants, which are generated from mutation template.
 */

#ifndef MUTATIONTEMPLATEID_H_
#define MUTATIONTEMPLATEID_H_

/**@addtogroup mut_template Mutation Template
 * @{
 */

#include <glib.h>
#include "Utility.h"
#include <glib/gprintf.h>

/**
 * \brief Alias for MutationId.
 * MutationId is generated from mutation template.
 *
 **/
typedef GArray MutationId;

/**
 * \brief Alias for MutationCompactId.
 * MutationCompactId is generated from mutation template. It represents a set of MutationId at the same locations.
 *
 **/
typedef GArray MutationCompactId;

/**
 * \brief Create a MutationId or a MutationCompactId.
 *
 * \return a new MutationId. Use mutation_id_free() to free it.
 *
 * @see mutation_id_free()
 * @see mutation_id_print()
 **/
MutationId * mutation_id_new();

/**
 * \brief Create a MutationId from given text form.
 *
 * \return a new MutationId. Use mutation_id_free() to free it.
 *
 * @see mutation_id_free()
 * @see mutation_id_print()
 **/
MutationId * mutation_id_new_from_text(gchar * mid_text);

/**
 * \brief Free the given MutationId.
 *
 * \param mid The MutationId to be free.
 **/
void mutation_id_free(MutationId * mid);

/**
 * \brief Free the given set of MutationIds.
 *
 * \param mids The set of MutationIds to be free.
 *
 **/
void mutation_ids_free(GPtrArray * mids);


/**
 * \brief Generate compact mutation ids for nth order mutants from a mutation number template.
 *
 * \param num_template The number form of mutation temple.
 * \param order The order of mutants to generate.
 *
 * \return A set of mutation compact ids for nth order mutants.
 **/
GPtrArray * mutation_compact_ids_get_nth_order(GArray * num_template, gint order);

/**
 * \brief Generate mutation ids from mutation compact ids.
 *
 * \param mcids The given set of mutation compact id.
 *
 * \return A set of mutation ids expended from the given compact ids.
 **/
GPtrArray * mutation_compact_ids_expend(GPtrArray * mcids);

/**
 * \brief Generate mutation ids for all first order mutants from a mutation number template.
 *
 * \deprecated To avoid memory overload, Milu3 generates mutation ids from mutation compact ids.
 *
 * \param num_template The number form of mutation temple.
 *
 * \return A set of mutation ids for all first order mutants.
 **/
GPtrArray * mutation_ids_get_1st_order(GArray * num_template);

/**
 * \brief Generate mutation ids nth order mutants.
 *
 *  The nth order mutation ids are generated from the (n-1)th order mutation ids.
 *
 *  \deprecated To avoid memory overload, Milu3 generates mutation ids from mutation compact ids.
 *
 * \param mids The (n-1)th order mutation ids.
 * \param fom_mids The 1th order mutation ids.
 *
 * \return Nth order mutation ids.
 **/
GPtrArray * mutation_ids_get_next_order(GPtrArray * mids, GPtrArray * fom_mids );

/**
 * \brief Print the given MutationId object.
 *
 * \param mid The MutationId to print.
 * \param output The output stream.
 *
 **/
void mutation_id_print(MutationId * mid, FILE * output);


/**
 * \brief Print the given set of MutationIds.
 *
 * \param mids The set of MutationId to print.
 * \param output The output stream.
 *
 **/
void mutation_ids_print(GPtrArray * mids, FILE * output);


/**
 * \brief From a mutation number template, generate a nth order mutation randomly.
 *
 * \param num_template The number form of mutation temple.
 * \param order The order of mutants to generate.
 *
 * \return A mutation id for a nth order mutant.
 **/
GArray * mutation_id_get_random_nth_order(GArray * num_template, gint order);

/**
 * \brief From a mutation number template, generate mutation ids for m-nth order mutants randomly.
 *
 * \param num_template The number form of mutation temple.
 * \param order_begin The begin order of mutants to generate.
 * \param order_end The end order of mutants to generate.
 * \param mutants_num Set the number of mutants to generate.
 *
 * \return A set of mutation ids for n-m th order mutants.
 **/
GPtrArray * mutant_ids_get_random_nth_order(GArray * num_template, gint mutants_num, gint order_begin, gint order_end);


/**
 * \brief From a mutation number template, generate mutation ids for m-nth order mutants randomly but try to cover all mutation locations.
 *
 * \param num_template The number form of mutation temple.
 * \param order_begin The begin order of mutants to generate.
 * \param order_end The order of mutants to generate.
 *
 * \return A set of mutation ids for n-m th order mutants.
 **/
GPtrArray * mutant_ids_get_random_nth_order_cover_fom(GArray * num_template, gint mutants_num, gint order);

//TODO: add commment
 gboolean mutant_ids_is_equal_2_ordered_mids(GArray * mid1, GArray * mid2);
#endif /* MUTATIONTEMPLATEID_H_ */

/**@}*/
