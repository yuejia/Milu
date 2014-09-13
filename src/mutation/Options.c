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

#include "Options.h"
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>

gboolean MILU_GLOBAL_VERBOSE = FALSE;
gboolean MILU_PRINT_MUT_TYPE= FALSE;
static gint MP_NUM = -1;
static gint MILU_MUTANT_ORDER = 1;
static gint MILU_MUTANT_ORDER_BEGIN = -1;
static gint MILU_MUTANT_ORDER_END = -1;
static gchar * MILU_TEST_INPUT_PATH=NULL;
static gchar * MILU_MUT_OPERATORS_PATH=NULL;
static gchar * MILU_TEST_DRIVER_PATH=NULL;
static gchar * MILU_OUPUT_PATH=NULL;
static gchar * MILU_COMPILE_CMD=NULL;
static gchar * MILU_DEBUG = NULL;
static gchar * MILU_MUTANT_STRATEGY = NULL;
static gchar * MILU_SEARCH_STRATEGY = NULL;
static gchar * MILU_MID_PATH = NULL;
static gint MILU_MUTANT_RAND_LIMIT = 1000;
static gchar * MILU_EXEC_STRATEGY = NULL;
static gchar * MILU_FUNC_LIST_PATH = NULL;
static gchar * MILU_FUNC_NAME= NULL;
static gchar * MILU_TAG_LIST_PATH = NULL;
static gboolean MILU_HTML_VIEW = FALSE;
static gboolean MILU_SAVE_MID = FALSE;
static gboolean MILU_HTML_SAVE_ALL_MUT = FALSE;
static gchar * MILU_SRC_PATH = NULL;
gboolean MILU_NOT_SAVE_MUT = FALSE;
gboolean MILU_SAVE_KILLING_RESULT = FALSE;

gboolean MILU_AUSTIN_TRANSFORM = FALSE;
gboolean MILU_UNIT_TEST = FALSE;
gboolean MILU_KILLING_RESULT= FALSE;
gboolean MILU_TCE_OPTIMISATION= FALSE;

