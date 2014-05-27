/***************************LineTest.java*****************************/

package light3D;

import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GraphicsConfiguration;
import javax.media.j3d.Appearance;
import javax.media.j3d.Background;
import javax.media.j3d.BoundingSphere;
import javax.media.j3d.BranchGroup;
import javax.media.j3d.Canvas3D;
import javax.media.j3d.DirectionalLight;
import javax.media.j3d.LineArray;
import javax.media.j3d.LineAttributes;
import javax.media.j3d.Material;
import javax.media.j3d.Shape3D;
import javax.media.j3d.Transform3D;
import javax.media.j3d.TransformGroup;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.vecmath.Color3f;
import javax.vecmath.Point3d;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3d;
import javax.vecmath.Vector3f;

import com.sun.j3d.utils.applet.MainFrame;
import com.sun.j3d.utils.behaviors.mouse.MouseRotate;
import com.sun.j3d.utils.behaviors.mouse.MouseTranslate;
import com.sun.j3d.utils.behaviors.mouse.MouseZoom;
import com.sun.j3d.utils.geometry.Sphere;
import com.sun.j3d.utils.universe.SimpleUniverse;
import com.sun.j3d.utils.universe.Viewer;
import com.sun.j3d.utils.universe.ViewingPlatform;

public class LineTest extends Applet{
	
    private SimpleUniverse universe;
    GraphicsConfiguration config =SimpleUniverse.getPreferredConfiguration();
    Canvas3D can=new Canvas3D(config);
    JTextArea jta=null;
    public BranchGroup createBranchGroup()
    {
    	
    JFrame f=new JFrame("数组显示区");
    f.setUndecorated(true);
    f.setSize(300,200);
    jta=new JTextArea();
    jta.setForeground(Color.red);
    jta.setBackground(Color.green);
    f.add(jta);
    f.setLocation(java.awt.Toolkit.getDefaultToolkit().getScreenSize().width-320,java.awt.Toolkit.getDefaultToolkit().getScreenSize().height-240);
    f.setResizable(false);
    f.setAlwaysOnTop(true);
    f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    f.setVisible(true);
    
    BranchGroup branchgrouproot = new BranchGroup();
    BoundingSphere bouds=new BoundingSphere(new Point3d(0.0,0.0,0.0),100);
    Background bg=new Background(new Color3f(Color.white));
    bg.setApplicationBounds(bouds);
    branchgrouproot.addChild(bg);
    
    //光源的设置
    DirectionalLight directionLight=new DirectionalLight(new Color3f(1.0f,1.0f,1.0f),new Vector3f(0f,3.0f,-5.0f));
    directionLight.setInfluencingBounds(bouds);
    branchgrouproot.addChild(directionLight);
    
    //坐标变换的设置
    TransformGroup trans=new TransformGroup();
    trans.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
    trans.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
    branchgrouproot.addChild(trans);
    
    //鼠标的设置旋转
    MouseRotate behavie1=new MouseRotate();
    behavie1.setTransformGroup(trans);
    branchgrouproot.addChild(behavie1);
    behavie1.setSchedulingBounds(bouds);
    
    //鼠标的平移
    MouseTranslate behavie2=new MouseTranslate();
    behavie2.setTransformGroup(trans);
    branchgrouproot.addChild(behavie2);
    behavie2.setSchedulingBounds(bouds);
    
    //鼠标的放大
    MouseZoom behavie3=new MouseZoom();
    behavie3.setTransformGroup(trans);
    behavie3.setSchedulingBounds(bouds);
    branchgrouproot.addChild(behavie3);
    
    MyMousePickBehavior behavie4=new MyMousePickBehavior(can,branchgrouproot,bouds,jta);
    
    
    TransformGroup lineGroup_pre = new TransformGroup();
    lineGroup_pre.setTransform(new Transform3D());
    LineShape ls=new LineShape();
    lineGroup_pre.addChild(ls);
    trans.addChild(lineGroup_pre);
    
    TransformGroup XwordGroup=new TransformGroup();
    XwordGroup.setTransform(new Transform3D());
    StringEle Xele=new StringEle("X",new Point3f(5.5f,-3.5f,-3.5f));
    XwordGroup.addChild(Xele);
    trans.addChild(XwordGroup);
    
    TransformGroup YwordGroup=new TransformGroup();
    YwordGroup.setTransform(new Transform3D());
    StringEle Yele=new StringEle("Y",new Point3f(-3.5f,5.5f,-3.5f));
    YwordGroup.addChild(Yele);
    trans.addChild(YwordGroup);
    
    TransformGroup ZwordGroup=new TransformGroup();
    ZwordGroup.setTransform(new Transform3D());
    StringEle Zele=new StringEle("Z",new Point3f(-3.5f,-3.5f,5.5f));
    ZwordGroup.addChild(Zele);
    trans.addChild(ZwordGroup);
    
    
 
       
       for(int point_num=0;point_num<1536;point_num=point_num+3){
	       TransformGroup sphereGroup =new TransformGroup();
	       sphereGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
	       sphereGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_READ);
	       sphereGroup.setCapability(TransformGroup.ENABLE_PICK_REPORTING);
	       Transform3D sphereTransform3D=new  Transform3D();
	       sphereTransform3D.setTranslation(new  Vector3d(LineShape.points[point_num],LineShape.points[point_num+1],LineShape.points[point_num+2]));
	       sphereGroup.setTransform(sphereTransform3D);
	       Sphere sphere2=new  Sphere(0.1f, -1, 80);
	       String userData=""+LineShape.points[point_num]+","+LineShape.points[point_num+1]+","+LineShape.points[point_num+2]+","+0;
	       sphere2.setName(userData);
	       sphereGroup.setName(userData);
	       Appearance appear=new Appearance();
	       Material mater=new Material();
	       mater.setDiffuseColor(new Color3f(Color.black));
	       appear.setMaterial(mater);
	       sphere2.setAppearance(appear);
	       sphereGroup.addChild(sphere2);
	       trans.addChild(sphereGroup);
       }
       
       
       float[] linevertX={-4.5f,-3.5f,-3.5f,    6.0f,-3.5f,-3.5f};
       float[] linevertY={-3.5f,-4.5f,-3.5f,    -3.5f,6.0f,-3.5f};
       float[] linevertZ={-3.5f,-3.5f,-4.5f,    -3.5f,-3.5f,6.0f};
       float[] linecolor={0.0f,0.0f,7.0f,   0.0f,7.0f,0.0f};
       
