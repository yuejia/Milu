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

#ifndef MILU3_H_
#define MILU3_H_

/**@file
 * Define mutation testing interface for Milu.
 */


/**@addtogroup Milu
 * @{
 */


#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "Utility.h"
#include "Options.h"
#include "Project.h"
#include "MutantExecutor.h"




/**
 *  \brief Initial global settings for Milu.
 */
void milu_init();

/**
 *  \brief Free the global resources for Milu.
 */
void milu_free();


/**
 *  \brief Generate a set of mutants by a given order.
 *
 *  To generate nth order mutants, set order= n.
 *
 *  \param project The given project.
 *  \param order_begin Specify the lower boundary of the order of mutants.
 *  \param order_end Specify the higher boundary of the order of mutants.
 *
 *  \return A set of mutants. Use mutants_free() to free it.
 */
GPtrArray * milu_generate_mutants_by_order(const Project * project,gint order_begin, gint order_end);

/**
 *  \brief Generate a set of mutants by a given order range.
 *
 *  To generate nth order mutants, set order_lower_bound = order_higher_bound = n.
 *
 *  \param project The given project.
 *  \param order_begin Specify the lower boundary of the order of mutants.
 *  \param order_end Specify the higher boundary of the order of mutants.
 *
 *  \return A set of mutants. Use mutants_free() to free it.
 */
GPtrArray * milu_generate_mutants_by_order_range(Project * project, gint order_begin, gint order_end);

/**
 *  \brief Generate a set of mutants randomly.
 *
 *  To generate nth order mutants randomly, set order_lower_bound = order_higher_bound = n.
 *
 *  \param project The given project.
 *  \param mutants_num The number of mutants to generate.
 *  \param order_begin Specify the lower boundary of the order of mutants.
 *  \param order_end Specify the higher boundary of the order of mutants.
 *
 *  \return A set of mutants. Use mutants_free() to free it.
 */
GPtrArray * milu_explore_mutants_by_random(const Project * project, gint mutants_num, gint order_begin, gint order_end);

/**
 *  \brief Generate a set of mutants randomly  and try to cover all fom locations.
 *
 *  To generate nth order mutants randomly, set order_lower_bound = order_higher_bound = n.
 *
 *  \param project The given project.
 *  \param mutants_num The number of mutants to generate.
 *  \param order_begin Specify the lower boundary of the order of mutants.
 *  \param order_end Specify the higher boundary of the order of mutants(not using).
 *
 *  \return A set of mutants. Use mutants_free() to free it.
 */
GPtrArray * milu_explore_mutants_by_random_cover_fom(const Project * project, gint mutants_num, gint order_begin, gint order_end);

/**
 *  \brief Run mutation testing for the given set of mutants.
 *
 *  Customize mutation testing by setting number of process and running strategy
 *
 *  @see MultiCoreProcess
 *  @see StrategyType
 *
 *  \param project The given project.
 *  \param mutants The given mutants.
 *  \param strategy_type Specify mutation testing running strategy.
 *
 */
void milu_run_mutation_testing(Project * project, GPtrArray * mutants, StrategyType strategy_type);

/**
 *  \brief Save the given set of mutants.
 *
 *  \param mutants The given set of mutants to save.
 *
 */
void milu_save_mutants(GPtrArray * mutants);

/**
 * \brief Print the summary of results.
 *
 * \param project The project to print.
 * \param mutants The given set of mutants.
 * \param output The output stream.
 *
 **/
void milu_print_results(const Project * project, GPtrArray * mutants, FILE * output);


/**
 * \brief Print the killings detail.
 *
 * \param project The project to print.
 * \param mutants The given set of mutants.
 * \param output The output stream.
 *
 **/
void milu_print_killing_results(const Project * project, GPtrArray * mutants, FILE * output);

/**
 * \brief Compile mutants.
 *
 * \param The given set of mutants to compile.
 *
 **/
void milu_compile_mutants(GPtrArray * mutants);

/**
 * \brief Apple TCE to check equivalent mutants.
 *
 * \param The given set of mutants to check.
 * \param The original mut.
 *
 **/
void milu_check_equivalent_mutants(GPtrArray * mutants, Mutant * std);

/**
 * \brief Apple TCE to check duplicated mutants.
 *
 * \param The given set of mutants to check.
 *
 **/
void milu_check_duplicated_mutants(GPtrArray * mutants);


/**
 * \brief Search higher order mutants
 *
 * \param project The given project.
 * \param project The number of mutants to generate.
 *
 **/
GPtrArray * milu_search_hom(const Project * project, gint mutants_num , gint strategy, gint order_begin, gint order_end);


/**
 *  \brief Load mutants from mid file.
 *
 *
 */
GPtrArray * milu_restore_mutants_from_mid_file(Project * project, gchar * mid_file);

#endif /* MILU3_H_ */

/**@}*/
