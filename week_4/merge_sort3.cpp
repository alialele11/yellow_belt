#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 == range_end) {
        return;
    }
    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    auto one_three = elements.begin() + elements.size() / 3;
    auto two_three = elements.begin() + (elements.size() / 3) * 2;
    MergeSort(elements.begin(), one_three);
    MergeSort(one_three, two_three);
    vector<typename RandomIt::value_type> e;
    merge(elements.begin(),one_three, one_three, two_three, back_inserter(e));
    MergeSort(two_three, elements.end());
    merge(e.begin(), e.end(), two_three, elements.end(), range_begin);
}

int main() {
    vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1, 5 };
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}