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

#ifndef TESTINPUT_H_
#define TESTINPUT_H_

/**@file
 * Implement utilities for Milu.
 */

/**@addtogroup test_input Test inputs
 * @{
 */

#include <glib.h>
#include "ASTUnit.h"
#include "ASTNode.h"
#include "ASTPrinter.h"
#include "ASTNodeUtil.h"
#include "Utility.h"

/**
 * \brief Load the test inputs form the given path.
 *
 * \param tests_path The path for test inputs.
 *
 * \return A set of test inputs. Use test_inputs_free() to free it.
 **/
GPtrArray * test_inputs_load(const gchar * tests_path);

/**
 * \brief Load the test inputs form a unit test.
 *
 * \param tests_path The path for unit test inputs.
 *
 * \return A set of test inputs. Use test_inputs_free() to free it.
 **/
GPtrArray * process_unit_testing_driver(const gchar * unit_tests_path);

/**
 * \brief Free the given test inputs.
 *
 * \param test_inputs The set of test inputs to be free.
 *
 **/
void test_inputs_free(GPtrArray * test_inputs);

#endif /* TESTINPUT_H_ */

/**@}*/
