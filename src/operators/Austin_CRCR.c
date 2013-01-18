/*
 * austin_orrn.c
 *
 *  Created on: Sep 2, 2012
 *      Author: yuejia
 */


#include "MutationOperator.h"


Operator * get_Austin_CRCR_operator()
{
	const gchar * name = "Austin_CRCR";
	const gchar * info = "Austin Do integer constant operator mutation";
	Operator * op = mutation_operator_new(name, info);

	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_integer_constant_replacement());

//Todo:	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_float_constant_replacement());
	return op;
}
