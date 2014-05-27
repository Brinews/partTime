import java.util.*;

class Gamer
{
	private int times;
	private int curTotal;
	private String name;

	public Gamer(String n)
	{
		name = n;
	}

	public String getName() { return name; }
	public int getTimes() { return times; }
	public int getGamerTotal() { return curTotal; }

	public void addTimes() { times++; }
	public void addTotal(int t) { curTotal += t; }
	public void setTotal(int total) { curTotal = total; }
	public void reset()
	{
		times = 0;
		curTotal = 0;
	}

	public boolean isWin() { return curTotal == 100; }

	public boolean isContinue() 
	{
		String input;
		Scanner in = new Scanner(System.in);
		System.out.println("Continue or Not(Y/N):");

		input = in.nextLine();
		//System.out.println("INPUT=" + input);

		if ("Y".equals(input)) return true;
		else if ("N".equals(input)) return false;
		else
		{
			System.exit(0);
			return false;
		}
	}
}

public class RaceTo100 {

	private List<Gamer> userList = new ArrayList<Gamer>();

	public RaceTo100()
	{
	}

	public int getNextNumber()
	{
		int r = (int) (Math.random() * 100);
		return r%6 + 1; 
	}

	public int addGamer(String name)
	{
		String aName;
		Gamer aGamer;

		for (int i = 0; i < userList.size(); i++) {
			aGamer = userList.get(i);

			if (name.equals(aGamer.getName())) {
				System.out.println("Gamer already Exist.");
				return 0;
			}
		}

		userList.add(new Gamer(name));

		return userList.size();
	}

	public void resetGamer()
	{
		for (int i = 0; i < userList.size(); i++)
		{
			Gamer g = userList.get(i);
			g.reset();
		}
	}

	public void simulate()
	{
		int i = 0;

		int p, q;
		int total = 0;

		while (true)
		{
			if (userList.size() == 0) break;

			Gamer g = userList.get(i);
			g.setTotal(total);

			do {
				p = getNextNumber();
				q = getNextNumber();
				System.out.println("user[" + g.getName() + "] get two number: " + p + ", " + q);

				if (p != q) 
				{
					g.addTotal(p + q);
					g.addTimes();
				}

				if (p == q && p == 1)
				{
					System.out.println("Game Restart, All Gamer begin with zero");
					resetGamer();
					i = 0;
				}

				if (p == q && p != 1) 
				{
					System.out.println(g.getName() + " Get Total = " + g.getGamerTotal());
					//break;
				}

				total = g.getGamerTotal();

				if (g.isWin()) {
					System.out.println(g.getName() + " Race To 100 After " + g.getTimes() + " Times try.");
					return;
				}
			} while(g.isContinue());

			/** next gamer */
			i++;
			i = i%userList.size();
		}
	}

	public static void main(String[] args)
	{
		RaceTo100 rt100 = new RaceTo100();

		Scanner in = new Scanner(System.in);
		System.out.println("Input Gamer Number : ");
		int num = in.nextInt();
		in.nextLine();

		System.out.println("Input Each Gamer Name(one by line): ");
		for (int i = 0; i < num; i++)
		{
			String str = in.nextLine();
			rt100.addGamer(str);
		}

		rt100.simulate();
	}
}
