#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;


class Figure {
    public:
    virtual string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle : public Figure {
    public:
    Triangle(const double& a, const double& b, const double& c) : a_(a), b_(b), c_(c){}
    virtual string Name() const override {
        return "TRIANGLE";
    }
    virtual double Perimeter() const override {
        return a_ + b_ + c_;
    }
    virtual double Area() const override {
        double p = (a_ + b_ + c_) / 2;
        return sqrt(p * (p - a_) * (p - b_) * (p - c_));
    }
    private:
    const double a_;
    const double b_;
    const double c_;
};

class Rect : public Figure {
    public:
    Rect(const double& a, const double& b) : a_(a), b_(b){}
    virtual string Name() const override {
        return "RECT";
    }
    virtual double Perimeter() const override {
        return (a_ + b_) * 2;
    }
    virtual double Area() const override {
        return a_ * b_;
    }
    private:
    const double a_;
    const double b_;
};

class Circle : public Figure{
public:
Circle(const double& r) : r_(r){}
virtual string Name() const override{
    return "CIRCLE";
}
virtual double Perimeter() const override{
    return 2 * 3.14 * r_;
}
virtual double Area() const override {
    return 3.14 * r_ * r_;
}
private:
const double r_;
};

shared_ptr<Figure> CreateFigure(istringstream& i) {
    string input;
    i >> input;
    shared_ptr<Figure> result;
    if (input == "TRIANGLE") {
        double a, b, c;
        i >> a;
        i >> b;
        i >> c;
        result = make_shared<Triangle>(a, b, c);
    }
    else if (input == "RECT") {
        double a;
        double b;
        i >> a;
        i >> b;
        result = make_shared<Rect>(a, b);
    }
    else if (input == "CIRCLE") {
        double r;
        i >> r;
        result = make_shared<Circle>(r);
    }
    return result;
}


int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            // Пропускаем "лишние" ведущие пробелы.
            // Подробнее об std::ws можно узнать здесь:
            // https://en.cppreference.com/w/cpp/io/manip/ws
            is >> ws;
            figures.push_back(CreateFigure(is));
        }
        else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                    << current_figure->Name() << " "
                    << current_figure->Perimeter() << " "
                    << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}
