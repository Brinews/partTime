import java.awt.BorderLayout;


import javax.swing.*;
import java.awt.*;


public class chessboard {
	public static void main(String args[]){
		EventQueue.invokeLater(new Runnable()
        {
           public void run()
           {
        	player p1=new player("h");
        	player p2=new player("g");
        	String pre_url,image_url="";
       		pre_url="src\\chessBMP\\";
       		String s[]={"hrook","hknight","hbishop","hking","hqueen","hbishop","hknight","hrook","hpawn","grook","gknight","gbishop","gking","gqueen","gbishop","gknight","grook","gpawn"};
       		String color[]={"b","w","hhh"};
       		piece pp[][]=new piece[8][8];
       		JFrame f1=new JFrame("CHESS GAME");       		
       		f1.setSize(768,480);   
       		f1.setLayout(null);
       	    JPanel board=new JPanel();       	  
       	    board.setSize(384, 384);
       	    board.setLayout(new GridLayout(8,8));       	   
       	           	    
       		mouselistener l=new mouselistener(pp,p1,p2);
       		for(int i=1;i<9;i++)
       		{
       			for(int j=1;j<9;j++)
       			{
       				
       				image_url=pre_url+color[(j+i-1)%2]+".bmp";
       				point xy=new point(i-1,j-1);
       				pp[i-1][j-1]=new piece(color[(j+i-1)%2],image_url,color[(j+i-1)%2],xy);
       				pp[i-1][j-1].setVisible(true);
       				
       				pp[i-1][j-1].addMouseListener(l);
       					
       				
       				board.add(pp[i-1][j-1]);
       			}
       		}
       		for(int i=1;i<9;i++)
       		{
       			for(int j=1;j<9;j++)
       			{
       				if(i==1)
       				{
       					point xy=new point(i-1,j-1);
       					image_url=pre_url+color[j%2]+s[j-1]+".bmp";
       					pp[i-1][j-1].setpiece(s[j-1],image_url,color[(j+i-1)%2],xy,p1);
       					p1.setp(pp[i-1][j-1]);
       					
       					
       				}
       				if(i==2)
       				{
       					point xy=new point(i-1,j-1);
       					image_url=pre_url+color[(j+1)%2]+s[8]+".bmp";
       					pp[i-1][j-1].setpiece(s[8],image_url,color[(j+i-1)%2],xy,p1);
       					p1.setp(pp[i-1][j-1]);
       					
       				}
       				if(i==7)
       				{
       					point xy=new point(i-1,j-1);
       					image_url=pre_url+color[j%2]+s[17]+".bmp";
       					pp[i-1][j-1].setpiece(s[17],image_url,color[(j+i-1)%2],xy,p2);
       					p2.setp(pp[i-1][j-1],j+7);
       					
       				}
       				if(i==8)
       				{
       					point xy=new point(i-1,j-1);
       					image_url=pre_url+color[(j+1)%2]+s[j+8]+".bmp";
       					pp[i-1][j-1].setpiece(s[j+8],image_url,color[(j+i-1)%2],xy,p2);
       					p2.setp(pp[i-1][j-1],j-1);
       					
       				}
       			}
       		}   
       		
       		JPanel label_east=new JPanel();
       		label_east.setLayout(new BorderLayout());
       		label_east.setSize(384, 384);
       		
       		JButton restart=new JButton("restart");
       		restart.setSize(384,20);
       		JTextArea text1=new JTextArea(20,30);
       		text1.setText("The game is ready!");
       		text1.setEditable(false);
       		text1.setBorder(BorderFactory.createLineBorder(Color.RED,5));       	
       		//text1.setSize(366, 500);     		
      		label_east.add(restart,BorderLayout.NORTH);      	
      		label_east.add(text1);    
      		
      		piece[][] ppp=new piece[2][16];
      		JPanel ss=new JPanel();       	  
       	    ss.setSize(768, 96);
       	    ss.setLayout(new GridLayout(2,16));       	   
       	           	    
       	
       		for(int i=1;i<3;i++)
       		{
       			for(int j=1;j<17;j++)
       			{
       				
       				image_url=pre_url+color[2]+".bmp";
       				point xy=new point(i-1,j-1);
       				ppp[i-1][j-1]=new piece(color[(j+i-1)%2],image_url,color[(j+i-1)%2],xy);
       				ppp[i-1][j-1].setVisible(true);
       				
       				ppp[i-1][j-1].addMouseListener(l);
       					
       				
       				ss.add(ppp[i-1][j-1]);
       			}
       		}
       		label_east.setBounds(384, 0, 384, 384);
       		board.setBounds(0, 0, 384, 384);
       		ss.setBounds(0, 384, 768, 96);
      		f1.add(board);f1.add(label_east);f1.add(ss);
      		f1.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
       		f1.setVisible(true);
       		
           }
        });
		
		
		
		
	   }

}
