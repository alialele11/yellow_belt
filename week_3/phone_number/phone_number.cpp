#include <iostream>
#include <algorithm>
#include "phone_number.h"
#include "unit_tests_template.h"

using namespace std;

PhoneNumber::PhoneNumber(const string& international_number) {
    //Первичная проверка на "исключаемые" ситуации
    if (international_number == "") {
        throw invalid_argument("");
    }
    if (international_number[0] != '+') {
        throw invalid_argument("");
    }
    size_t i = 1;
    //Считываем код страны
    while (international_number[i] != '-' && i < international_number.size()) {
        country_code_ += international_number[i];
        ++i;
    }
    i++;
    //Считываем код города до '-' или конца строки
    while (international_number[i] != '-' && i < international_number.size()) {
        city_code_ += international_number[i];
        ++i;
    }
    i++;
    //Считываем локальный номер до конца строки
    while (i < international_number.size()) {
        local_number_ += international_number[i];
        ++i;
    }
    //Проверяем на "исключаемые" ситуации
    if (country_code_ == "" || city_code_ == "" || local_number_ == "") {
        throw invalid_argument("");
    }
}
string  PhoneNumber::GetCountryCode() const {
//Получаем код страны
    return country_code_;
}
string PhoneNumber::GetCityCode() const {
//Получаем городской номер
    return city_code_;
}
string PhoneNumber::GetLocalNumber() const {
//Получаем локальный номер
    return local_number_;
}
string PhoneNumber::GetInternationalNumber() const {
//Приводим номер к единому виду
    return "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
}

int main() {
//Tecтим на некоторых данных
    TestRunner tr;
    tr.RunTest(TestWorks, "testworks");
    return 0;
}
