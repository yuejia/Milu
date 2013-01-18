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

#ifndef UTILITY_H_
#define UTILITY_H_

/**@file
 * Implement utilities for Milu.
 */

/**@addtogroup Utilities
 * @{
 */

#include <glib.h>
#include <stdlib.h>


/**
 *  \brief Size of string pool.
 */
#define MILU_STRING_POOL_SIZE 1024

/**
 *  \brief String pool for Milu.
 */
extern GStringChunk *   MiluStringPool;


/**
 *  \brief Use to generate random numbers for Milu.
 */
extern GRand * MiluGRand;


/**
 * \brief Check the given dir.
 *
 * Create a new dir if the dir does not exist.
 * If the dir exists, remove the old first and then create a new one.
 *
 * \param path The dir path to check
 *
 **/
void milu_utility_check_dir(gchar * path);

/**
 * \brief Alias for MiluPrintMode
 **/
typedef enum MiluPrintMode PrintMode;

/**
 * \brief Max number of trial times for random mutant generation.
 **/
extern gint MiluRandomMutantsTrialTimes;

/**
 * \brief Define print mode for Milu.
 **/
enum MiluPrintMode
{
    PrintSource,     /**< Print out in the source code format. */
    PrintDot,       /**< Print out in dot format. */
    PrintHtml,      /**< Print out in html format. */
    PrintDefault,   /**< Print the default mode. */
};


/**
 * \brief Load a text file and store the content to a GPtrArray line by line.
 *
 * \param path The file to load.
 *
 * \return An GPtrArray store the content of the file
 **/
GPtrArray * milu_utility_load_text_file_to_gptrarray(const gchar * path);


/**
 * \brief Split a GPtrArray elements into n arrays.
 *
 * \param gp The given GPtrArray.
 * \param num The number want to split.
 *
 * \return An GPtrArray store the spllited arrays and it should be free by user.
 **/
GPtrArray * milu_utility_split_gptrarray(GPtrArray * gp, gint num);

/**
 * \brief Append a GPtrArray elements into another.
 *
 * \param gp_a The given GPtrArray to append.
 * \param gp_b The given GPtrArray to be appended.
 *
 **/
void  milu_utility_append_gptrarray(GPtrArray * gp_a, GPtrArray * gp_b );

/**
 * \brief Convert source code format to html format.
 *
 * \param src The given source code.
 *
 **/
GString * milu_utility_source_to_html(GString * src);

#endif /* UTILITY_H_ */
/**@}*/
