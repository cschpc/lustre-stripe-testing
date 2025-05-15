#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>

// for temp file name
#include <unistd.h>
#include <string.h>

#include <mpi.h>

/* Creates a file with MPI-IO and writes 'bytesPerRank' bytes from each rank, repeating the write 'n_repeat' times.
Return value is the total write time. Collective write is used (`MPI_File_write_at_all`)
*/
double RunIOTest(const std::string& fileName, size_t bytesPerRank, size_t n_repeat)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<char> buffer(bytesPerRank, 'A' + rank % 26);

    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, fileName.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    const MPI_Offset offset = static_cast<MPI_Offset>(rank * bytesPerRank);

    // warmup
    MPI_File_write_at_all(file, offset, buffer.data(), buffer.size(), MPI_CHAR, MPI_STATUS_IGNORE);

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < n_repeat; i++)
    {
        MPI_File_write_at_all(file, offset, buffer.data(), buffer.size(), MPI_CHAR, MPI_STATUS_IGNORE);
    }

    auto end = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double>(end - start).count();

    MPI_File_close(&file);
    MPI_File_delete(fileName.c_str(), MPI_INFO_NULL);

    return elapsed;
}


int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <fileSize1_MB> <fileSize2_MB> ...\n";
        MPI_Finalize();
        return 1;
    }

    int mpiSize;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<size_t> fileSizesMB;
    // Process input arguments: file sizes in MB
    for (int i = 1; i < argc; i++)

    {   const size_t megaBytes = static_cast<size_t>(std::stoul(argv[i]));
        if (megaBytes % mpiSize != 0 && rank == 0)
        {
            std::cerr << "fileSize_MB must be divisible with number of MPI ranks\n";
            std::cerr << "Faulty fileSize was: " << megaBytes << "\n";
            MPI_Finalize();
            return 1;
        }

        fileSizesMB.push_back(megaBytes);
    }

    // how many times to repeat the IO time measurement
    const size_t n_repeat = 1;

    if (rank == 0)
    {
        printf("n_repeat = %zu, mpiSize = %zu\n", n_repeat, mpiSize);
        printf("fileSizeMB AvgTime AvgBandwidth\n");
    }

    for (size_t megaBytes : fileSizesMB)
    {

        const size_t bytes = megaBytes * 1024 * 1024;
        const size_t bytesPerRank = bytes / mpiSize;

        // Ensure we create and write to a unique file each time.
        // For this, produce a unique temp file name, POSIX style

        std::string fname = "tempfileXXXXXX";
        char* fname_cstyle = strdup(fname.c_str());

        if (rank == 0)
        {
            // Generate unique filename
            int fd = mkstemp(fname_cstyle);

            if (fd == -1)
            {
                std::cerr << "temp file creation failed, aborting\n";
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }

        MPI_Bcast(fname_cstyle, fname.length(), MPI_CHAR, 0, MPI_COMM_WORLD);

        fname = std::string(fname_cstyle);

        const double totalTime = RunIOTest(fname, bytesPerRank, n_repeat);
        const double avgTime = totalTime / n_repeat;
        const double avgBandwidth = (n_repeat * megaBytes) / totalTime;

        if (rank == 0)
        {
            // Write time measurement
            printf("%zu %g %g\n",
                megaBytes, avgTime, avgBandwidth
            );
            fflush(stdout);
        }
    }

    MPI_Finalize();
    return 0;
}
