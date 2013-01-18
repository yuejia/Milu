/*
 * austin_orrn.c
 *
 *  Created on: Sep 2, 2012
 *      Author: yuejia
 */


#include "MutationOperator.h"


Operator * get_Austin_OAAN_operator()
{
	const gchar * name = "Austin_OAAN";
	const gchar * info = "Do arithmetic operator mutation";
	Operator * op = mutation_operator_new(name, info);

	g_ptr_array_add(op->mutators, (gpointer)mutator_austin_milu_add_arithmetic_replacement());
	g_ptr_array_add(op->mutators, (gpointer)mutator_austin_milu_subtract_arithmetic_replacement());
	g_ptr_array_add(op->mutators, (gpointer)mutator_austin_milu_multiply_arithmetic_replacement());
	g_ptr_array_add(op->mutators, (gpointer)mutator_austin_milu_divide_arithmetic_replacement());
	g_ptr_array_add(op->mutators, (gpointer)mutator_austin_milu_modulus_arithmetic_replacement());
	return op;
}
