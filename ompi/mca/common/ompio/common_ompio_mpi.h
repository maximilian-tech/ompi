/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */

#ifndef MCA_COMMON_OMPIO_MPI_H
#define MCA_COMMON_OMPIO_MPI_H

#include "ompi_config.h"

#include "ompi/mca/common/ompio/common_ompio.h"
#include "ompi/mca/pml/pml.h"

BEGIN_C_DECLS

int mca_common_ompio_comm_dup(ompio_file_t *fh, ompi_communicator_t *comm);
int mca_common_ompio_comm_free(ompio_file_t *fh);
int mca_common_ompio_barrier(ompio_file_t *fh);
int mca_common_ompio_bcast(ompio_file_t *fh, void *buffer, int count,
                           ompi_datatype_t *datatype, int root);
int mca_common_ompio_allreduce(ompio_file_t *fh, const void *sendbuf, void *recvbuf,
                               int count, ompi_datatype_t *datatype, struct ompi_op_t *op);
int mca_common_ompio_allgather(ompio_file_t *fh, const void *sendbuf, int sendcount,
                               ompi_datatype_t *sendtype, void *recvbuf, int recvcount,
                               ompi_datatype_t *recvtype);
int mca_common_ompio_allgatherv(ompio_file_t *fh, const void *sendbuf, int sendcount,
                                ompi_datatype_t *sendtype, void *recvbuf,
                                const size_t *recvcounts, const ptrdiff_t *displs,
                                ompi_datatype_t *recvtype);
int mca_common_ompio_isend(ompio_file_t *fh, const void *buf, int count,
                           ompi_datatype_t *datatype, int dest, int tag,
                           mca_pml_base_send_mode_t mode, ompi_request_t **request);
int mca_common_ompio_irecv(ompio_file_t *fh, void *buf, int count,
                           ompi_datatype_t *datatype, int source, int tag,
                           ompi_request_t **request);
int mca_common_ompio_waitall(ompio_file_t *fh, int count, ompi_request_t **requests,
                             ompi_status_public_t *statuses);
int mca_common_ompio_create_hindexed(ompio_file_t *fh, int count,
                                     const int *blocklengths, const MPI_Aint *displs,
                                     ompi_datatype_t *oldtype,
                                     ompi_datatype_t **newtype);

END_C_DECLS

#endif /* MCA_COMMON_OMPIO_MPI_H */
