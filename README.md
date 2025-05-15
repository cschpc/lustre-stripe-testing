# lustre-stripe-testing

Testing collective (MPI-IO) file write performance with different Lustre stripe counts. Made for the HPC summer school.

Directory `data_mpisize256` contains output from several runs on Mahti, using 256 MPI processes. Processed data and plots are also included.

Code does not automatically configure the striping. Use Lustre commands to set it for your output directory: `lfs setstripe -c`.
