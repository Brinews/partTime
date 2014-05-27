import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.ArrayList;

class MyControlPanel extends JPanel
{
	/* For View Panel Select */
	public int panelID;
	
	/* Left control panel */
	public JPanel topPanel;
	
	/* Main view panel */
	public JPanel viewPanel;

	/* For Message Display */
	public JButton display;

	/* view panel class */
	List<JPanel> panelList;
	List<JButton> btnList;

	FilmListPanel mlp;
	FilmAddPanel map;

	ShowListPanel clp;
	ShowAddPanel cap;

	TicketListPanel vlp;

	UserLogonPanel ulp;

	public MyControlPanel()
	{
		panelList = new ArrayList<JPanel>();
		btnList = new ArrayList<JButton>();

		panelID = 0;
		setLayout(new BorderLayout());

		display = new JButton("[...]");
		display.setEnabled(false);
		add(display, BorderLayout.SOUTH);

		ActionListener setstate = new StateAction();

		topPanel = new JPanel();
		viewPanel = new JPanel();

		/***
		 * Left control buttons
		 */
		addButton("Logon Here", setstate);

		addButton("Film List", setstate);
		addButton("Film Add", setstate);

		addButton("MovieShow List", setstate);
		addButton("MovieShow Add", setstate);

		addButton("Ticket Buy", setstate);

		topPanel.setBorder(new TitledBorder(new EtchedBorder(), ""));
		add(topPanel, BorderLayout.NORTH);

		/***
		 * View Panels Init
		 */

		mlp = new FilmListPanel();
		map = new FilmAddPanel("");

		panelList.add(mlp);
		panelList.add(map);
		
		viewPanel.add(mlp);
		viewPanel.add(map);

		clp = new ShowListPanel();
		cap = new ShowAddPanel("");

		panelList.add(clp);
		panelList.add(cap);
		
		viewPanel.add(clp);
		viewPanel.add(cap);

		vlp = new TicketListPanel();

		panelList.add(vlp);
		viewPanel.add(vlp);

		ulp = new UserLogonPanel();
		viewPanel.add(ulp);
		panelList.add(ulp);

		viewPanel.setBorder(new TitledBorder(new EtchedBorder(), ""));
		hideViewPanel(6);

		add(viewPanel, BorderLayout.CENTER);

		ulp.setBtnList(btnList);
		hideClickButton(2, 6);
	}

	public void hideViewPanel(int f)
	{
		int i = 0;
		for(JPanel p:panelList)
		{
			i++;
			p.setVisible(false);
			if (i == f) {
				p.setVisible(true);
			}
		}
	}

	public void hideClickButton(int s, int e)
	{
		int i = 1;
		for(JButton p:btnList)
		{
			p.setVisible(true);
			if (i >= s && i <= e)
				p.setVisible(false);
			i++;
		}
	}

	public void addButton(String label, ActionListener listener)
	{
		JButton button = new JButton(label);
		button.addActionListener(listener);
		
		btnList.add(button);
		topPanel.add(button);
	}

	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
	}

	private class StateAction implements ActionListener {
		public void actionPerformed(ActionEvent event)
		{
			String input = event.getActionCommand();

			if (input.equals("Film List"))
				panelID = 1;
			if (input.equals("Film Add"))
				panelID = 2;
			if (input.equals("MovieShow List"))
				panelID = 3;
			if (input.equals("MovieShow Add"))
				panelID = 4;
			if (input.equals("Ticket Buy"))
				panelID = 5;
			if (input.equals("Logon Here"))
				panelID = 6;

			/***
			 *repaint
			 */
			hideViewPanel(panelID);
		}
	}
}

