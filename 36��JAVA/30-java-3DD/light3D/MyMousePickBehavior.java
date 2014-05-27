
package light3D;
import java.awt.Color;
import javax.media.j3d.Appearance;
import javax.media.j3d.Bounds;
import javax.media.j3d.BranchGroup;
import javax.media.j3d.Canvas3D;
import javax.media.j3d.Material;
import javax.swing.JTextArea;
import javax.vecmath.Color3f;
import com.sun.j3d.utils.geometry.Sphere;
import com.sun.j3d.utils.picking.behaviors.PickMouseBehavior;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.picking.PickTool;

public class MyMousePickBehavior extends PickMouseBehavior{
	 int data[][]=new int[8][8];
	 Appearance appear=null;
	 Material mater=null;
	 JTextArea jta=null;
	public MyMousePickBehavior(Canvas3D canvas, BranchGroup root, Bounds bounds, JTextArea jta) {
		super(canvas, root, bounds);
		this.jta=jta;
		this.setSchedulingBounds(bounds);
		root.addChild(this);
		pickCanvas.setMode(PickTool.GEOMETRY);
		  appear=new Appearance();
	      mater=new Material();
	      mater.setDiffuseColor(new Color3f(Color.red));
	      appear.setMaterial(mater);
		
	}

	public void updateScene(int xpos, int ypos) {
		PickResult pickResult = null;
		Sphere shape = null;
		pickCanvas.setShapeLocation(xpos, ypos);

		pickResult = pickCanvas.pickClosest();
		if (pickResult != null) {
		    shape = (Sphere) pickResult.getNode(PickResult.PRIMITIVE);
		    System.out.println(shape.getName());
		    dealData(shape);
		}
		
	    }
	
	public int getZintValue(float zValue){
		if(zValue==-3.5f){
			return Integer.parseInt("00000001", 2);
		}else if(zValue==-2.5f){
			return Integer.parseInt("00000010", 2);
		}else if(zValue==-1.5f){
			return Integer.parseInt("00000100", 2);
		}else if(zValue==-0.5f){
			return Integer.parseInt("00001000", 2);
		}else if(zValue==0.5f){
			return Integer.parseInt("00010000", 2);
		}else if(zValue==1.5f){
			return Integer.parseInt("00100000", 2);
		}else if(zValue==2.5f){
			return Integer.parseInt("01000000", 2);
		}else{
			return Integer.parseInt("10000000", 2);
		}
	}
	
	
    public int getXintValue(float xValue){
    	if(xValue==-3.5f){
			return 0;
		}else if(xValue==-2.5f){
			return 1;
		}else if(xValue==-1.5f){
			return 2;
		}else if(xValue==-0.5f){
			return 3;
		}else if(xValue==0.5f){
			return 4;
		}else if(xValue==1.5f){
			return 5;
		}else if(xValue==2.5f){
			return 6;
		}else{
			return 7;
		}
    }
    
    public int getYintValue(float yValue){
    	if(yValue==-3.5f){
			return 0;
		}else if(yValue==-2.5f){
			return 1;
		}else if(yValue==-1.5f){
			return 2;
		}else if(yValue==-0.5f){
			return 3;
		}else if(yValue==0.5f){
			return 4;
		}else if(yValue==1.5f){
			return 5;
		}else if(yValue==2.5f){
			return 6;
		}else{
			return 7;
		}
    }
	
    
    public void dealData(Sphere shape){
    	String userData=shape.getName();
    	String[] str=userData.split(",");
    	int state=Integer.parseInt(str[3]);
    	float x=Float.parseFloat(str[0]);
    	float y=Float.parseFloat(str[1]);
    	float z=Float.parseFloat(str[2]);
    	if(state==0){
    		data[getXintValue(x)][getYintValue(y)]=data[getXintValue(x)][getYintValue(y)] | getZintValue(z);
    		shape.setName( str[0]+","+str[1]+","+str[2]+","+1);
    		Appearance appear=new Appearance();
    		Material mater=new Material();
  	        mater.setDiffuseColor(new Color3f(Color.red));
  	        appear.setMaterial(mater);
  	        shape.setAppearance(appear);
  	        jta.setText(getDataString());
    	}else{
    		data[getXintValue(x)][getYintValue(y)]=data[getXintValue(x)][getYintValue(y)] & (~getZintValue(z));
    		shape.setName( str[0]+","+str[1]+","+str[2]+","+0);
    		Appearance appear=new Appearance();
    		Material mater=new Material();
  	        mater.setDiffuseColor(new Color3f(Color.black));
  	        appear.setMaterial(mater);
  	        shape.setAppearance(appear);
  	        jta.setText(getDataString());
    	}
    }
    
    
    public String getDataString(){
    	String temp="";
    	temp=temp+"int data_array[8][8]=\n";
    	temp=temp+"{\n";
    	for(int x=0;x<8;x++){
    		temp=temp+"{";
    		for(int y=0;y<8;y++){
    			if(data[x][y]==0){
    				temp=temp+"0x00";
    			}else{
    				temp=temp+"0x"+Integer.toHexString(data[x][y]);
    			}
    			if(y!=7) temp=temp+",";
    		}
    		temp=temp+"}";
    		if(x!=7) temp=temp+",";
    		temp=temp+"\n";
    	}
    	temp=temp+"}";
    	return temp;
    }
	
	}

