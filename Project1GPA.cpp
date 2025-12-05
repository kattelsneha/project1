// ============================================================================
// GPA & Grade Calculator
// Author: Sneha Kattel
// Course: COSC 2436-H01
// Description:
// 1
//   This program lets the user keep track of college courses and assignments.
//   It can:
//     * Add courses with names and credit hours
//     * Add assignments to courses with earned and maximum points
//     * Compute each course's percentage and letter grade
//     * Compute overall GPA on a 4.0 scale using course grades and credit hours
//     * Run a "what-if" scenario for a hypothetical assignment
//     * Edit course information and assignment scores
//     * Delete courses and assignments
//     * Show a grade distribution report (how many A/B/C/D/F)
//
//   The program uses a simple text menu in the console so the user can
//   choose what they want to do.
// ============================================================================

#include <iostream>   // for input and output (std::cout, std::cin)
#include <string>     // for std::string
#include <vector>     // for std::vector to store many courses/assignments
#include <limits>     // for std::numeric_limits (used when clearing input)
#include <iomanip>    // for std::setprecision and std::fixed when printing

using namespace std;

// ============================================================================
// DATA STRUCTURES
// ============================================================================
//2
// Each assignment belongs to a single course.
// For example: "Homework 1", 85 points earned out of 100 possible.
struct Assignment {
    string name;      // a short name for the assignment (e.g., "Exam 1")
    double earned;    // how many points the student scored on this assignment
    double max;       // the maximum possible points for this assignment
};

// Each course can have many assignments.
// For example: "COSC 3345" with 3 credit hours and several assignments.
struct Course {
    int id;                     // a unique id so we can select this course
    string name;                // name of the course (e.g., "COSC 3345")
    double creditHours;         // credit hours (e.g., 3.0 or 4.0)
    vector<Assignment> work;    // list of assignments in this course
};

// ============================================================================
// HELPER FUNCTION DECLARATIONS (PROTOTYPES)
// ============================================================================

// General input helpers
void clearInputLine();
void pauseForUser();
int readIntInRange(const string& prompt, int minValue, int maxValue);
double readDoubleInRange(const string& prompt, double minValue, double maxValue);

// Menus
void showMainMenu();
void manageCoursesAndAssignments(vector<Course>& courses);

// Course operations
void addCourse(vector<Course>& courses, int& nextCourseId);
int findCourseIndexById(const vector<Course>& courses, int id);
void listCoursesSummary(const vector<Course>& courses);

// Assignment operations
void addAssignmentToCourse(vector<Course>& courses);

// Grade calculations and displays
double calculateCoursePercentage(const Course& course);
string percentageToLetter(double percent);
double letterToGradePoints(const string& letter);
void showCourseDetails(const vector<Course>& courses);
double calculateOverallGPA(const vector<Course>& courses);
void showOverallGPA(const vector<Course>& courses);
void showGradeDistribution(const vector<Course>& courses);

// What-if scenario
void whatIfScenario(const vector<Course>& courses);

// Editing / deleting helpers
void renameCourse(vector<Course>& courses);
void changeCourseCreditHours(vector<Course>& courses);
void editAssignmentInCourse(vector<Course>& courses);
void deleteAssignmentFromCourse(vector<Course>& courses);
void deleteCourse(vector<Course>& courses);

// ============================================================================
// MAIN FUNCTION
// ============================================================================
//3
int main() {
    vector<Course> courses;   // holds all the courses
    int nextCourseId = 1;     // each new course gets a new ID
    bool running = true;      // controls the main loop

    while (running) {
        // Show the main menu options to the user.
        showMainMenu();

        // Note: max choice is now 8 because we added new features.
        int choice = readIntInRange("Enter your choice: ", 0, 8);

        cout << "\n"; // blank line for readability

        switch (choice) {
            case 1:
                addCourse(courses, nextCourseId);
                break;
            case 2:
                listCoursesSummary(courses);
                break;
            case 3:
                addAssignmentToCourse(courses);
                break;
            case 4:
                showCourseDetails(courses);
                break;
            case 5:
                showOverallGPA(courses);
                break;
            case 6:
                whatIfScenario(courses);
                break;
            case 7:
                manageCoursesAndAssignments(courses);
                break;
            case 8:
                showGradeDistribution(courses);
                break;
            case 0:
                cout << "Exiting GPA & Grade Calculator. Goodbye!\n";
                running = false;
                break;
            default:
                // readIntInRange ensures we never actually hit this.
                cout << "Unknown menu choice.\n";
                break;
        }

        if (running) {
            // Let the user read the output before showing the menu again.
            pauseForUser();
        }

        cout << "\n"; // extra blank line between menu screens
    }

    return 0; // End of program.
}

