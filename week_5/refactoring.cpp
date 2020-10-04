#include <iostream>
#include <string>
#include <vector>

using namespace std;

class People {
public:
	People(const string& name, const string& t) : Name(name), type(t) {}

	virtual void Walk(const string& destination) const {
		cout << type << ": " << Name << " walks to: " << destination << endl;
	}
	const string Name;
	const string type;
	const string full_name = type + ": " + Name;
};

class Student : public People {
public:

	Student(const string& name, const string& favouriteSong) : People(name, "Student"), FavouriteSong(favouriteSong) {}
	void Learn() {
		cout << full_name << " learns" << endl;
	}
	void SingSong() {
		cout << full_name << " sings a song: " << FavouriteSong << endl;
	}
	virtual void Walk(const string& destination)const override {
		cout << full_name << " walks to: " << destination << endl;
		cout << full_name << " sings a song: " << FavouriteSong << endl;
	}

private:
	const string FavouriteSong;
};


class Teacher : public People {
public:
	Teacher(const string& name, const string& subject) : People(name, "Teacher"), Subject(subject) {}
	void Teach() {
		cout << full_name << " teaches: " << Subject << endl;
	}
private:
	const string Subject;
};


class Policeman : People {
public:
	Policeman(const string& name) : People(name, "Policeman") {}
	void Check(const People& h) {
		cout << full_name << " checks " << h.type << ". " << h.type << "'s name is: " << h.Name << endl;
	}
};

void VisitPlaces(const People& t, const vector<string>& places) {
	for (auto p : places) {
		t.Walk(p);
	}
}

int main() {
	Teacher t("Jim", "Math");
	Student s("Ann", "We will rock you");
	Policeman p("Bob");

	VisitPlaces(t, { "Moscow", "London" });
	p.Check(s);
	VisitPlaces(s, { "Moscow", "London" });
	return 0;
}