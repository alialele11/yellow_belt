#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        }
        catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        }
        catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};
void TestWorks() {
    {
        PhoneNumber number("+7-495-111-22-33");
        AssertEqual(number.GetCountryCode(), "7", "7");
        AssertEqual(number.GetCityCode(), "495", "495");
        AssertEqual(number.GetLocalNumber(), "111-22-33", "111-22-33");
        AssertEqual(number.GetInternationalNumber(), "+7-495-111-22-33", "GetInternationalNumber");
    }

    {
        PhoneNumber number("+323-22-460002");
        AssertEqual(number.GetCountryCode(), "323", "323");
        AssertEqual(number.GetCityCode(), "22", "22");
        AssertEqual(number.GetLocalNumber(), "460002", "460002");
        AssertEqual(number.GetInternationalNumber(), "+323-22-460002", "GetInternationalNumber");
    }

    {
        PhoneNumber number("+1-2-coursera-cpp");
        AssertEqual(number.GetCountryCode(), "1", "1");
        AssertEqual(number.GetCityCode(), "2", "2");
        AssertEqual(number.GetLocalNumber(), "coursera-cpp", "coursera-cpp");
        AssertEqual(number.GetInternationalNumber(), "+1-2-coursera-cpp", "GetInternationalNumber");
    }
}
