#include <bits/stdc++.h>
#include "read.h"
#include "point.h"
#include "polygon.h"
#include "sort.h"

using namespace std;

int main()
{
    vector<polygon> v = read();
    sort(v);

    for (auto x : v)
        x.print();
    return 0;
}
