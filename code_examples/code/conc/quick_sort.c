#include "csapp.h"

void qsort_serial(data_t *base, size_t nele)
{
    if (nele <= 1)
        return;
    if (nele == 2)
    {
        if (base[0] > base[1])
            swap(base, base + 1);
        return;
    }

    /* Partition returns index of pivot */
    size_t m = partition(base, nele);
    if (m > 1)
        qsort_serial(base, m);
    if (nele - 1 > m + 1)
        qsort_serial(base + m + 1, nele - m - 1);
}
