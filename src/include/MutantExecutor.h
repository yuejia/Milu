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

#ifndef MUTANTEXECUTION_H_
#define MUTANTEXECUTION_H_

/**@file
 * Implement execution process for mutation testing.
 */

/**@addtogroup mut_Ex Mutant Executor
 * @{
 */

#include <glib.h>
#include <sys/types.h>
#include <unistd.h>
#include "Mutant.h"
#include "MultiProcessing.h"

/**
 * \brief Define function type for executing mutants.
 **/

typedef void (*MutantExecutionStrategy)(GPtrArray * mutants , Mutant *std, GPtrArray * tests) ;


/**
 * \brief Alias for MutationExecutionStrategyType
 **/
typedef enum MutationExecutionStrategyType StrategyType;



/**
 * \brief Define execution strategy for mutation analysis.
 **/
enum MutationExecutionStrategyType
{
    StrategyMutantFirst = 1,     /**< For each mutant, run all tests until the mutant is killed. */
    StrategyTestFirst = 2,       /**< For each test input, run all alive mutants. */
    StrategyMutantFirstFull = 3, /**< For each mutant, run all tests. */
    StrategyTestFirstFull = 4    /**< For each test input, run all mutants. */
};

/**
 *  \brief Run mutation testing for the given set of mutants.
 *
 *  Customize mutation testing by setting number of process and running strategy
 *
 *  @see MultiCoreProcess
 *  @see StrategyType
 *
 *  \param mutants The given mutants.
 *  \param original The original program.
 *  \param tests The given set of test inputs.
 *  \param strategy_type Specify mutation testing running strategy.
 *
 */
void mutant_executor_run(GPtrArray * mutants, Mutant * original, GPtrArray * tests, StrategyType strategy_type);


#endif /* MUTANTEXECUTION_H_ */

/**@}*/
