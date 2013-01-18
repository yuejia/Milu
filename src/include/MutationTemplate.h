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
 * Implement Mutation Template to generate mutants.
 */

#ifndef MUTATIONTEMPLATE_H_
#define MUTATIONTEMPLATE_H_


/**@addtogroup mut_template Mutation Template
 * @{
 */

#include "MutationOperator.h"
#include "Mutator.h"
#include "ASTUnit.h"
#include "MutationId.h"
#include <glib.h>
#include "Mutant.h"

/**
 * \brief Alias for MutationTemplate.
 *
 * MutaitonTemplate stores all possible mutants at every possible locations of the source code.
 **/
typedef GPtrArray MutationTemplate;

/**
 * \brief Alias for MutationNumTemplate.
 *
 * Number form of MutationTemplate.
 **/
typedef GArray MutationNumTemplate;

/**
 * \brief Return the current MutationTemplate.
 *
 * \return The current MutationTemplate.
 **/
MutationTemplate * mutation_template_get_current();

/**
 * \brief Create mutation template and mutation locations for the given functions.
 *
 *  Apply the given set of mutation operators to the selected list of functions.
 *  Create a mutation location object which stores all possible mutation location and a mutation template object
 *  which stores all possible type of mutation. Use mutation_template_location_free() to free mutation location and template object.
 *
 * \param functions The set of ASTNode represents the selected functions for mutation.
 * \param mut_operators The selected mutation operators.
 * \param mut_locations [out] The location of all possible mutants.
 * \param mut_template [out] The mutation template of stores all possible mutants.
 *
 * @see mutation_template_location_free()
 *
 **/
void milu_mutation_analyse(GPtrArray * functions, GPtrArray * mut_operators, GPtrArray **  mut_locations, GPtrArray ** mut_template);

/**
 * \brief Generate a set of nth order mutants.
 *
 * Genearte a set of nth order mutants from the given mutation number template.
 *
 * \param num_template The given mutaiton number template.
 * \param order The order of mutants
 * \param muts_path The path to save mutants.
 *
 * \return a set of mutants. Free the mutants use mutants_free().
 **/
GPtrArray * mutation_template_generate_mutants(MutationNumTemplate * num_template, gint order, gchar * muts_path);


/**
 * \brief Generate a set of n-m th order mutants randomly.
 *
 * Genearte a set of n-m th order mutants from the given mutation number template.
 *
 * \param num_template The given mutation number template.
 * \param mutants_num Set the number of mutants to generate.
 * \param order_begin The begin order of mutants.
 * \param order_end The end order of mutants.
 * \param muts_path The path to save mutants.
 *
 * \return a set of mutants. Free the mutants use mutants_free().
 **/
GPtrArray * mutation_template_generate_random_mutants(GArray * num_template, gint mutants_num, gint order_begin, gint order_end, gchar * muts_path);

/**
 * \brief Generate a set of n-m th order mutants randomly and try to cover all location of fom.
 *
 * Genearte a set of n-m th order mutants from the given mutation number template.
 *
 * \param num_template The given mutation number template.
 * \param mutants_num Set the number of mutants to generate.
 * \param order The  order of mutants.
 * \param muts_path The path to save mutants.
 *
 * \return a set of mutants. Free the mutants use mutants_free().
 **/
GPtrArray * mutation_template_generate_random_mutants_cover_fom(GArray * num_template, gint mutants_num, gint order, gchar * muts_path);

/**
 * \brief Generate a number form of mutation template.
 *
 * It is easier to use the number form of mutation template to enumerate mutation ids.
 *
 * \param mut_template The given mutation template.
 *
 * \return a mutation number template. Free it use mutation_num_template_free().
 **/
GArray * mutation_template_num_form(GPtrArray * mut_template);

/**
 * \brief Print a given num mutation template.
 *
 * \param mut_num_template The given mutation template.
 * \param output The output stream.
 *
 **/
void mutation_template_num_form_print(GArray * mut_num_template, FILE * output);


/**
 * \brief Apply a given mutation at a give location.
 *
 * \param loc The given location.
 * \param id The type of mutation.
 **/
void mutation_template_apply_mutation(gint loc, gint id);

/**
 * \brief Clean a given mutation at a give location.
 *
 * \param loc The given location.
 * \param id The type of mutation.
 **/
void mutation_template_clean_mutation(gint loc, gint id);

/**
 * \brief Free a given mutation template and mutation location object.
 *
 * \param mut_template The given mutation template.
 * \param mut_locations The given mutation locations.
 *
 **/
void mutation_template_location_free(GPtrArray * mut_template, GPtrArray * mut_locations);

/**
 * \brief Free a mutation number template.
 *
 * \param mut_num_template The given mutation number template.
 *
 **/
void mutation_num_template_free(MutationNumTemplate * mut_num_template);

#endif /* MUTATIONTEMPLATE_H_ */

/**@}*/
