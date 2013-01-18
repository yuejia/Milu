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

#ifndef PROJECT_H_
#define PROJECT_H_

/**@file
 * Define mutation testing interface for Milu.
 */

/**@addtogroup project_gpr Project
 * @{
 */

#include <glib.h>
#include <glib/gprintf.h>
#include "Utility.h"
#include "ASTUnit.h"
#include "Options.h"
#include "TestInput.h"
#include "MutationOperator.h"
#include "MutationTemplate.h"
#include "MutantExecutor.h"


/**
 * \brief Alias for MiluProject
 **/
typedef struct MiluProject Project;

/**
 * \brief Define MiluProject data type.
 *
 * MiluProject stores the project
 * information.
 *
 **/
struct MiluProject
{
	gchar * source_path; /**< The source file path under test. */
	gchar * output_path; /**< The output path for milu.*/
    gchar * tests_path; /**< The path for input tests. */
    gchar * test_driver_path; /**< The path for test driver file. */
    gchar * compilation_cmd; /**< The command to compile mutants. */
    gchar * mutants_path; /**< The path for mutants. */
    ASTUnit * ast_unit; /**< The ASTUnit of the input source file. */
    GPtrArray * mutation_operators; /**< The set of mutation operators for current project. */
    GPtrArray * mutation_locations; /**< The locations for all mutants. */
    MutationTemplate * mutation_template ; /**< The mutation template for all mutants. */
    MutationNumTemplate * mutation_num_template; /**< The mutation template for all mutants in numeric form. */
    GPtrArray * test_inputs; /**< The set of test inputs. */
    GPtrArray * func_nodes; /**< The set of selected funciton nodes. */
    Mutant * original_program; /**< The mutant object representing the original program. */
};


/**
 * \brief Create a MiluProject object.
 *
 * \param source_path The source file path under test.
 * \param output_path The output path for milu.
 * \param test_driver_path The path for test driver file.
 * \param tests_path The path for input tests.
 * \param compilation_cmd The command to compile mutants.
 * \return a new MiluProject object. Use milu_project_free() to free it.
 *
 * @see milu_project_free()
 * @see milu_project_print()
 **/
Project * milu_project_new(const gchar * source_path, const gchar * output_path, const gchar * test_driver_path, const gchar * tests_path, const gchar * compilation_cmd);


/**
 * \brief Analyse the mutation testing.
 *
 * \param project The give project to analyse.
 *
 **/
void milu_project_analyse_template(Project * project);

/**
 * \brief Free the given MiluProject object.
 *
 * \param project The project to be free.
 *
 **/
void milu_project_free(Project * project);


/**
 * \brief Print the given MiluProject object.
 *
 * \param project The project to print.
 * \param mode The print mode.
 * \param output The output stream.
 *
 **/
void milu_project_print(const Project * project, PrintMode mode, FILE * output);


/**
 * \brief Compile the original program with the given driver.
 *
 *  In the mutants_path is changed, call this method to recompile the original program.
 *
 * \param project The project to save original program.
 *
 **/
void milu_project_prepare_original_program(Project * project);


/**
 * \brief Check if the project is ready to run mutants.
 *
 * \param project The project to check.
 *
 * \return TRUE if test inputs and test driver are given. *
 *
 **/
gboolean milu_project_is_executable(const Project * project);

/**
 * \brief Load function nodes by given file.
 *
 * If func_path is NULL, load all the user defined function.
 *
 * \param project The current project.
 *
 **/
void milu_project_load_function_settings(Project * project, const gchar * func_path);

/**
 * \brief Load the mutation operator settings by given file.
 *
 * If mut_op_path is NULL, load mutation operators.
 *
 * \param project The current project.
 *
 **/
void milu_project_load_mut_operators_settings (Project * project, const gchar * mut_op_path);

/**
 * \brief Save a simple html frame to view mutants.
 *
 * \param project The give project.
 * \param mutants The give set of project to save.
 */

void milu_project_save_html_output(Project * project, GPtrArray * mutants);

/**
 * \brief Save the mids of current mutants to a file.
 */
void milu_project_save_mid(Project * project, GPtrArray * mutants);
#endif /* PROJECT_H_ */


/**@}*/
