package light3D;
import java.awt.Font;  
import java.awt.geom.QuadCurve2D;  
import javax.media.j3d.Appearance;  
import javax.media.j3d.Font3D;  
import javax.media.j3d.FontExtrusion;  
import javax.media.j3d.Material;  
import javax.media.j3d.Shape3D;  
import javax.media.j3d.Text3D;  
import javax.vecmath.Color3f;  
import javax.vecmath.Point3f;  
  
public class StringEle extends Shape3D{  
	  
    private String str= "";  
    private Point3f points;  
      
    StringEle(String str,Point3f point)  
    {  
       this.str=str;  
       this.points=point;  
       QuadCurve2D.Double curve=new QuadCurve2D.Double();  
       curve.setCurve(0,0,0.0,0.0,0.01,0);  
       FontExtrusion fe1=new FontExtrusion();  
       fe1.setExtrusionShape(curve);  
       Font3D f3d=new Font3D(new Font("",Font.PLAIN,1),fe1);  
       Text3D txt=new Text3D(f3d,str,points);  
       Appearance app=new Appearance();  
       Material m=new Material();  
       m.setDiffuseColor(new Color3f(0.0f,1.0f,0.0f));  
       app.setMaterial(m);  
       this.setGeometry(txt);  
       this.setAppearance(app);    
    }  
}  
  
