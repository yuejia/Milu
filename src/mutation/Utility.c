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

#include "Utility.h"

GStringChunk *   MiluStringPool;
GRand * MiluGRand;
gint MiluRandomMutantsTrialTimes = 1000000;

// Public function implementation

void milu_utility_check_dir(gchar * path)
{
	GString * cmd = g_string_new("");
	if (g_file_test(path, G_FILE_TEST_EXISTS))
	{
		if (g_file_test (path, G_FILE_TEST_IS_DIR))
		{
			g_string_printf(cmd,"rm -r %s",path);
			system(cmd->str);
		}
		else
		{
			g_log ("Milu",G_LOG_LEVEL_WARNING,"check dir error: %s", path) ;
		}
	}

	g_string_printf(cmd,"mkdir %s",path);

	system(cmd->str);
	g_string_free(cmd, TRUE);
}


GPtrArray * milu_utility_load_text_file_to_gptrarray(const gchar * path)
{
    g_assert(path && "Invalid arguments!");

	GPtrArray * test_inputs = g_ptr_array_new();
	gchar * contents;
	gsize len;
	if(!g_file_get_contents(path, &contents, &len, NULL))
	{
		g_log ("Milu",G_LOG_LEVEL_ERROR,"Cannot load the inputs file. %s", path) ;
	}
	gchar ** inputs  = g_strsplit (contents, CR, -1);
	gchar ** curr_input = inputs;
    while(*curr_input)
    {
        gchar * tmp_input = (*curr_input)[0] != '\0' ? g_string_chunk_insert (MiluStringPool, *curr_input) : NULL;
        if (tmp_input != NULL)
            g_ptr_array_add(test_inputs, tmp_input);
    	curr_input++;
    }
	g_strfreev (inputs);
    g_free(contents);
    return test_inputs;
}

GPtrArray * milu_utility_split_gptrarray(GPtrArray * gp, gint num)
{
    g_assert(gp && num > 0 && gp->len >= num && "Array splitting error");

    GPtrArray * sgps = g_ptr_array_new();

    gint n = gp->len / num;
    gint curr_i = 0;
    for(gint i = 0 ; i< num ; i ++)
    {
        GPtrArray * sgp = g_ptr_array_new();
        for (gint j = 0; j < n; j++)
        {
            curr_i = i*n+j;
            g_ptr_array_add(sgp, g_ptr_array_index(gp, curr_i));
        }
         g_ptr_array_add(sgps, sgp);
    }
    gint j = 0;
    for(gint i = curr_i + 1 ; i < gp->len ; i++)
    {
        GPtrArray * sgp = g_ptr_array_index(sgps, j);
        g_ptr_array_add(sgp, g_ptr_array_index(gp, i));
        j++;
    }

    return sgps;
}

void  milu_utility_append_gptrarray(GPtrArray * gp_a, GPtrArray * gp_b )
{
    g_assert(gp_a && gp_b && "Appending array error!");
     for(gint i = 0 ; i < gp_b->len ; i ++)
     {
    	g_ptr_array_add(gp_a, g_ptr_array_index(gp_b, i));
     }
}

GString * milu_utility_source_to_html(GString * src)
{
    GString * html = g_string_new("");
 //   src = g_string_prepend(src, "<html>");
 //   g_string_append_printf(src, "</html>");

    //TODO maybe free src
    return html;
}
