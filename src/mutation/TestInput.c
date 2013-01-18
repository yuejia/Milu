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

#include "TestInput.h"

// Public function implementation

GPtrArray * test_inputs_load(const gchar * tests_path)
{
    g_assert(tests_path != NULL);
    return milu_utility_load_text_file_to_gptrarray(tests_path);
}

//TODO: fix global
gboolean	PARSING_UNITTESTS = FALSE;
GPtrArray * process_unit_testing_driver(const gchar * unit_tests_path)
{

	PARSING_UNITTESTS = TRUE;
	ASTUnit * au = ASTUnit_new(unit_tests_path) ;
	GPtrArray *  func_list = ASTUnit_get_current_func_list();

	GString * driver_text= g_string_sized_new((gsize)102400);
	GString * test_text= g_string_sized_new((gsize)102400);

	g_string_append_printf(driver_text,"%s","#include <stdio.h>\n#include <stdlib.h> \n");
//	g_string_append_printf(driver_text,"%s","#include <stdio.h>\n");

	GPtrArray * utest = milu_utility_load_text_file_to_gptrarray(unit_tests_path);
	   for(gint i = 0 ; i < utest-> len ; i ++)
	   {
	    	gchar * curr_line = g_ptr_array_index(utest,i);
	    	g_string_append_printf(driver_text,"%s",curr_line);
	    	g_string_append_printf(driver_text,"%s","\n");
	   }

	   g_string_append_printf(driver_text,"int main(int argc, char * argv[])\n {\n");

	   g_string_append_printf(driver_text,"alarm(1);\n");
	   for(gint i = 0 ; i < func_list-> len ; i ++)
	    {
	    	ASTNode * curr_func = g_ptr_array_index(func_list, i);
	    	if(is_ASTNode_func_decl_with_body(curr_func))
	    	{
	    	g_string_append_printf(test_text,"%d\n", i);
	    	g_string_append_printf(driver_text,"if( atoi(argv[1]) == %d)\n", i);

	    	g_string_append_printf(driver_text,"fprintf(stdout, \"%%d\",%s());\n",curr_func->text);
//	    	printf("%s\n", curr_func->text);
	    	}
	    }
	   g_string_append_printf(driver_text,"exit(0);}\n");

//	   printf("%s\n", driver_text->str);
//	   printf("%s\n", test_text->str);

	   FILE * dfile = fopen("milu_driver.c","w");
	   dfile == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"cannot write milu driver %s", "milu_driver.c") : 0;
	   g_fprintf(dfile,"%s", driver_text->str);
	   fclose(dfile);

	   FILE * tfile = fopen("milu_test","w");
	   tfile == NULL ? g_log ("Milu",G_LOG_LEVEL_ERROR,"cannot write milu test %s", "milu_test") : 0;
	   g_fprintf(tfile,"%s", test_text->str);
	   fclose(tfile);

	PARSING_UNITTESTS = FALSE;
	   return test_inputs_load("milu_test");
}
void test_inputs_free(GPtrArray * test_inputs)
{
	g_assert(test_inputs != NULL);
	g_assert(test_inputs->len > 0);
	g_ptr_array_free(test_inputs, TRUE);
}
