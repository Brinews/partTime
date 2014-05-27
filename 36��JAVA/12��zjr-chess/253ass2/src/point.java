
public class point {
	public point()
	{
		X=0;
		Y=0;
	}
	public point(int i,int j)
	{
		X=i;
		Y=j;
	}
	public void setX(int i)
	{
		X=i;
	}
	public void setY(int i)
	{
		Y=i;
	}
	public void setpoint(int i,int j)
	{
		setX(i);
		setY(j);
	}
	
	public  int getX()
	{
		return X;
	}
	public int getY()
	{
		return Y;
	}
	public point getpoint()
	{
		return this;
	} 
	
	private int X;
	private int Y;
}
