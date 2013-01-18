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


#include "MultiProcessing.h"

extern gboolean  MILU_GLOBAL_VERBOSE;
static MultiProcess Milu_MuliProcess = MultiProcessNo;

// Private function interface

static void create_multi_process_mutants(GPtrArray * mutants, MultiProcessingMutantsFunc mfunc);

// Public function implementation

void milu_set_multi_process(MultiProcess mp)
{
	MILU_GLOBAL_VERBOSE ? g_log ("Milu",G_LOG_LEVEL_MESSAGE,"Using %d processes", mp) : 0;
    Milu_MuliProcess = mp;
}

MultiProcess milu_get_multi_process()
{
    return Milu_MuliProcess;
}

void milu_multi_process_mutants(GPtrArray * mutants, MultiProcessingMutantsFunc mfunc)
{
	 g_assert(Milu_MuliProcess>=0);

	 if (Milu_MuliProcess == MultiProcessNo)
	 {
		 mfunc(mutants);
	 }
	 else
	 {
		 int s = 0;
         GPtrArray * smuts = milu_utility_split_gptrarray(mutants,Milu_MuliProcess);
		 for(gint i = 0 ; i < Milu_MuliProcess; i++)
		 {
			 create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, i), mfunc);
		 }
		 for(gint i = 0 ; i < Milu_MuliProcess; i++)
		 {
			 wait(&s);
		 }
		 g_ptr_array_free(smuts, TRUE);
	 }
}
	/* old
void milu_multi_process_mutants(GPtrArray * mutants, MultiProcessingMutantsFunc mfunc)
{
	 switch(Milu_MuliProcess)
	 {
	 case MultiProcessNo:
	 {
		 mfunc(mutants);
		 break;
	 }
	 case MultiProcessOne:
	 {
         create_multi_process_mutants(mutants, mfunc);
		 int s = 0;
		 wait(&s);
		 break;
	 }
	 case MultiProcessTwo:
	 {
		 int s = 0;
         GPtrArray * smuts = milu_utility_split_gptrarray(mutants,2);
         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 0), mfunc);
         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 1), mfunc);
		 wait(&s);
		 wait(&s);
		 g_ptr_array_free(smuts, TRUE);
         break;
	 }
	 case MultiProcessFour:
	 {
		 int s = 0;
         GPtrArray * smuts = milu_utility_split_gptrarray(mutants,4);
         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 0), mfunc);
         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 1), mfunc);
         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 2), mfunc);
         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 3), mfunc);
		 wait(&s);
		 wait(&s);
		 wait(&s);
		 wait(&s);
		 g_ptr_array_free(smuts, TRUE);
         break;
	 }
	 case MultiProcessSix:
		 {
			 int s = 0;
	         GPtrArray * smuts = milu_utility_split_gptrarray(mutants,6);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 0), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 1), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 2), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 3), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 4), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 5), mfunc);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 g_ptr_array_free(smuts, TRUE);
	         break;
		 }
	 case MultiProcessEight:
		 {
			 int s = 0;
	         GPtrArray * smuts = milu_utility_split_gptrarray(mutants,8);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 0), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 1), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 2), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 3), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 4), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 5), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 6), mfunc);
	         create_multi_process_mutants((GPtrArray*) g_ptr_array_index(smuts, 7), mfunc);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 wait(&s);
			 g_ptr_array_free(smuts, TRUE);
	         break;
		 }
	 default:
	 {
		   g_log ("Milu",G_LOG_LEVEL_WARNING,"Non supporting  process number.") ;
		   mfunc(mutants);
		   break;
	 }

	 }
}

*/
// Private function implementation

static void create_multi_process_mutants(GPtrArray * mutants, MultiProcessingMutantsFunc mfunc)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot spawn process.") ;
	}
	else if (pid == 0)
	{
		mfunc(mutants);
		exit(0);
	}
}
