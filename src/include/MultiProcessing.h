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

#ifndef MULTIPROCESSING_H_
#define MULTIPROCESSING_H_


/**@file
 * Set multiprocessing for Milu.
 */

/**@addtogroup Multiprocessing
 * @{
 */

#include <glib.h>
#include "Utility.h"

/**
 * \brief Alias for MultiProcess function run mutants.
 **/
typedef void (*MultiProcessingMutantsFunc)(GPtrArray *) ;

/**
 * \brief Alias for MiluMultiProcess
 **/
typedef enum MiluMultiProcess MultiProcess;


/**
 * \brief Define the multiprocessing mode to run mutants.
 **/
enum MiluMultiProcess
{
    MultiProcessNo = 0, /**< Run mutants using the same process as Milu.  */
    MultiProcessOne = 1, /**< Run mutants using one additional process in parallel. */
    MultiProcessTwo = 2, /**< Run mutants using two additional processes in parallel. */
    MultiProcessFour = 4, /**< Run mutants using sour additional processes in parallel. */
    MultiProcessSix = 6, /**< Run mutants using six additional processes in parallel. */
    MultiProcessEight = 8, /**< Run mutants using eight additional processes in parallel. */
    MultiProcessResearchEight = 80
};

/**
 * \brief Set the multiprocessing mode to run mutants.
 *
 * \param process: Specify the multiprocessing mode.
 *
 * @see MiluMultiProcessing
 **/
void milu_set_multi_process(MultiProcess mp);

/**
 * \brief Get the current multiprocessing mode.
 *
 * \return The current multiprocessing mode.
 *
 * @see MiluMultiCoreProcess
 **/
MultiProcess milu_get_multi_process();



/**
 * \brief Spawn a new process to run mutants function.
 *
 * \param mutants The given set of mutants.
 * \param mfun The function to run using a new process.
 *
 **/
void milu_multi_process_mutants(GPtrArray * mutants, MultiProcessingMutantsFunc mfunc);

#endif /* MULTICOREPROCESS_H_ */
/**@}*/
