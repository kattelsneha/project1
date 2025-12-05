# GPA & Grade Calculator (C++)

This project is a console-based **GPA & Grade Calculator** written in C++.  
It helps a college student keep track of courses, assignments, and grades, and see their overall GPA on a 4.0 scale.

---

## Features

- Add courses with:
  - Course name (e.g., `COSC 3345`)
  - Credit hours (e.g., `3` or `4`)
- Add assignments to each course:
  - Assignment name (e.g., `Exam 1`, `Homework 2`)
  - Points earned and maximum points
- Automatically calculate:
  - Course average percentage
  - Letter grade (A/B/C/D/F)
  - Overall GPA on a 4.0 scale (weighted by credit hours)
- **What-if scenario**:
  - Test a hypothetical assignment and see how it would change the course grade and overall GPA (without saving it)
- **Edit & delete**:
  - Rename a course
  - Change course credit hours
  - Edit assignment scores and name
  - Delete assignments
  - Delete an entire course
- **Grade distribution report**:
  - Shows how many courses currently have A, B, C, D, or F

---
## link to presentation
https://drive.google.com/drive/folders/1f-NNLfw0J6R3XGFV1u4YeC3f_YQdLx3x?usp=sharing
## How to Run

1. Clone or download this repository.
2. Open the project in your favorite C++ environment (local or online).
3. Compile and run the `main.cpp` file.

Example (with `g++`):

```bash
g++ -std=c++11 main.cpp -o gpa_calculator
./gpa_calculator


