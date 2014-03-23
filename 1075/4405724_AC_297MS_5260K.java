import java.util.*;

public class Main{
	private static final Scanner in=new Scanner(System.in);
	private Student[] students;
	private University[] universities;
	private class Student{
		private int region,score;
		private List<University> priorityList=new ArrayList<University>();
		private University acceptedTo=null;
		public void Input(){
			region=in.nextInt();
			score=in.nextInt();
			int listSize=in.nextInt();
			while(listSize-->0){
				University university=universities[in.nextInt()-1];
				priorityList.add(university);
				university.studentsWithWill.add(this);
			}
		}
		public double AvailableScore(int region){
			return score*(this.region==region?1.0:0.7);
		}
		public boolean Try(University university){
			if(acceptedTo==null){
				acceptedTo=university;
				return true;
			}
			if(priorityList.indexOf(acceptedTo)>priorityList.indexOf(university)){
				acceptedTo.capacity++;
				acceptedTo=university;
				return true;
			}
			return false;
		}
	}
	private class University{
		private int region,capacity,ID;
		private PriorityQueue<Student> studentsWithWill;
		public University(int ID){
			this.ID=ID;
			studentsWithWill=new PriorityQueue<Student>(1,
					new Comparator<Student>(){
						public int compare(Student left,Student right){
							return Double.compare(right.AvailableScore(region),left.AvailableScore(region));
						}
					});
		}
		public void Input(){
			region=in.nextInt();
			capacity=in.nextInt();
		}
	}
	public Main(){
		students=new Student[in.nextInt()];
		universities=new University[in.nextInt()];
		for(int i=0;i<students.length;i++)
			students[i]=new Student();
		for(int ID=1;ID<=universities.length;ID++)
			universities[ID-1]=new University(ID);
		for(Student student:students)
			student.Input();
		for(University university:universities)
			university.Input();
		while(true){
			boolean anyChange=false;
			for(University university:universities){
				while(university.capacity>0 && !university.studentsWithWill.isEmpty()){
					Student student=university.studentsWithWill.poll();
					if(student.Try(university)){
						university.capacity--;
						anyChange=true;
					}
				}
			}
			if(!anyChange)
				break;
		}
	}
	public void Output(){
		for(Student student:students){
			if(student.acceptedTo==null)
				System.out.println("not accepted");
			else
				System.out.println(student.acceptedTo.ID);
		}
		System.out.println();
	}
	public static void main(String[] args){
		int caseNum=in.nextInt();
		while(caseNum-->0){
			Main testCase=new Main();
			testCase.Output();
		}
	}
}