static  GOptionEntry entries[] =
{
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &MILU_GLOBAL_VERBOSE, "Be verbose.", NULL },
  { "TCE", 0, 0, G_OPTION_ARG_NONE, &MILU_TCE_OPTIMISATION, "Apply TCE Otimisation.", NULL },
  { "print-mutants-type", 0, 0, G_OPTION_ARG_NONE, &MILU_PRINT_MUT_TYPE, "Print a list of mutation operators applied.", NULL },
  { "unit-test", 'u', 0, G_OPTION_ARG_NONE, &MILU_UNIT_TEST, "Unit test case.", NULL },
  { "killing-result", 'k', 0, G_OPTION_ARG_NONE, &MILU_KILLING_RESULT, "Get Milu Killing results.", NULL },
  { "save-killing-result", 0, 0, G_OPTION_ARG_NONE, &MILU_SAVE_KILLING_RESULT, "SAVE Milu Killing results.", NULL },
  { "austin", 'a', 0, G_OPTION_ARG_NONE, &MILU_AUSTIN_TRANSFORM, "Generate Austin-Milu version of the mutants.", NULL },
  { "multiprocess", 'p', 0, G_OPTION_ARG_INT, &MP_NUM, "Specify the number of additional processes to run mutants. Multiprocessing is turned off by default.", "P (1,2,4)" },
  { "test-input", 't', 0, G_OPTION_ARG_STRING, &MILU_TEST_INPUT_PATH, "Set the path to test input file. Milu only generate mutants if there is no test inputs.", "path" },
  { "mut-operators", 'm', 0, G_OPTION_ARG_STRING, &MILU_MUT_OPERATORS_PATH, "Set the path to mutation operators filter file. Milu load all supported operators without a given path.", "path" },
  { "test-driver", 'd', 0, G_OPTION_ARG_STRING, &MILU_TEST_DRIVER_PATH, "Set the path to test driver file. Milu only generate mutants if there is no test driver", "path" },
  { "compile-cmd", 'c', 0, G_OPTION_ARG_STRING, &MILU_COMPILE_CMD, "Set test the command to compile mutants, e.g. gcc -lm", "C" },
  { "output", 'o', 0, G_OPTION_ARG_STRING, &MILU_OUPUT_PATH, "Set the milu output path. The default output path is milu_output", "path" },
  { "debug", 0, 0, G_OPTION_ARG_STRING, &MILU_DEBUG, "Print out Milu AST in source format or dot format immediately after parsing.", "[dot,src]"},
  { "order", 0, 0, G_OPTION_ARG_INT, &MILU_MUTANT_ORDER, "Set the order of mutants to generate. The default value is 1.", "N"},
  { "order_begin", 0, 0, G_OPTION_ARG_INT, &MILU_MUTANT_ORDER_BEGIN, "Set the begin order of mutants to generate.", "N"},
  { "order_end", 0, 0, G_OPTION_ARG_INT, &MILU_MUTANT_ORDER_END, "Set the end order of mutants to generate.", "N"},
  { "mut-strategy", 0, 0, G_OPTION_ARG_STRING, &MILU_MUTANT_STRATEGY, "Set the strategy to generate mutants.", "[rand,restore...]"},
  { "search-strategy", 0, 0, G_OPTION_ARG_STRING, &MILU_SEARCH_STRATEGY, "Set the strategy to search higher order mutants.", "[rand,...]"},
  { "exec-strategy", 0, 0, G_OPTION_ARG_STRING, &MILU_EXEC_STRATEGY, "Set the strategy to execute mutants.", "[mut-1st, test-1st, mut-1st-full, test-1st-full]"},
  { "rand-limit", 0,0, G_OPTION_ARG_INT, &MILU_MUTANT_RAND_LIMIT, "Set the max number of mutants to generate in random mode. The default value is 1000.", "N"},
  { "func-list", 'f',0, G_OPTION_ARG_STRING, &MILU_FUNC_LIST_PATH, "Set the path to a file, selecting a set of functions to mutate.", "path"},
  { "func-name", 0,0, G_OPTION_ARG_STRING, &MILU_FUNC_NAME, "Set a function to mutate.", "function name"},
  { "tag-list", 0,0, G_OPTION_ARG_STRING, &MILU_TAG_LIST_PATH, "This is a temp function, set the path to a file, selecting a set of external declarations to reduce the tree size.", "path"},
  { "html", 'h',0, G_OPTION_ARG_NONE, &MILU_HTML_VIEW, "Generate a simple html mutant viewer.", NULL},
  { "mid", 'i',0, G_OPTION_ARG_NONE, &MILU_SAVE_MID, "Generate a Mid file for the current set of mutants.", NULL},
  { "html_all", 'h',0, G_OPTION_ARG_NONE, &MILU_HTML_SAVE_ALL_MUT, "Save all html mutants.", NULL},
  { "not-save-mut", 0,0, G_OPTION_ARG_NONE, &MILU_NOT_SAVE_MUT, "Do not save mutants", NULL},
  { "mid_path", 0,0, G_OPTION_ARG_STRING, &MILU_MID_PATH, "Path to Mid file.", NULL},
  /*
  }
  { "mode", 0, 0, G_OPTION_ARG_STRING, &mode, "Mode for each options", "M" },
  { "killingtime", 0, 0, G_OPTION_ARG_INT, &killingtime, "Killing time for loop mutants", "K" },
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Be verbose", NULL },
  { "silent", 's', 0, G_OPTION_ARG_NONE, &silent, "Be silent", NULL },
  { "noindent", 0, 0, G_OPTION_ARG_NONE, &noindent, "Not apply indent to pretty print source code", NULL },
  { "test-file", 'f', 0, G_OPTION_ARG_NONE, &test_file, "Try to parse individual file", NULL },
  { "test-project", 'p', 0, G_OPTION_ARG_NONE, &test_project, "Try to parse source folder", NULL },
  { "gccplugin-init", 'g', 0,  G_OPTION_ARG_NONE, &gccplugin, "Init Milu with GCC plugin", NULL },
  { "gccplugin-gm", 'e', 0,  G_OPTION_ARG_NONE, &gccplugin1, "Generate Mutants", NULL },
  { "generate_mut", 0, 0,  G_OPTION_ARG_NONE, &generate_mut, "Generate mutants", NULL },
  { "generate_selective_mut", 'm', 0,  G_OPTION_ARG_NONE, &generate_selective_mut, "Generate selective mutants", NULL },
  { "run_selective_test", 't', 0,  G_OPTION_ARG_NONE, &run_selective_test, "Run selectivemutants", NULL },
  { "run_test", 0, 0,  G_OPTION_ARG_NONE, &run_test, "Run mutants", NULL },
  { "2hom", 0, 0,  G_OPTION_ARG_NONE, &do2hom, "do 2hom", NULL },
  { "count", 0, 0,  G_OPTION_ARG_NONE, &count_mut, "count mutants", NULL },
  { "ins", 0, 0,  G_OPTION_ARG_NONE, &ins, "do instrumentation", NULL },
  { "filtersize", 0, 0,  G_OPTION_ARG_INT, &filter_size, "Filter size", NULL },
  */
  { NULL }
};

// Public function implementation

void milu_options_parse(int argc, char *argv[ ])
{
	GError *error = NULL;
	GOptionContext *context = g_option_context_new ("- Milu: Higher Order Mutation Testing System v3.0. "
			"Copyright (C) 2011 Yue Jia. "
			"This program comes with ABSOLUTELY NO WARRANTY.");
	g_option_context_add_main_entries (context, entries, NULL);
	if (!g_option_context_parse (context, &argc, &argv, &error))
	{
		g_log ("Milu",G_LOG_LEVEL_WARNING,"Show all help options: milu --help\n") ;
		exit(1);
	}


	if(MILU_MUTANT_STRATEGY == NULL)
	{
	   MILU_MUTANT_STRATEGY = g_strdup("default");
	}

	if(MILU_OUPUT_PATH== NULL)
	{
       gchar * curr_path =  g_get_current_dir ();
	   MILU_OUPUT_PATH=g_strdup_printf("%s/milu_output",curr_path);
       g_free(curr_path);
	}

	if(MILU_EXEC_STRATEGY == NULL)
	{
		MILU_EXEC_STRATEGY = g_strdup("test-1st");
	}

	if(MILU_TEST_DRIVER_PATH == NULL)
	{
		//MILU_TEST_DRIVER_PATH= g_strdup("milu_driver.c");
		//Yue August 7, 2014 ignore driver if not given
		MILU_TEST_DRIVER_PATH= NULL;//g_strdup("");
	}

	if(MP_NUM > 0)
		milu_set_multi_process(MP_NUM);

}

