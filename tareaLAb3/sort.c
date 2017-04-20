#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <utility>
#include <ctime>
#include <algorithm>
#include <vector>

using namespace std;

int ComputePartner(int phase, int rank, int size) {
    int partner = ((phase ^ rank) & 1) ? rank + 1 : rank - 1;
    if (partner == -1 or partner == size){
        partner = MPI_PROC_NULL;
    }
    return partner;
}

vector<int> MergeSmall(const vector<int> &a, const vector<int> &b) {
    vector<int> c(a.size());
    size_t i = 0, j = 0;
    while (i < a.size() and j < b.size() and i + j < c.size()) {
        if (a[i] < b[j])
          c[i+j] = a[i++];
        else
          c[i+j] = b[j++];
    }
    while (i < a.size() and i + j < c.size())
        c[i+j] = a[i++];
    while (j < b.size() and i + j < c.size())
        c[i+j] = b[j++];
    return c;
}

vector<int> MergeLarge(const vector<int> &a, const vector<int> &b) {
    vector<int> c(a.size());
    size_t i = 0, j = 0;
    while (i < a.size() and j < b.size() and i + j < c.size()) {
        if (a[a.size()-i-1] > b[b.size()-j-1])
            c[c.size()-(i+j+1)] = a[a.size()-(i+++1)];
        else
            c[c.size()-(i+j+1)] = b[b.size()-(j+++1)];
    }
    while (i < a.size() and i + j < c.size())
        c[c.size()-(i+j+1)] = a[a.size()-(i+++1)];
    while (j < b.size() and i + j < c.size())
        c[c.size()-(i+j+1)] = b[b.size()-(j+++1)];
    return c;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int n;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    vector<int> all_data;
    
    if (rank == 0) {
        cin >> n;   
        all_data.resize(n);
        for (size_t i = 0; i < n; ++i)
            cin >> all_data[i];
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    vector<int> part(n/size);
    vector<int> other(n/size);
    MPI_Scatter(all_data.data(), n/size, MPI_INT,
        part.data(), n/size, MPI_INT, 0, MPI_COMM_WORLD);
    
    sort(part.begin(), part.end());
    
    for (int phase = 0; phase < size; ++phase) {
        int partner = ComputePartner(phase, rank, size);
        MPI_Sendrecv(part.data(), n/size, MPI_INT, partner, 0, other.data(), n/size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (rank < partner)
            part = MergeSmall(part, other);
        else
            part = MergeLarge(part, other);
    }

    cout << "Proc " << rank << " finished " << endl;

    MPI_Gather(part.data(), n/size, MPI_INT, all_data.data(), n/size, MPI_INT, 0, MPI_COMM_WORLD);

    
    if (rank == 0) {
        for (int i = 0; i < n; ++i)
            printf("%d ", all_data[i]);
        puts("");
    }

    MPI_Finalize();
    return 0;
}