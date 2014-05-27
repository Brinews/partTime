import java.awt.Color;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.BorderFactory;


public class mouselistener extends MouseAdapter{
	public mouselistener(piece[][] pt,player pi,player pj)
	{
		pp=pt;
		r=new rule(pt);
		jishu=0;
		cc=new piece[64];
		dangqianplay=pi;
		pone=pi;
		ptwo=pj;
		//ppp={pi,pj};
	}
	public void mouseClicked(MouseEvent event)
	{
		 
		p=(piece)event.getSource();		
		if(p.getplay()==null||p.getplay()==dangqianplay){
		 if(jilu!=null)
			{ 
			 if(r.isok(jilu,p))
				 //if(true)
			 	{
			 		p.setpiece(jilu.getName(),"src\\chessBMP\\"+p.getColor()+jilu.getName()+".bmp",p.getColor(),p.getlocation());
			 		jilu.setpiece(jilu.getColor(),"src\\chessBMP\\"+jilu.getColor()+".bmp", jilu.getColor(), jilu.getlocation());
			 		for(int i=0;i<jishu;i++)
				    {
				    	cc[i].setBorder(null);
				    } 
			 		p.setBorder(null);
					 jilu.setBorder(null);
			 		jilu.repaint();
			 		p.repaint();
			 		jilu=null;
			 		if(dangqianplay==pone){dangqianplay=ptwo;}
			 		else{dangqianplay=pone;}
			 	}else{
				    for(int i=0;i<jishu;i++)
				    {
				    	cc[i].setBorder(null);
				    }
				    p.setBorder(null);
			 		jilu.setBorder(null);
			 		jilu=null;}
			}
		else
			{
				if((p.getName()!="b")&&(p.getName()!="w"))
					{
					System.out.print(""+p.getlocation().getX()+" "+p.getlocation().getY());
					
						jilu=p;
						 p.setBorder(BorderFactory.createLineBorder(Color.RED,5));
						 jishu=0;
						 for(int i=0;i<8;i++)
						 {
							 for(int j=0;j<8;j++)
							 {
								if(r.isok(p, pp[i][j])) 
								{
									pp[i][j].setBorder(BorderFactory.createLineBorder(Color.RED,5));
									cc[jishu]=pp[i][j];
									jishu++;
								}
							 }
						 }
					}
			}
		 //dangqianplay=null;
		 }
	}
	
	public void mousePressed(MouseEvent event)
    {
		
		 
		
    }
	private piece p=null;
	private  piece jilu=null;
	private piece[][] pp;
	private piece[] cc;
	int jishu;
	private rule r;
	private player dangqianplay,pone,ptwo;
	//private player[] ppp=new player[3];
}