// ============================================================================
// HELPER FUNCTION DEFINITIONS
// ============================================================================

// Discards leftover characters on the current input line.
void clearInputLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Pauses the program until the user presses Enter.
void pauseForUser() {
    cout << "Press Enter to continue...";
    clearInputLine();  // clear leftover input
    cin.get();         // wait for Enter
}

// Safely reads an int in [minValue, maxValue].
//4
int readIntInRange(const string& prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        int value;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            clearInputLine();
            cout << "Invalid input. Please enter a whole number.\n";
            continue;
        }

        clearInputLine();

        if (value < minValue || value > maxValue) {
            cout << "Please enter a number between " << minValue
                 << " and " << maxValue << ".\n";
            continue;
        }

        return value;
    }
}

// Safely reads a double in [minValue, maxValue].
double readDoubleInRange(const string& prompt, double minValue, double maxValue) {
    while (true) {
        cout << prompt;
        double value;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            clearInputLine();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        clearInputLine();

        if (value < minValue || value > maxValue) {
            cout << "Please enter a value between " << minValue
                 << " and " << maxValue << ".\n";
            continue;
        }

        return value;
    }
}

// ============================================================================
// MENUS
// ============================================================================

// Main menu.
void showMainMenu() {
    cout << "==========================================\n";
    cout << "         GPA & Grade Calculator\n";
    cout << "==========================================\n";
    cout << "1. Add a new course\n";
    cout << "2. List all courses (summary)\n";
    cout << "3. Add an assignment to a course\n";
    cout << "4. Show details for one course\n";
    cout << "5. Show overall GPA\n";
    cout << "6. What-if grade scenario\n";
    cout << "7. Manage courses & assignments (edit/delete)\n";
    cout << "8. Grade distribution report\n";
    cout << "0. Exit\n";
}

// Sub-menu for editing and deleting things.
void manageCoursesAndAssignments(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available yet.\n";
        return;
    }

    bool inSubMenu = true;
    while (inSubMenu) {
        cout << "==========================================\n";
        cout << "   Manage Courses & Assignments\n";
        cout << "==========================================\n";
        cout << "1. Rename a course\n";
        cout << "2. Change course credit hours\n";
        cout << "3. Edit an assignment's scores\n";
        cout << "4. Delete an assignment from a course\n";
        cout << "5. Delete a course\n";
        cout << "0. Return to main menu\n";

        int choice = readIntInRange("Enter your choice: ", 0, 5);
        cout << "\n";

        switch (choice) {
            case 1:
                renameCourse(courses);
                break;
            case 2:
                changeCourseCreditHours(courses);
                break;
            case 3:
                editAssignmentInCourse(courses);
                break;
            case 4:
                deleteAssignmentFromCourse(courses);
                break;
            case 5:
                deleteCourse(courses);
                // After deleting a course, the vector changes size.
                // That's okay; we can stay in the sub-menu.
                break;
            case 0:
                inSubMenu = false;
                break;
            default:
                cout << "Unknown choice.\n";
                break;
        }

        if (inSubMenu) {
            pauseForUser();
            cout << "\n";
        }
    }
}

// ============================================================================
// COURSE MANAGEMENT
// ============================================================================
//5
// Adds a new course with name and credit hours.
void addCourse(vector<Course>& courses, int& nextCourseId) {
    Course c;

    c.id = nextCourseId;
    nextCourseId++;  // increment for next course

    cout << "Enter course name (for example, COSC 3345): ";
    getline(cin, c.name);

    c.creditHours = readDoubleInRange(
        "Enter credit hours (e.g., 3 or 4): ", 0.5, 6.0);

    c.work = vector<Assignment>();

    courses.push_back(c);

    cout << "Course added with id " << c.id << ".\n";
}

