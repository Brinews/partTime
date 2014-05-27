package light3D;
import javax.media.j3d.Appearance;
import javax.media.j3d.LineArray;
import javax.media.j3d.LineAttributes;
import javax.media.j3d.Shape3D;

public class LineShape extends Shape3D{
	private float vert[]=new float[192*2*3];
    
    private float color[]=new float[192*2*3];
    
    public static float points[]=new float[1536];
    
    public LineShape(){
    	setCoordData();
    	setColorData();
    	setPointData();
    	LineArray line=new LineArray(1152,LineArray.COORDINATES|LineArray.COLOR_3);
    	line.setCoordinates(0, vert);
    	line.setColors(0, color);
    	
    	LineAttributes linea = new LineAttributes();
    	linea.setLineWidth(0.01f);
    	linea.setLineAntialiasingEnable(true);
    	
    	Appearance app = new Appearance();
    	app.setLineAttributes(linea);
    	this.setGeometry(line);
    	this.setAppearance(app);
    	this.setPickable(false);
    }
    
    
    public void setCoordData(){
    	int array_num=0;
    	
        //设置X方向上线段对应的数据
    	for(float z=3.5f;z>=-3.5f;z=z-1){
    		for(float y=3.5f;y>=-3.5f;y=y-1){
    			for(int x=-1;x<2;x=x+2,array_num=array_num+3){
    				vert[array_num]=3.5f*x;
    				vert[array_num+1]=1.0f*y;
    				vert[array_num+2]=1.0f*z;
    			}
    		}
    	}
    	
    	//设置y方向上线段对应的数据
    	for(float z=3.5f;z>=-3.5f;z=z-1){
    		for(float x=3.5f;x>=-3.5f;x=x-1){
    			for(int y=-1;y<2;y=y+2,array_num=array_num+3){
    				vert[array_num]=1.0f*x;
    				vert[array_num+1]=3.5f*y;
    				vert[array_num+2]=1.0f*z;
    			}
    		}
    	}
    	
    	//设置z方向上线段对应的数据
    	for(float y=3.5f;y>=-3.5f;y=y-1){
    		for(float x=3.5f;x>=-3.5f;x=x-1){
    			for(int z=-1;z<2;z=z+2,array_num=array_num+3){
    				vert[array_num]=1.0f*x;
    				vert[array_num+1]=1.0f*y;
    				vert[array_num+2]=3.5f*z;
    			}
    		}
    	}
    		
    }
    
    //设置线段颜色数据
    public void setColorData(){
    	for(int i=0;i<1152;i++){
    		color[i]=0.5f;
    	}
    }
    
    //设置节点数据
    public void setPointData(){
    	int point_num=0;
    	for(float x=3.5f;x>=-3.5f;x=x-1){
    		for(float y=3.5f;y>=-3.5f;y=y-1){
    			for(float z=3.5f;z>=-3.5f;z=z-1,point_num=point_num+3){
    			   points[point_num]=1.0f*x;
    			   points[point_num+1]=1.0f*y;
    			   points[point_num+2]=1.0f*z;
    			}
    		}
    	}
    }
}
