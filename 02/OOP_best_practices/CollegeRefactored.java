import java.util.List;

public class CollegeRefactored {

	class Group {

		private List<Student> students;

		public int getStudentsCount() {
			return students.size();
		}

		// rest of the Group logic
	}

	class StudentsPerGroupStats {

		private Group[] groups;

		public int getStudentsCount() {
			int numberOfStudents = 0;
			for (Group current : groups) {
				numberOfStudents += current.getStudentsCount();
			}
			return numberOfStudents;
		}

		// rest of the StudentsPerGroupStats logic
	}

}