// Finds course index by ID, or returns -1.
int findCourseIndexById(const vector<Course>& courses, int id) {
    for (size_t i = 0; i < courses.size(); ++i) {
        if (courses[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Lists summary info about each course.
void listCoursesSummary(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses have been added yet.\n";
        return;
    }

    cout << "Courses summary:\n";
    cout << "------------------------------------------------------------\n";
    for (const Course& c : courses) {
        cout << "ID: " << c.id
             << " | Name: " << c.name
             << " | Credits: " << c.creditHours;

        if (!c.work.empty()) {
            double percent = calculateCoursePercentage(c);
            string letter = percentageToLetter(percent);

            cout << " | Grade: " << fixed << setprecision(2)
                 << percent << "% (" << letter << ")";
        } else {
            cout << " | Grade: N/A (no assignments yet)";
        }

        cout << "\n";
    }
    cout << "------------------------------------------------------------\n";
}

// ============================================================================
// ASSIGNMENT MANAGEMENT
// ============================================================================

// Adds an assignment to a specific course.
void addAssignmentToCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "There are no courses yet. Add a course first.\n";
        return;
    }

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course to add an assignment to: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    Course& c = courses[index];
    Assignment a;

    cout << "Enter assignment name (for example, Exam 1): ";
    getline(cin, a.name);

    a.max = readDoubleInRange(
        "Enter maximum points for this assignment: ", 1.0, 10000.0);

    a.earned = readDoubleInRange(
        "Enter points earned on this assignment: ", 0.0, a.max);

    c.work.push_back(a);

    cout << "Assignment added to course '" << c.name << "'.\n";
}

// ============================================================================
// GRADE CALCULATIONS
// ============================================================================

// Calculates the overall percentage for a course as the average percentage
// across its assignments.

//6
double calculateCoursePercentage(const Course& course) {
    if (course.work.empty()) {
        return 0.0; // caller should check emptiness
    }

    double sumPercentages = 0.0;

    for (const Assignment& a : course.work) {
        double assignmentPercent = (a.earned / a.max) * 100.0;
        sumPercentages += assignmentPercent;
    }

    double averagePercent = sumPercentages /
        static_cast<double>(course.work.size());

    return averagePercent;
}

// Maps percentage to letter grade.
string percentageToLetter(double percent) {
    if (percent >= 90.0) {
        return "A";
    } else if (percent >= 80.0) {
        return "B";
    } else if (percent >= 70.0) {
        return "C";
    } else if (percent >= 60.0) {
        return "D";
    } else {
        return "F";
    }
}

// Maps letter grade to grade points on a 4.0 scale.
double letterToGradePoints(const string& letter) {
    if (letter == "A") {
        return 4.0;
    } else if (letter == "B") {
        return 3.0;
    } else if (letter == "C") {
        return 2.0;
    } else if (letter == "D") {
        return 1.0;
    } else {
        return 0.0; // F or anything else
    }
}

// ============================================================================
// COURSE DETAILS + GPA
// ============================================================================

// Shows detailed information about one course.
void showCourseDetails(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course to view details: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    const Course& c = courses[index];

    cout << "====================================================\n";
    cout << "Course details for: " << c.name << " (ID " << c.id << ")\n";
    cout << "Credit hours: " << c.creditHours << "\n";

    if (c.work.empty()) {
        cout << "No assignments have been added to this course yet.\n";
    } else {
        cout << left << setw(25) << "Name"
             << setw(15) << "Earned"
             << setw(15) << "Max"
             << setw(15) << "Percent"
             << "\n";

        cout << "----------------------------------------------------\n";

        for (const Assignment& a : c.work) {
            double percent = (a.earned / a.max) * 100.0;
            cout << left << setw(25) << a.name
                 << setw(15) << a.earned
                 << setw(15) << a.max
                 << setw(15) << fixed << setprecision(2) << percent
                 << "\n";
        }

        double coursePercent = calculateCoursePercentage(c);
        string letter = percentageToLetter(coursePercent);

        cout << "----------------------------------------------------\n";
        cout << "Course average: " << fixed << setprecision(2)
             << coursePercent << "% (" << letter << ")\n";
    }

    cout << "====================================================\n";
}

// Computes overall GPA across all courses using credit-hour weighting.
double calculateOverallGPA(const vector<Course>& courses) {
    double totalQualityPoints = 0.0;  // sum of (gradePoints * creditHours)
    double totalCredits = 0.0;        // sum of credit hours

    for (const Course& c : courses) {
        if (c.work.empty()) {
            continue; // skip ungraded courses
        }

        double coursePercent = calculateCoursePercentage(c);
        string letter = percentageToLetter(coursePercent);
        double gradePoints = letterToGradePoints(letter);

        totalQualityPoints += gradePoints * c.creditHours;
        totalCredits += c.creditHours;
    }

    if (totalCredits == 0.0) {
        return 0.0; // no graded courses yet
    }

    return totalQualityPoints / totalCredits;
}

// Shows overall GPA in a user-friendly way.
void showOverallGPA(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available yet.\n";
        return;
    }

    double gpa = calculateOverallGPA(courses);

    // Check if there is at least one course with assignments.
    bool hasGradedCourse = false;
    for (const Course& c : courses) {
        if (!c.work.empty()) {
            hasGradedCourse = true;
            break;
        }
    }

    if (!hasGradedCourse) {
        cout << "No graded courses yet. Add assignments first.\n";
        return;
    }

    cout << "==========================================\n";
    cout << "Overall GPA (4.0 scale): " << fixed << setprecision(2)
         << gpa << "\n";
    cout << "==========================================\n";
}

