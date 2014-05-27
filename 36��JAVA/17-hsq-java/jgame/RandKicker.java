import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import java.util.Timer;
import java.util.TimerTask;
import javax.swing.SwingWorker;

public class RandKicker
{
	public static void main(String[] args)
	{
		KickerFrame frame = new KickerFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}

class KickerFrame extends JFrame
{
	public KickerFrame()
	{
		setSize(650, 400);
		setTitle("Letter Kicker(Made By HongShuiQuan)");
		KickerPanel panel = new KickerPanel();
		add(panel);
		//pack();
	}
}

class KickerPanel extends JPanel
{
	public KickerPanel()
	{
		setLayout(new BorderLayout());

		display = new JButton("[Two Or Three Equals, Try]");
		display.setEnabled(false);
		add(display, BorderLayout.NORTH);

		JPanel cp = new JPanel();
		cp.setLayout(new GridLayout(1, 3));

		left = new JLabel("0", JLabel.CENTER);
		left.setFont(new Font("Serif", Font.BOLD, 200));
		left.setBorder(BorderFactory.createEtchedBorder());
		cp.add(left);

		center = new JLabel("0", JLabel.CENTER);
		center.setFont(new Font("Serif", Font.BOLD, 200));
		center.setBorder(BorderFactory.createEtchedBorder());
		cp.add(center);

		right = new JLabel("0", JLabel.CENTER);
		right.setFont(new Font("Serif", Font.BOLD, 200));
		right.setBorder(BorderFactory.createEtchedBorder());
		cp.add(right);

		add(cp, BorderLayout.CENTER);

		start = new JButton("Start");
		end = new JButton("STOP");

		ActionListener cmd = new SwitchCommandAction();
		start.addActionListener(cmd);
		end.addActionListener(cmd);

		JPanel bp = new JPanel();
		bp.setLayout(new GridLayout(1, 2));
		bp.add(start);
		bp.add(end);

		add(bp, BorderLayout.SOUTH);

		time.schedule(new TimerTask() {
				@Override
				public void run() { update();}
				}, 0, 300);
		
	}

	private class SwitchCommandAction implements ActionListener
	{
		public void actionPerformed(ActionEvent event)
		{
			display.setText("[..]");
			String input = event.getActionCommand();

			if (input.equals("Start")) control = true;
			if (input.equals("STOP")) 
			{
				control = false;
				int mark = isSuccess();
				if (mark > 0) {
					display.setText("[Congratulation, You Got Coin : " + mark + "]" );
				}
				else
					display.setText("[Two Or Three Equals, Try Again...]");
			}
		}
	}

	private int isSuccess()
	{
		String A = left.getText();
		String B = center.getText();
		String C = right.getText();

		if (A.equals(B) || A.equals(C)
				|| B.equals(C)) {

			if (A.equals(B) && A.equals(C)) return 100;
			if (A.equals(B) || A.equals(C)) return 50 + 2*(int)A.charAt(0);
			if (B.equals(C)) return 50 + 2*(int)B.charAt(0);
		}

		return 0;
	}


	private int getRandomInt()
	{
		int r = (int) (Math.random() * 100);

		return r;
	}

	private void updateLabel(JLabel label)
	{
		String s = label.getText();
		char c = s.charAt(0);

		int d = ((int)c + getRandomInt())%10;

		c = (char)(d + '0');
		s = "" + c;
		label.setText(s);
	}

	private void update()
	{
		new SwingWorker<Object, Object>()
		{
			protected Object doInBackground() throws Exception
			{
				if (control) 
				{
					updateLabel(left);
					updateLabel(center);
					updateLabel(right);
				}
				
				return null;
			}
		}.execute();
	}

	private boolean control = false;

	private JButton display;
	private JLabel left, center, right;
	private JButton start,end;

	private static final Timer time = new Timer("RUNNER");
}
		
