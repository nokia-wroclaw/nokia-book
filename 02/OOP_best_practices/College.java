public class College {

	class Group {

		private Student[] students;

		public Student[] getStudents() {
			return students;
		}

		// rest of the Group logic
	}

	class StudentsPerGroupStats {

		private Group[] groups;

		public int getStudentsCount() {
			int numberOfStudents = 0;
			for (Group current : groups) {
				numberOfStudents += current.getStudents().length;
			}
			return numberOfStudents;
		}

		// rest of the StudentsPerGroupStats logic
	}

}