// ============================================================================
// WHAT-IF GRADE SCENARIO
// ============================================================================
//7
void whatIfScenario(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available yet. Add a course first.\n";
        return;
    }

    cout << "==========================================\n";
    cout << "         WHAT-IF GRADE SCENARIO\n";
    cout << "==========================================\n";
    cout << "This feature lets you test a hypothetical assignment.\n";
    cout << "The program will NOT save it. It only shows what would\n";
    cout << "happen if that assignment existed.\n\n";

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course for the what-if scenario: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    const Course& originalCourse = courses[index];

    cout << "\nYou selected course: " << originalCourse.name
         << " (ID " << originalCourse.id << ")\n";
    cout << "Credit hours: " << originalCourse.creditHours << "\n\n";

    bool hasAssignments = !originalCourse.work.empty();
    if (hasAssignments) {
        double currentPercent = calculateCoursePercentage(originalCourse);
        string currentLetter = percentageToLetter(currentPercent);

        cout << "Current course average: " << fixed << setprecision(2)
             << currentPercent << "% (" << currentLetter << ")\n";
    } else {
        cout << "This course currently has NO assignments.\n";
        cout << "So its course average is N/A for now.\n";
    }

    double oldGPA = calculateOverallGPA(courses);
    cout << "Current overall GPA: " << fixed << setprecision(2)
         << oldGPA << " (on a 4.0 scale)\n\n";

    Assignment hypothetical;

    cout << "Enter a name for the hypothetical assignment\n";
    cout << "(for example, \"Final Exam\" or \"Big Project\"):\n";
    getline(cin, hypothetical.name);

    hypothetical.max = readDoubleInRange(
        "Enter MAXIMUM possible points on this hypothetical assignment: ",
        1.0, 10000.0);

    hypothetical.earned = readDoubleInRange(
        "Enter the points you THINK you might earn: ",
        0.0, hypothetical.max);

    // Create a temporary copy of the courses.
    vector<Course> tempCourses = courses;

    // Modify only the copied course.
    Course& tempCourse = tempCourses[index];
    tempCourse.work.push_back(hypothetical);

    double newCoursePercent = calculateCoursePercentage(tempCourse);
    string newCourseLetter = percentageToLetter(newCoursePercent);
    double newGPA = calculateOverallGPA(tempCourses);

    cout << "\n------------------------------------------\n";
    cout << "RESULTS OF WHAT-IF SCENARIO\n";
    cout << "Course: " << originalCourse.name << "\n\n";

    if (hasAssignments) {
        double oldCoursePercent = calculateCoursePercentage(originalCourse);
        string oldCourseLetter = percentageToLetter(oldCoursePercent);

        cout << "Old course average: " << fixed << setprecision(2)
             << oldCoursePercent << "% (" << oldCourseLetter << ")\n";
    } else {
        cout << "Old course average: N/A (no assignments before what-if)\n";
    }

    cout << "New course average WITH hypothetical assignment: "
         << fixed << setprecision(2) << newCoursePercent
         << "% (" << newCourseLetter << ")\n\n";

    cout << "Old overall GPA: " << fixed << setprecision(2) << oldGPA << "\n";
    cout << "New overall GPA WITH hypothetical assignment: "
         << fixed << setprecision(2) << newGPA << "\n";
    cout << "------------------------------------------\n";
    cout << "Remember: This change was NOT saved.\n";
    cout << "It is only a simulation to help you plan.\n";
    cout << "------------------------------------------\n";
}

// ============================================================================
// EDITING / DELETING COURSES AND ASSIGNMENTS
// ============================================================================

// Rename a course.
//8
void renameCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course you want to rename: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    Course& c = courses[index];

    cout << "Current name: " << c.name << "\n";
    cout << "Enter new course name: ";
    string newName;
    getline(cin, newName);

    if (newName.empty()) {
        cout << "Name not changed (empty name entered).\n";
        return;
    }

    c.name = newName;
    cout << "Course name updated.\n";
}

// Change course credit hours.
void changeCourseCreditHours(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course whose credit hours you want to change: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    Course& c = courses[index];

    cout << "Current credit hours for " << c.name
         << ": " << c.creditHours << "\n";

    double newCredits = readDoubleInRange(
        "Enter new credit hours (0.5 to 6.0): ", 0.5, 6.0);

    c.creditHours = newCredits;
    cout << "Credit hours updated.\n";
}