       LineArray lineX=new LineArray(2,LineArray.COORDINATES | LineArray.COLOR_3);
       lineX.setCoordinates(0, linevertX);
       lineX.setColors(0, linecolor);
       
       LineArray lineY=new LineArray(2,LineArray.COORDINATES | LineArray.COLOR_3);
       lineY.setCoordinates(0, linevertY);
       lineY.setColors(0, linecolor);
       
       LineArray lineZ=new LineArray(2,LineArray.COORDINATES | LineArray.COLOR_3);
       lineZ.setCoordinates(0, linevertZ);
       lineZ.setColors(0, linecolor);
       
       LineAttributes lineAttr=new LineAttributes();
       lineAttr.setLineWidth(3.0f);
       lineAttr.setLineAntialiasingEnable(true);
       
       Appearance lineAppr=new Appearance();
       lineAppr.setLineAttributes(lineAttr);
       
       TransformGroup lineGroup = new TransformGroup();
       lineGroup.setTransform(new Transform3D());
       Shape3D line3DX=new Shape3D();
       line3DX.setGeometry(lineX);
       line3DX.setAppearance(lineAppr);
       lineGroup.addChild(line3DX);
       
       Shape3D line3DY=new Shape3D();
       line3DY.setGeometry(lineY);
       line3DY.setAppearance(lineAppr);
       lineGroup.addChild(line3DY);
       
       Shape3D line3DZ=new Shape3D();
       line3DZ.setGeometry(lineZ);
       line3DZ.setAppearance(lineAppr);
       lineGroup.addChild(line3DZ);
       
       trans.addChild(lineGroup);
       
       
       
       
	   branchgrouproot.compile();
	   return branchgrouproot; 
    }
    
    public void init()
    {
	    this.setLayout(new BorderLayout());
	    this.add("Center",can);
	    Viewer viewer=new Viewer(can);
	    Vector3f vpoint=new Vector3f(0.0f,0.0f,25.0f);
	    Transform3D t=new Transform3D();
	    t.set(vpoint);
	    ViewingPlatform vplat=new ViewingPlatform();
	    vplat.getViewPlatformTransform().setTransform(t);
	    universe= new SimpleUniverse(vplat,viewer);
	    universe.getViewingPlatform();
	    universe.addBranchGraph(createBranchGroup());
    }
    
    
    public static void main(String[] args) {
       new MainFrame(new LineTest(),500,500 );    
    }
}
