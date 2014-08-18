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

#ifndef MUTANT_H_
#define MUTANT_H_

/**@file
 * Implement mutants generation.
 */

/**@addtogroup Mutants
 * @{
 */

#include "Mutator.h"
#include "MutationTemplate.h"
#include "MutationId.h"
#include "ASTUnit.h"
#include "Options.h"
#include "Utility.h"
#include <string.h>
#include <glib.h>
#include <stdlib.h>

typedef struct MiluMutant Mutant;

/**
 * \brief Define the mutant representation.
 **/
struct MiluMutant
{
	MutationId * id; /**< The Mutation Id. */
	gboolean compilable; /**< Is Mutant compilable. */
	gboolean iskilled; /**< Is Mutant is killed. */
    gboolean isequivalent;  /**< Is Mutant is equivalent. */
	gint file_id;  /**< The file id. */
	gchar * base_path;  /**< The file id. */
	gchar * src_path;  /**< The source path. */
	gchar * html_path;  /**< The html path. */
	gchar * bin_path;   /**< The binary path. */
	gchar * output_path;   /**< The output path. */
    GArray * results;   /**< The results. */
    GPtrArray * foms;  /**< The foms constructing HOMs */
    gdouble fitness; /** < Fitness value */

		// old
//	gchar * path;
//		gchar * type;
//		PtrNode mut_func_root;
//		gint file_id;
//		gint func_id;
//		gint stat_id;
//		gchar * mut_path;
//		gchar * std_path;

//		gboolean is_killed;
//		GPtrArray * mtemplate;

	//	gint * id;		/**< The token is going to be mutated. */
//		GArray * id;		/**< The token is going to be mutated. */
//		gint id_len; /**< length of the id */
//		gint * result;	/**< Kill result. */
//		gint result_len; /**< length of the result */
//		gint killvalue; /**< Kill ability. */
//		gint survvalue; /**< Survvalue. */
//		gdouble fitness; /**< fitness value. */

//		gint order;
//		gdouble fitness1; /**< subsuming fitness value. */
//		gdouble fitness2; /**< strong fitness value. */
//	MutantList fom_list; /**< fom list. */

};

//void mutant_free();
void mutant_save(Mutant * mut);
void mutant_save_html(Mutant * mut);
void mutant_save_id(Mutant * mut, FILE * output);

void mutants_save_id(GPtrArray * Muts, FILE * output);

//void mutant_compile(gchar * cmd);
//void mutant_run(gchar * bin);
Mutant * mutant_new(MutationId * id, gint file_id, gchar * base_path);
GPtrArray * mutants_new(GPtrArray * ids, gchar * muts_path);
void mutants_save(GPtrArray * muts);
gchar * mutant_get_src_path(gchar * mut_path);
void  mutant_check_dir(gchar * path);
void mutant_print(Mutant * mut, FILE * output);
void mutants_print(GPtrArray * muts, FILE * output);

void mutant_compile(Mutant * mut, gchar * command, gchar * driver);
void mutants_compile(GPtrArray * muts);

gboolean mutant_run(Mutant * mut, Mutant * std, gchar * test);
Mutant * mutant_new_original(gchar * base_path);
void mutant_set_killed(Mutant * mut);


gboolean mutant_is_killed(Mutant * mut);
gboolean mutant_is_compilable(Mutant * mut);
gboolean mutant_is_equivalent(Mutant * mut);

gfloat mutants_cal_mutation_score(GPtrArray * mutants);
gint mutants_get_killed_number(GPtrArray * mutants);
gint mutants_get_non_equivalent_number(GPtrArray * mutants);

void mutant_results_clean(Mutant * mut);
gboolean mutant_has_results(Mutant * mut);
void mutants_results_clean(GPtrArray * mutants);

void mutant_results_set_kill(Mutant * mut);
void mutant_results_set_not_kill(Mutant * mut);

void mutant_results_save(Mutant * mut, FILE * output);
void mutants_results_save(GPtrArray * mutants, FILE * output);

void mutant_results_load(Mutant * mut, gchar * results);
void mutants_results_load(GPtrArray * mutants, gchar * path);
void mutants_free(GPtrArray * mutants);


gboolean mutant_check_compiled(Mutant * mut);
void mutants_check_compiled(GPtrArray * muts);
void mutants_append_mutants(GPtrArray * mutantsA, GPtrArray * mutantsB);

void mutant_evaluate_subsuming_fitness(Mutant * mut, GPtrArray * foms);
void mutant_evaluate_strongly_subsuming_fitness(Mutant * mut, GPtrArray * foms);
#endif /* MUTANT_H_ */

/**@}*/