// Edit an assignment's scores in a chosen course.
void editAssignmentInCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course containing the assignment: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    Course& c = courses[index];

    if (c.work.empty()) {
        cout << "This course has no assignments to edit.\n";
        return;
    }

    cout << "Assignments for course: " << c.name << "\n";
    for (size_t i = 0; i < c.work.size(); ++i) {
        const Assignment& a = c.work[i];
        cout << (i + 1) << ". " << a.name
             << " (earned " << a.earned << " / " << a.max << ")\n";
    }

    int choice = readIntInRange(
        "Enter the number of the assignment to edit: ",
        1, static_cast<int>(c.work.size()));

    Assignment& a = c.work[choice - 1];

    cout << "Editing assignment: " << a.name << "\n";

    // Let user either keep the name or change it
    cout << "Current name: " << a.name << "\n";
    cout << "Enter a new name, or just press Enter to keep it: ";
    string newName;
    getline(cin, newName);
    if (!newName.empty()) {
        a.name = newName;
    }

    double newMax = readDoubleInRange(
        "Enter NEW maximum points: ", 1.0, 10000.0);

    double newEarned = readDoubleInRange(
        "Enter NEW points earned: ", 0.0, newMax);

    a.max = newMax;
    a.earned = newEarned;

    cout << "Assignment updated.\n";
}

// Delete an assignment from a course.
void deleteAssignmentFromCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course containing the assignment to delete: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    Course& c = courses[index];

    if (c.work.empty()) {
        cout << "This course has no assignments to delete.\n";
        return;
    }

    cout << "Assignments for course: " << c.name << "\n";
    for (size_t i = 0; i < c.work.size(); ++i) {
        const Assignment& a = c.work[i];
        cout << (i + 1) << ". " << a.name
             << " (earned " << a.earned << " / " << a.max << ")\n";
    }

    int choice = readIntInRange(
        "Enter the number of the assignment to DELETE: ",
        1, static_cast<int>(c.work.size()));

    cout << "Are you sure you want to delete assignment '"
         << c.work[choice - 1].name << "'? (1 = Yes, 0 = No): ";
    int confirm;
    cin >> confirm;
    if (cin.fail()) {
        cin.clear();
    }
    clearInputLine();

    if (confirm != 1) {
        cout << "Deletion cancelled.\n";
        return;
    }

    c.work.erase(c.work.begin() + (choice - 1));
    cout << "Assignment deleted.\n";
}

// Delete a course completely.
void deleteCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    listCoursesSummary(courses);

    int id = readIntInRange(
        "Enter the ID of the course to DELETE: ",
        1, 1000000);

    int index = findCourseIndexById(courses, id);
    if (index == -1) {
        cout << "No course found with that ID.\n";
        return;
    }

    const Course& c = courses[index];

    cout << "Are you sure you want to delete the entire course '"
         << c.name << "' and all its assignments? (1 = Yes, 0 = No): ";
    int confirm;
    cin >> confirm;
    if (cin.fail()) {
        cin.clear();
    }
    clearInputLine();

    if (confirm != 1) {
        cout << "Course deletion cancelled.\n";
        return;
    }

    courses.erase(courses.begin() + index);
    cout << "Course deleted.\n";
}

// ============================================================================
// GRADE DISTRIBUTION REPORT
// ============================================================================

// Shows how many courses currently have A, B, C, D, or F.
void showGradeDistribution(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }

    int countA = 0;
    int countB = 0;
    int countC = 0;
    int countD = 0;
    int countF = 0;

    // We only consider courses that have at least one assignment.
    for (const Course& c : courses) {
        if (c.work.empty()) {
            continue;
        }

        double percent = calculateCoursePercentage(c);
        string letter = percentageToLetter(percent);

        if (letter == "A") {
            countA++;
        } else if (letter == "B") {
            countB++;
        } else if (letter == "C") {
            countC++;
        } else if (letter == "D") {
            countD++;
        } else {
            countF++;
        }
    }

    int totalGradedCourses = countA + countB + countC + countD + countF;

    if (totalGradedCourses == 0) {
        cout << "No graded courses yet. Add assignments first.\n";
        return;
    }

    cout << "==========================================\n";
    cout << "         GRADE DISTRIBUTION REPORT\n";
    cout << "==========================================\n";
    cout << "Total graded courses: " << totalGradedCourses << "\n";
    cout << "A: " << countA << "\n";
    cout << "B: " << countB << "\n";
    cout << "C: " << countC << "\n";
    cout << "D: " << countD << "\n";
    cout << "F: " << countF << "\n";
    cout << "==========================================\n";
}