void milu_options_free()
{
	g_free(MILU_TEST_INPUT_PATH);
	g_free(MILU_TEST_DRIVER_PATH);
    g_free(MILU_OUPUT_PATH);
    g_free(MILU_DEBUG);
    g_free(MILU_MUTANT_STRATEGY);
    g_free(MILU_COMPILE_CMD);
    g_free(MILU_EXEC_STRATEGY);
    g_free(MILU_MUT_OPERATORS_PATH);
    g_free(MILU_SEARCH_STRATEGY);
    if (MILU_FUNC_NAME)
        g_free(MILU_FUNC_NAME);
}

gchar * milu_options_get_test_input_path()
{
    return MILU_TEST_INPUT_PATH;
}

gchar * milu_options_get_test_driver_path()
{
	return MILU_TEST_DRIVER_PATH;
}

void milu_options_set_test_driver_path(gchar * driver)
{
	MILU_TEST_DRIVER_PATH = g_strdup(driver);
}

gchar * milu_options_get_output_path()
{
	return MILU_OUPUT_PATH;
}

gchar * milu_options_get_debug_info()
{
    return MILU_DEBUG;
}

gchar * milu_options_get_search_strategy()
{
    return MILU_SEARCH_STRATEGY;
}
gchar * milu_options_get_mut_strategy()
{
    return MILU_MUTANT_STRATEGY;
}

gint milu_options_get_exec_strategy()
{
    if (g_strcmp0(MILU_EXEC_STRATEGY , "mut-1st" ) ==0 )
    {
        return 1;
    }
    else if (g_strcmp0(MILU_EXEC_STRATEGY , "mut-1st-full") ==0 )
    {
        return 3;
    }
    else if (g_strcmp0(MILU_EXEC_STRATEGY , "test-1st-full") ==0 )
    {
        return 4;
    }
    else if (g_strcmp0(MILU_EXEC_STRATEGY , "test-1st") ==0 )
    {
        return 2;
    }
    else
    {
        return 1;
    }

}

gint  milu_options_get_mut_order()
{
	return MILU_MUTANT_ORDER;
}

gint  milu_options_get_rand_limit_number()
{
    return MILU_MUTANT_RAND_LIMIT;
}

gchar * milu_options_get_func_list_path()
{
    return MILU_FUNC_LIST_PATH;
}

gchar * milu_options_get_func_name()
{
    return MILU_FUNC_NAME;
}

gchar * milu_options_get_tag_list_path()
{
    return MILU_TAG_LIST_PATH;
}

gchar * milu_options_get_mut_operators_path()
{
    return MILU_MUT_OPERATORS_PATH;
}

gint milu_options_get_order_start()
{
    if(MILU_MUTANT_ORDER_BEGIN < 0 || MILU_MUTANT_ORDER_END < 0)
        return MILU_MUTANT_ORDER;

    return MILU_MUTANT_ORDER_BEGIN;
}

gint milu_options_get_order_end()
{
    if(MILU_MUTANT_ORDER_BEGIN < 0 || MILU_MUTANT_ORDER_END < 0)
        return MILU_MUTANT_ORDER;

    return MILU_MUTANT_ORDER_END;
}

gchar * milu_options_get_compilation_cmd()
{
    return MILU_COMPILE_CMD;
}

gboolean milu_options_get_html_view()
{
	return MILU_HTML_VIEW;
}

gboolean milu_options_get_unit_test()
{
	return MILU_UNIT_TEST;
}


gboolean milu_options_get_save_mid()
{
	return MILU_SAVE_MID;
}

gchar * milu_options_get_mid_file_path()
{
	return MILU_MID_PATH;
}

gboolean milu_options_get_html_save_all()
{
	return MILU_HTML_SAVE_ALL_MUT;
}

void milu_options_set_html_save_all()
{
	MILU_HTML_SAVE_ALL_MUT = TRUE;
}

gboolean milu_options_print_killing_result()
{
	return MILU_KILLING_RESULT;
}

gboolean milu_options_not_save_mutants()
{
	return  MILU_NOT_SAVE_MUT;
}

gboolean milu_options_save_killing_result()
{
	return MILU_SAVE_KILLING_RESULT ;
}

void milu_options_set_src_path(gchar * path)
{
	MILU_SRC_PATH = g_string_chunk_insert(MiluStringPool, path);
}

gchar * milu_options_get_src_path()
{
	return MILU_SRC_PATH;
}
