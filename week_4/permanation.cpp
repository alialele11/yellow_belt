#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int N;
    cin >> N;
    int v[N];
    for (int i = 0; i < N; ++i){
        v[i] = i + 1;
    }
    sort(v, v + N);
    reverse(v, v + N);
    do {
        for (int i = 0; i < N; ++i) {
            cout << v[i] << ' ';
        }
        cout << endl;
    } while (prev_permutation(v, v + N));
       return 0;
}
