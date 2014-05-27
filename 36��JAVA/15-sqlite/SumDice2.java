// The "SumDice2" class.
import java.awt.*;
import hsa.Console;
import usefulstuff.*;

    public class SumDice2
{
    static Console c;           // The output console

     public static void main (String[] args) throws Exception
    {
	c = new Console ();

	// Place your program here.  'c' is the output console


	int sum = 5;
	int guess;
	int color = 1;
	int answer = 0;
	Dice d1 = new Dice ();

	c.println ("sum the all Dice  ");


	for (int i = 1 ; i <= 5 ; i++)
	{
	    Thread.sleep (1000);
	    d1.roll ();
	    if (i % 2 == 1)
	    {
		d1.setColor (Color.gray);
	    }
	    else
	    {
		d1.setColor (Color.red);
	    }

	    d1.draw (c);
	    answer = answer + d1.getNumber ();
	}

	c.print ("What is the sum: ");
	guess = c.readInt ();
	do
	{
	    if (guess < answer)
	    {
		c.println ("Incorrect,Too Low ");
		c.print ("What is the sum: ");
		guess = c.readInt ();
	    }

	    else if (guess > answer)
	    {
		c.println ("Incorrect, Too High ");
		c.print ("What is the sum: ");
		guess = c.readInt ();
	    }
	}
	while (guess != answer);

	if (guess == answer)
	{
	    c.print ("Correct");
	}


	// Place your program here.  'c' is the output console
    } // main method
} // SumDice2 class
