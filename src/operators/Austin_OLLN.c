/*
 * austin_orrn.c
 *
 *  Created on: Sep 2, 2012
 *      Author: yuejia
 */


#include "MutationOperator.h"


Operator * get_Austin_OLLN_operator()
{
	const gchar * name = "Austin_OLLN";
	const gchar * info = "Austin Do relational operator mutation";
	Operator * op = mutation_operator_new(name, info);

	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_and_logical_replacement());
	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_or_logical_replacement());

	return op;
}
