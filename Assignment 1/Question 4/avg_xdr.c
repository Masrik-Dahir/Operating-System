/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "avg.h"

bool_t
xdr_input_data (XDR *xdrs, input_data *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->input_data.input_data_val, (u_int *) &objp->input_data.input_data_len, 100,
		sizeof (double), (xdrproc_t) xdr_double))
		 return FALSE;
	return TRUE;
}
