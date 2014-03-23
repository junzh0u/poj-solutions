import java.util.*;

public class Main {
	private SortedMap<String,Agent> agents=new TreeMap<String,Agent>();
	private Map<String,Room> rooms=new HashMap<String,Room>();
	private Map<Integer,Elevator> elevators=new HashMap<Integer,Elevator>();
	private void Initialize(){
		final int FLOOR_NUM=10,ROOM_IN_FLOOR=10;
		for(int floor=1;floor<=FLOOR_NUM;floor++){
			elevators.put(floor,new Elevator(floor));
			for(int no=1;no<=ROOM_IN_FLOOR;no++){
				Room room=new Room(floor,no);
				rooms.put(room.GetName(),room);
			}
		}
	}
	private class Agent implements Comparable<Agent>{
		private String ID;
		private Time timeEnter;
		private Stack<Place> places=new Stack<Place>();
		private Stack<Integer> times=new Stack<Integer>();
		private AgentEvent event;
		private String logs=new String();
		public Agent(String ID){
			final String END_OF_INPUT="0";
			this.ID=ID;
			timeEnter=new Time(in.next());
			int currentFloor=1;
			while(true){
				String roomName=in.next();
				if(roomName.equals(END_OF_INPUT))
					break;
				Room room=rooms.get(roomName);
				int timeStay;
				if(currentFloor!=room.GetFloor()){
					timeStay=(Math.abs(currentFloor-room.GetFloor()))*30;
					places.add(elevators.get(currentFloor));
					times.add(timeStay);
					currentFloor=room.GetFloor();
				}
				timeStay=in.nextInt();
				places.add(room);
				times.add(timeStay);
			}
			if(currentFloor!=1){
				places.add(elevators.get(currentFloor));
				times.add(Math.abs(currentFloor-1)*30);
				currentFloor=1;
			}
			event=CreateEvent();
			event.Activate(timeEnter);
		}
		private AgentEvent CreateEvent(){
			Event event=new Exit(this);
			Place next=null;
			while(!places.isEmpty()){
				Place current=places.pop();
				int timeStay=times.pop();
				if(next!=null){
					event=new Transfer(this,event,current,next);
				}
				event=new Stay(this,event,current,timeStay);
				event=new Wait(this,event,current);
				next=current;
			}
			return new Entry(this,event);
		}
		public void Log(String line){
			logs+=line+"\n";
		}
		public int compareTo(Agent another){
			return ID.compareTo(another.ID);
		}
		public boolean equals(Agent another){
			return compareTo(another)==0;
		}
		public String toString(){
			return ID+"\n"+logs;
		}
	}
	private abstract class Place{
		protected int floor;
		protected Announce announce;
		public Announce GetAnnounce(){
			return announce;
		}
		public int GetFloor(){
			return floor;
		}
		public abstract String toString();
	}
	private class Room extends Place{
		private int no;
		public Room(String name){
			this(Integer.parseInt(name.substring(0,2)),Integer.parseInt(name.substring(2)));
		}
		public Room(int floor,int no){
			this.floor=floor;
			this.no=no;
			announce=new RoomAnnounce(this);
		}
		public String GetName(){
			return String.valueOf(floor+100).substring(1)+String.valueOf(no+100).substring(1);
		}
		public String toString(){
			return "room "+GetName();
		}
	}
	private class Elevator extends Place{
		public Elevator(int floor){
			this.floor=floor;
			announce=new ElevatorAnnounce(this);
		}
		public String toString(){
			return "elevator";
		}
	}
	private class Time implements Comparable<Time>{
		private int hour,minute,second;
		public Time(){
			this(Integer.MAX_VALUE);
		}
		public Time(String time){
			hour=Integer.parseInt(time.substring(0,2));
			minute=Integer.parseInt(time.substring(3,5));
			second=Integer.parseInt(time.substring(6,8));
		}
		public Time(int seconds){
			hour=seconds/3600;
			minute=(seconds%3600)/60;
			second=seconds%60;
		}
		public int GetSeconds(){
			return hour*3600+minute*60+second;
		}
		public Time Advance(int second){
			return new Time(GetSeconds()+second);
		}
		public int compareTo(Time another){
			if(GetSeconds()<another.GetSeconds())
				return -1;
			else if(GetSeconds()==another.GetSeconds())
				return 0;
			else
				return 1;
		}
		public boolean equals(Time another){
			return compareTo(another)==0;
		}
		public String toString(){
			return 	String.valueOf(100+hour).substring(1)+":"+
					String.valueOf(100+minute).substring(1)+":"+
					String.valueOf(100+second).substring(1);
		}
	}
	private class TimeLine{
		private SortedSet<Event> events=new TreeSet<Event>();
		public TimeLine(){
		}
		public void Add(Event event){
			events.add(event);
		}
		public boolean Contains(Event event){
			return events.contains(event);
		}
		public void Run(){
			while(!events.isEmpty()){
				Event next=events.first();
				events.remove(next);
				next.Run();
			}
		}
	}
	private TimeLine timeLine=new TimeLine();;
	private abstract class Event implements Comparable<Event>{
		protected Time nextAct=new Time();
		protected int priority;
		public Event(){
		}
		public int compareTo(Event another){
			if(!nextAct.equals(another.nextAct))
				return nextAct.compareTo(another.nextAct);
			if(priority>another.priority)
				return -1;
			if(priority<another.priority)
				return 1;
			return 0;
		}
		public boolean equals(Event another){
			return compareTo(another)==0;
		}
		public abstract void Run();
	}
	private abstract class AgentEvent extends Event{
		public static final int NOT_STARTED=0,RUNNING=1,ENDED=2;
		protected int status=NOT_STARTED;
		protected Agent visitor;
		protected AgentEvent next;
		protected Time start,end;
		public AgentEvent(Agent visitor,Event next){
			this.visitor=visitor;
			priority=(int)visitor.ID.charAt(0)+10000;
			Link(next);
		}
		public void Activate(Time time){
			nextAct=time;
			timeLine.Add(this);
		}
		public void Link(Event next){
			this.next=(AgentEvent)next;
		}
		public void Run(){
			switch(status){
			case NOT_STARTED:
				start=nextAct;
				Start();
				status=RUNNING;
				break;
			case RUNNING:
				end=nextAct;
				End();
				WriteLog();
				status=ENDED;
			}
		}
		protected abstract void Start();
		protected abstract void End();
		private void WriteLog(){
			if(!start.equals(end))
				visitor.Log(start+" "+end+" "+this);
		}
	}
	private class Wait extends AgentEvent{
		private Place waitFor;
		public Wait(Agent visitor,Event next,Place waitFor){
			super(visitor,next);
			this.waitFor=waitFor;
		}
		protected void Start(){
			waitFor.GetAnnounce().Register(this);
			nextAct=new Time();
		}
		protected void End(){
			next.Activate(end);
		}
		public String toString(){
			if(waitFor instanceof Room)
				return "Waiting in front of "+waitFor;
			return "Waiting in elevator queue";
		}
	}
	private class Stay extends AgentEvent{
		private Place stayIn;
		private int timeLength;
		public Stay(Agent visitor,Event next,Place stayIn,int timeLength){
			super(visitor,next);
			this.stayIn=stayIn;
			this.timeLength=timeLength;
		}
		protected void Start(){
			Activate(nextAct.Advance(timeLength));
		}
		protected void End(){
			stayIn.GetAnnounce().Recall(end);
			next.Activate(end);
		}
		public String toString(){
			return "Stay in "+stayIn;
		}
	}
	private class Transfer extends AgentEvent{
		public static final int COST=10;
		protected int timeCost;
		private Place from,to;
		public Transfer(Agent visitor,Event next,Place from,Place to){
			super(visitor,next);
			this.from=from;
			this.to=to;
			timeCost=COST;
		}
		protected void Start(){
			Activate(nextAct.Advance(timeCost));
		}
		protected void End(){
			if(next!=null)
				next.Activate(end);
		}
		public String toString(){
			return "Transfer from "+from+" to "+to;
		}
	}
	private class Entry extends Transfer{
		public static final int COST=30;
		public Entry(Agent visitor,Event next){
			super(visitor,next,null,null);
			timeCost=COST;
		}
		public String toString(){
			return "Entry";
		}
	}
	private class Exit extends Transfer{
		public static final int COST=30;
		public Exit(Agent visitor){
			super(visitor,null,null,null);
			timeCost=COST;
		}
		public String toString(){
			return "Exit";
		}
	}
	private abstract class Announce extends Event{
		protected SortedSet<AgentEvent> nexts=new TreeSet<AgentEvent>();
		protected Place place;
		public Announce(Place place){
			this.place=place;
		}
		public abstract void Register(AgentEvent next);
		public abstract void Recall(Time time);
	}
	private class RoomAnnounce extends Announce{
		private boolean occupied=false;
		public RoomAnnounce(Room room){
			super(room);
			priority=room.floor*100+room.no;
		}
		public void Register(AgentEvent next){
			if(nexts.isEmpty() && !occupied){
				nextAct=next.nextAct;
				timeLine.Add(this);
			}
			nexts.add(next);
		}
		public void Recall(Time time){
			occupied=false;
			nextAct=time;
			timeLine.Add(this);
		}
		public void Run(){
			if(nexts.isEmpty())
				return;
			AgentEvent next=nexts.first();
			next.Activate(nextAct);
			occupied=true;
			nexts.remove(next);
		}
	}
	private class ElevatorAnnounce extends Announce{
		public static final int INTERNAL=5;
		public ElevatorAnnounce(Elevator elevator){
			super(elevator);
			priority=elevator.floor;
		}
		public void Register(AgentEvent next){
			if(nexts.isEmpty()){
				Time time=next.nextAct;
				if(time.GetSeconds()%5!=0)
					time=time.Advance(5-time.GetSeconds()%5);
				nextAct=time;
				timeLine.Add(this);
			}
			nexts.add(next);
		}
		public void Recall(Time time){
		}
		public void Run(){
			if(nexts.isEmpty())
				return;
			AgentEvent next=nexts.first();
			next.Activate(nextAct);
			nexts.remove(next);
			//nextResponse=nextAct.Advance(INTERNAL);
			if(!nexts.isEmpty()){
				nextAct=nextAct.Advance(INTERNAL);
				timeLine.Add(this);
			}
		}
	}
	private Main(){
		Initialize();
		final String END_OF_INPUT=".";
		while(true){
			String ID=in.next();
			if(ID.equals(END_OF_INPUT))
				break;
			agents.put(ID,new Agent(ID));
		}
		timeLine.Run();
	}
	private void Output(){
		Iterator<Agent> iter=agents.values().iterator();
		while(iter.hasNext()){
			Agent agent=iter.next();
			System.out.println(agent);
		}
	}
	private static final Scanner in=new Scanner(System.in);
	public static void main(String[] args){
		Main department=new Main();
		department.Output();
	}
}