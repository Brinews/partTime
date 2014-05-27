
public class rule {
	public rule(piece[][] pt)
	{
		pp=pt;
	}
	/*public boolean issamecolor(piece source,piece target)
	{
		String s=source.getColor();
		String t=target.getColor();
		if(s==t) return true;
		else return false;
	}*/
	public boolean isinl(point s,point t)
	{
		boolean flog=false;
		int sx=s.getX();
		int sy=s.getY();
		int tx=t.getX();
		int ty=t.getY();
		if(sx==tx) return flog=true;
		if(sy==ty) return flog=true;
		return flog;
	}
	public boolean isinx(point s,point t)
	{
		boolean flog=false;
		int sx=s.getX();
		int sy=s.getY();
		int tx=t.getX();
		int ty=t.getY();
		if((sx+sy)==(tx+ty)) return flog=true;
		if((sx-sy)==(tx-ty)) return flog=true;
		return flog;
	}
	public boolean isinL(point s,point t)
	{
		boolean flog=false;
		int sx=s.getX();
		int sy=s.getY();
		int tx=t.getX();
		int ty=t.getY();
		if((sx-2==tx)&&(sy-1==ty))return flog=true;
		if((sx-1==tx)&&(sy-2==ty))return flog=true;
		if((sx+1==tx)&&(sy-2==ty))return flog=true;
		if((sx+2==tx)&&(sy-1==ty))return flog=true;
		if((sx-2==tx)&&(sy+1==ty))return flog=true;
		if((sx-1==tx)&&(sy+2==ty))return flog=true;
		if((sx+1==tx)&&(sy+2==ty))return flog=true;
		if((sx+2==tx)&&(sy+1==ty))return flog=true;
		return flog;
	}
	/*public boolean isinline(point s,point t)
	{
		if(isinl(s,t)) return true;
		 return false;
	}*/
	/*public float getmove(point s,point t)
	{
		int sx=s.getX();
		int sy=s.getY();
		int tx=t.getX();
		int ty=t.getY();
		int x=tx-sx;
		int y=ty-sy;
		float q=(float)x/(float)y;
		return q;
	}*/
	public boolean isv(point s,point t)
	{
		boolean f=false;
		int sx=s.getX();
		int sy=s.getY();
		int tx=t.getX();
		int ty=t.getY();
		int mx=0;
		int my=0;
		if((tx-sx)<0) mx=-1;
		if((tx-sx)==0) mx=0;
		if((tx-sx)>0) mx=1;
		if((ty-sy)<0) my=-1;
		if((ty-sy)==0) my=0;
		if((ty-sy)>0) my=1;
		int newx=sx+mx;
		int newy=sy+my;
		while(pp[newx][newy]!=pp[tx][ty])
		{
			if((pp[newx][newy].getName()!="b")&&(pp[newx][newy].getName()!="w")) return f;
			newx +=mx;
			newy +=my;
		}
		return f=true;
	}
	public boolean queen(piece source,piece target)
	{
		boolean f=false;
		point t=target.getlocation();
		point s=source.getlocation();
		if(!isinl(s,t)&&!isinx(s,t))
		{
			
			return f;
		}else{
			if(!isv(s,t))//有障碍
			{
				
				return f;
			}else{
				return true;
				}
		}
	}
	public boolean rook(piece source,piece target)
	{
		boolean f=false;
		point t=target.getlocation();
		point s=source.getlocation();
		if(!isinl(s,t))
		{
			
			return f;
		}else{
			if(!isv(s,t))//有障碍
			{
				
				return f;
			}else{
				return true;
				}
		}
	}
	public boolean knight(piece source,piece target)
	{
		boolean f=false;
		point t=target.getlocation();
		point s=source.getlocation();
		if(!isinL(s,t))
		{
			
			return f;
		}else{
			return true;
		}
	}
	public boolean bishop(piece source,piece target)
	{
		boolean f=false;
		point t=target.getlocation();
		point s=source.getlocation();
		if(!isinx(s,t))
		{
			
			return f;
		}else{
			if(!isv(s,t))//有障碍
			{
				
				return f;
			}else{
				return true;
				}
		}
	}
	public boolean king(piece source,piece target)
	{
		boolean f=false;
		point t=target.getlocation();
		point s=source.getlocation();
		if(!onestep(s,t)) return false;
		if(!isinl(s,t)&&!isinx(s,t))
		{
			
			return f;
		}else{
			if(!isv(s,t))//有障碍
			{
			
				return f;
			}else{
				return true;
				}
		}
	}
	public boolean pawn(piece source,piece target)
	{
		boolean f=false;
		point t=target.getlocation();
		point s=source.getlocation();
		if(!onestep(s,t)) return false;
		if(!isinl(s,t)&&!isinx(s,t))
		{
			
			return f;
		}else{
			if(!isv(s,t))//有障碍
			{
			
				return f;
			}else{
				return true;
				}
		}
	}
	/*public boolean pawn(piece source,piece target)
	{
		boolean f=false;
		point t=target.getlocation();
		point s=source.getlocation();
		if(!onestep(s,t)) return false;
		//if()
		if(!isinl(s,t))
		{
			return f;
		}else{
			if(!isv(s,t))//有障碍
			{
				
				return f;
			}else{
				return true;
				}
		}
		
	}*/
	public boolean onestep(point s,point t)
	{
		
		int sx=s.getX();
		int sy=s.getY();
		int tx=t.getX();
		int ty=t.getY();
		
		if((sx-1)<=tx && (sx+1)>=tx)
			if((sy-1)<=ty && (sy+1)>=ty)
				return true;
		return false;
	}
	public boolean isok(piece source,piece target)
	{
		boolean f=false;
		String sname=source.getName();
		String tname=target.getName();
		if(sname==tname) return f;
		if(sname.charAt(0)==tname.charAt(0)) return f;
		String role=sname;
		if(role=="hqueen"||role=="gqueen"){return queen(source,target);}
		
		if(role=="hrook"||role=="grook"){return rook(source,target);}//车
			
		if(role=="hknight"||role=="gknight"){return knight(source,target);}//马
		
		if(role=="hbishop"||role=="gbishop"){return bishop(source,target);}//象
			
		if(role=="hking"||role=="gking"){return king(source,target);}
				
		if(role=="hpawn"||role=="gpawn"){return pawn(source,target);}//兵
	
		return f;
		
	}
	private piece[][] pp;
	//private int move[][]={{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};
	

}
