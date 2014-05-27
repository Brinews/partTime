JTable table = new JTable(new DefaultTableModel(rowData, columnNames))
在参数里填入列数据和列名称
把table放到一个JFrame里显示
使用线程或者Timer定时更新数据
使用((DefaultTableModel) table.getModel()).fireTableDataChanged();让页面刷新

import java.awt.BorderLayout;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingWorker;
import javax.swing.table.DefaultTableModel;

public class test extends JFrame {
 private static final long serialVersionUID = -4892684184268025880L;
 private static final Timer time = new Timer("test");
 private Vector<Vector<Object>> rowData = new Vector<Vector<Object>>();
 private JTable table;
 private int count = 0;

 public test() {
  super("表格");
  this.setSize(400, 400);
  this.setLayout(new BorderLayout());
  this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

  Vector<String> columnNames = new Vector<String>();
  columnNames.add("统计项");
  columnNames.add("数目");
  table = new JTable(new DefaultTableModel(rowData, columnNames));
  this.add(new JScrollPane(table), BorderLayout.CENTER);
  time.schedule(new TimerTask() {
   @Override
   public void run() {
    update();
   }
  }, 0, 1000);
 }

 private void update() {
  new SwingWorker<Object, Object>() {
   protected Object doInBackground() throws Exception {
    rowData.clear();
    for (int i = 0; i < 4; i++) {
     Vector<Object> info = new Vector<Object>();
     info.add("测试");
     info.add(count);
     rowData.add(info);
    }
    count++;
    return null;
   }

   protected void done() {
    ((DefaultTableModel) table.getModel()).fireTableDataChanged();
   }
  }.execute();
 }

 public static void main(String args[]) {
  new test().setVisible(true);
 }
}

