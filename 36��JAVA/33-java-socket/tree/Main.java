package DecisionTree;

public class Main
{
	public static void main(String[] args)
	{
		Instance[] ins = new Instance[7];
		ins[0] = new Instance(1, true); 
		ins[1] = new Instance(3, true); 
		ins[2] = new Instance(4, false); 
		ins[3] = new Instance(5, false); 
		ins[4] = new Instance(6, true); 
		ins[5] = new Instance(7, false); 
		ins[6] = new Instance(8, false); 

		DecisionTree dt = new DecisionTree(ins);

		dt.print();

		System.out.println(dt.classify(2.0));

		dt.prune(1);
		dt.print();

	}
}
