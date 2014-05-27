
public class player {
	public player(String s)
	{
		name=s;
		life=1;
		pi=new piece[16];
		i=0;
	}
	public player(String s,int j)
	{
		name=s;
		life=1;
		pi=new piece[16];
		i=16;
	}
	public void setp(piece p)
	{
		pi[i]=p;
	}
	public void setp(piece p,int j)
	{
		pi[j]=p;
	}
	public String getname()
	{
		return name;
	}
	private piece[] pi;
	private int i;
	private String name;
    private int life;//0代表死；1代表活
}
