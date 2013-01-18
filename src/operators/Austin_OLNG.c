/*
 * austin_orrn.c
 *
 *  Created on: Sep 2, 2012
 *      Author: yuejia
 */


#include "MutationOperator.h"


Operator * get_Austin_OLNG_operator()
{
	const gchar * name = "Austin_OLNG";
	const gchar * info = "Austin Do logical negation operator mutation";
	Operator * op = mutation_operator_new(name, info);

	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_and_logical_negation());
	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_or_logical_negation());

	return op;
}
