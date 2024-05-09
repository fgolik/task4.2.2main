#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Student
{
private:
    unsigned int kurs;
    unsigned int age;
    string fullName;
    string gender;
    unsigned int grade;

public:
    // Constructors
    Student() : kurs(0), age(0), fullName(""), gender(""), grade(0) {}
    Student(unsigned int k, unsigned int a, const std::string& name, const std::string& gen, unsigned int g)
        : kurs(k), age(a), fullName(name), gender(gen), grade(g) {}
    Student(const Student& other) : kurs(other.kurs), age(other.age), fullName(other.fullName), gender(other.gender), grade(other.grade) {}
    Student(Student&& other) noexcept : kurs(other.kurs), age(other.age), fullName(std::move(other.fullName)), gender(std::move(other.gender)), grade(other.grade) {}

    // Destructor
    ~Student() {}

    // Methods
    void fillFromKeyboard();
    void loadFromFile( ifstream& in);
    void saveToFile(ofstream& out);
    void display();
    unsigned int getGrade() const { return grade; }
    unsigned int getKurs() const { return kurs; }
};

void Student::fillFromKeyboard() 
{
    cout << "Enter full name: ";
    cin.ignore();
    getline(std::cin, fullName);
    cout << "Enter gender: ";
    cin >> gender;
    cout << "Enter kurs: ";
    cin >> kurs;
    cout << "Enter grade: ";
    cin >> grade;
    cout << "Enter age: ";
    cin >> age;
}

void Student::loadFromFile(ifstream& in)
{
    in.read(reinterpret_cast<char*>(this), sizeof(Student));
}

void Student::saveToFile(ofstream& out) 
{
    out.write(reinterpret_cast<char*>(this), sizeof(Student));
}

void Student::display()
{
    cout << "Full Name: " << fullName << endl;
    cout << "Gender: " << gender << endl;
    cout << "Kurs: " << kurs << endl;
    cout << "Grade: " << grade << endl;
    cout << "Age: " << age << endl;
}

int main() 
{
    const string path = "students.bin";
    ofstream out(path, ios::out | ios::binary);

    if (!out.is_open()) 
    {
        cout << "File is not open" << endl;
        return 1;
    }

    vector<Student> students;
    int numStudents;
    cout << "Enter the number of students: ";
    cin >> numStudents;

    for (int i = 0; i < numStudents; ++i) 
    {
        Student student;
        student.fillFromKeyboard();
        students.push_back(student);
        student.saveToFile(out);
    }

    out.close();

    int targetCourse;
    cout << "Enter the course to calculate the average grade: ";
    cin >> targetCourse;

    unsigned int totalGrade = 0;
    unsigned int numStudentsInCourse = 0;

    ifstream in(path, ios::in | ios::binary);
    if (!in.is_open())
    {
        cout << "File is not open" << endl;
        return 1;
    }

    for (const auto& student : students) 
    {
        if (student.getKurs() == targetCourse) 
        {
            totalGrade += student.getGrade();
            numStudentsInCourse++;
        }
    }

    if (numStudentsInCourse > 0)
    {
        double averageGrade = static_cast<double>(totalGrade) / numStudentsInCourse;
        cout << "Average grade for course " << targetCourse << " is: " << averageGrade << endl;
    }
    else 
    {
        cout << "No students found in course " << targetCourse << endl;
    }

    in.close();

    return 0;
}