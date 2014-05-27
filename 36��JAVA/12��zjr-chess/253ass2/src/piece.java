import java.awt.*;

import java.io.*;

import javax.imageio.*;
import javax.swing.*;
public class piece extends JPanel{
	public piece(String s,String url,String c,point location)
	{
		piecelocation=location;
		name=s;
		color=c;
		try {
			image=ImageIO.read(new File(url));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		play=null;
		
	}
	public void paintComponent(Graphics g){
	      super.paintComponent(g);
	      g.drawImage(image,0,0,48,48,null);
	      
	  }     
	
	public void setImage(String url)
	{
		try {
			image=ImageIO.read(new File(url));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void setName(String s)
	{
		name=s;
	}
	public void setLocation(point location)
	{
		piecelocation=location;
	}
	public void setColor(String c)
	{
		color=c;
	}
	public void setpiece(String s,String url,String c,point location)
	{
		setImage(url);
		setName(s);
		setLocation(location);
		setColor(c);
	}
	public void setpiece(String s,String url,String c,point location,player pl)
	{
		setImage(url);
		setName(s);
		setLocation(location);
		setColor(c);
		play=pl;
	}
	public String getName()
	{
		return name;
	}
	public String getColor()
	{
		return color;
	}
	public point getlocation()
	{
		return piecelocation;
	}
	public Image getImage()
	{
		return image;
	}
	public player getplay()
	{
		return play;
	}
	
	private String name="";
	private Image image;
	private point piecelocation;//Æå×ÓµÄÎ»ÖÃ
	private String color="";
	private player play;
	
}
