/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */

#include "ompi_config.h"

#include <limits.h>
#include <stdlib.h>

#include "ompi/mca/common/ompio/common_ompio_mpi.h"
#include "ompi/request/request.h"
#include "ompi/runtime/ompi_mpiio_public.h"

static const ompi_mpiio_public_api_t *
public_api(const ompio_file_t *fh)
{
    return fh->f_use_mpi_symbols ? ompi_mpiio_public_api() : NULL;
}

int
mca_common_ompio_comm_dup(ompio_file_t *fh, ompi_communicator_t *comm)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->comm_dup((MPI_Comm) comm, (MPI_Comm *) &fh->f_comm)
                       : ompi_comm_dup(comm, &fh->f_comm);
}

int
mca_common_ompio_comm_free(ompio_file_t *fh)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->comm_free((MPI_Comm *) &fh->f_comm)
                       : ompi_comm_free(&fh->f_comm);
}

int
mca_common_ompio_barrier(ompio_file_t *fh)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->barrier((MPI_Comm) fh->f_comm)
                       : fh->f_comm->c_coll->coll_barrier(
                             fh->f_comm, fh->f_comm->c_coll->coll_barrier_module);
}

int
mca_common_ompio_bcast(ompio_file_t *fh, void *buffer, int count,
                       ompi_datatype_t *datatype, int root)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->bcast(buffer, count, (MPI_Datatype) datatype, root,
                                    (MPI_Comm) fh->f_comm)
                       : fh->f_comm->c_coll->coll_bcast(
                             buffer, count, datatype, root, fh->f_comm,
                             fh->f_comm->c_coll->coll_bcast_module);
}

int
mca_common_ompio_allreduce(ompio_file_t *fh, const void *sendbuf, void *recvbuf,
                           int count, ompi_datatype_t *datatype, struct ompi_op_t *op)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->allreduce(sendbuf, recvbuf, count, (MPI_Datatype) datatype,
                                        (MPI_Op) op, (MPI_Comm) fh->f_comm)
                       : fh->f_comm->c_coll->coll_allreduce(
                             sendbuf, recvbuf, count, datatype, op, fh->f_comm,
                             fh->f_comm->c_coll->coll_allreduce_module);
}

int
mca_common_ompio_allgather(ompio_file_t *fh, const void *sendbuf, int sendcount,
                           ompi_datatype_t *sendtype, void *recvbuf, int recvcount,
                           ompi_datatype_t *recvtype)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->allgather(sendbuf, sendcount, (MPI_Datatype) sendtype,
                                        recvbuf, recvcount, (MPI_Datatype) recvtype,
                                        (MPI_Comm) fh->f_comm)
                       : fh->f_comm->c_coll->coll_allgather(
                             sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype,
                             fh->f_comm, fh->f_comm->c_coll->coll_allgather_module);
}

int
mca_common_ompio_allgatherv(ompio_file_t *fh, const void *sendbuf, int sendcount,
                            ompi_datatype_t *sendtype, void *recvbuf,
                            const size_t *recvcounts, const ptrdiff_t *displs,
                            ompi_datatype_t *recvtype)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    ompi_count_array_t count_desc;
    ompi_disp_array_t disp_desc;
    int *arrays = NULL;
    int ret;

    if (NULL != api) {
        arrays = malloc((size_t) 2 * fh->f_size * sizeof(*arrays));
        if (NULL == arrays) {
            return OMPI_ERR_OUT_OF_RESOURCE;
        }
        for (int i = 0; i < fh->f_size; ++i) {
            if (recvcounts[i] > INT_MAX || displs[i] < 0 || displs[i] > INT_MAX) {
                free(arrays);
                api = NULL;
                break;
            }
            arrays[i] = (int) recvcounts[i];
            arrays[fh->f_size + i] = (int) displs[i];
        }
        if (NULL != api) {
            ret = api->allgatherv(sendbuf, sendcount, (MPI_Datatype) sendtype, recvbuf,
                                  arrays, arrays + fh->f_size, (MPI_Datatype) recvtype,
                                  (MPI_Comm) fh->f_comm);
            free(arrays);
            return ret;
        }
    }

    OMPI_COUNT_ARRAY_INIT(&count_desc, recvcounts);
    OMPI_DISP_ARRAY_INIT(&disp_desc, displs);
    return fh->f_comm->c_coll->coll_allgatherv(
        sendbuf, sendcount, sendtype, recvbuf, count_desc, disp_desc, recvtype,
        fh->f_comm, fh->f_comm->c_coll->coll_allgatherv_module);
}

int
mca_common_ompio_isend(ompio_file_t *fh, const void *buf, int count,
                       ompi_datatype_t *datatype, int dest, int tag,
                       mca_pml_base_send_mode_t mode, ompi_request_t **request)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->isend(buf, count, (MPI_Datatype) datatype, dest, tag,
                                    (MPI_Comm) fh->f_comm, (MPI_Request *) request)
                       : MCA_PML_CALL(isend(buf, count, datatype, dest, tag, mode,
                                           fh->f_comm, request));
}

int
mca_common_ompio_irecv(ompio_file_t *fh, void *buf, int count,
                       ompi_datatype_t *datatype, int source, int tag,
                       ompi_request_t **request)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->irecv(buf, count, (MPI_Datatype) datatype, source, tag,
                                    (MPI_Comm) fh->f_comm, (MPI_Request *) request)
                       : MCA_PML_CALL(irecv(buf, count, datatype, source, tag,
                                           fh->f_comm, request));
}

int
mca_common_ompio_waitall(ompio_file_t *fh, int count, ompi_request_t **requests,
                         ompi_status_public_t *statuses)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    return NULL != api ? api->waitall(count, (MPI_Request *) requests,
                                      (MPI_Status *) statuses)
                       : ompi_request_wait_all(count, requests, statuses);
}

int
mca_common_ompio_create_hindexed(ompio_file_t *fh, int count,
                                 const int *blocklengths, const MPI_Aint *displs,
                                 ompi_datatype_t *oldtype, ompi_datatype_t **newtype)
{
    const ompi_mpiio_public_api_t *api = public_api(fh);
    int ret;

    if (NULL != api) {
        ret = api->type_create_hindexed(count, blocklengths, displs,
                                        (MPI_Datatype) oldtype,
                                        (MPI_Datatype *) newtype);
        return OMPI_SUCCESS == ret ? api->type_commit((MPI_Datatype *) newtype) : ret;
    }

    ret = ompi_datatype_create_hindexed(count, OMPI_COUNT_ARRAY_CREATE(blocklengths),
                                        OMPI_DISP_ARRAY_CREATE(displs), oldtype, newtype);
    return OMPI_SUCCESS == ret ? ompi_datatype_commit(newtype) : ret;
}
