/*
 * austin_orrn.c
 *
 *  Created on: Sep 2, 2012
 *      Author: yuejia
 */


#include "MutationOperator.h"


Operator * get_Austin_ORRN_operator()
{
	const gchar * name = "Austin_ORRN";
	const gchar * info = "Do relational operator mutation";
	Operator * op = mutation_operator_new(name, info);

	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_greater_relational_replacement());
	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_less_relational_replacement());
	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_less_equal_relational_replacement());
	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_greater_equal_relational_replacement());
	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_equal_relational_replacement());
	g_ptr_array_add(op->mutators, (gpointer) mutator_austin_milu_not_equal_relational_replacement());

	return op;
}
