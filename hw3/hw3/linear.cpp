bool anyFalse(const double a[], int n) {
    if(n <= 0)
        return false;
    if(!somePredicate(a[0]))
        return true;
    return anyFalse(a + 1, n - 1);
}

int countFalse(const double a[], int n) {
    if(n <= 0)
        return 0;
    if(!somePredicate(a[0]))
        return 1 + countFalse(a + 1, n - 1);
    return countFalse (a + 1, n - 1);
}

int firstFalse(const double a[], int n) {
    if(n <= 0)
        return -1;
    if(!somePredicate(a[0]))
        return 0;
    int pos = firstFalse(a + 1, n - 1);
    if(pos != -1)
        return pos + 1; // adding 1 corresponds to the index in the OG array
    return -1;
}

int locateMinimum(const double a[], int n) {
    if(n <= 0)
        return -1;
    if(n == 1)
        return 0;
    int min = locateMinimum(a + 1, n - 1);
    if(a[0] < a[min + 1])
        return 0;
    return min + 1;
}

bool includes(const double a1[], int n1, const double a2[], int n2) {
    if(n2 <= 0)
        return true;
    if(n1 <= 0)
        return false;
    if(a1[0] == a2[0])
        return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    return includes(a1 + 1, n1 - 1, a2, n2);
}
