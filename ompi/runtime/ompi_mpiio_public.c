/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */

#include "ompi_config.h"

#include "ompi/runtime/ompi_mpiio_public.h"

bool ompi_mpiio_internal_use_mpi = false;
static const ompi_mpiio_public_api_t *public_api;

void
ompi_mpiio_register_public_api(const ompi_mpiio_public_api_t *api)
{
    public_api = api;
}

const ompi_mpiio_public_api_t *
ompi_mpiio_public_api(void)
{
    return public_api;
}
