package DecisionTree;

public class DTNode {

    Instance[] a;
    double testValue;
    DTNode left, right;

    // You will need to insert some code here
	// internal node
	public DTNode(double cutoff, Instance[] instances, int s, int e)
	{
		testValue = cutoff;
		a = new Instance[2];

		a[0] = instances[s];
		a[1] = instances[e];

		left = right = null;
	}

	// leaf node
	public DTNode(Instance[] instances, int s, int e)
	{
		int size = e - s + 1;

		testValue = 0.0;
		a = new Instance[size];

		for (int i = s; i <= e; i++)
			a[i-s] = instances[i];

		left = right = null;
	}
}
