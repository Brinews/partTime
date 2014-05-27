package DecisionTree;

import java.lang.Math.*;

public class DecisionTree {

	DTNode root;
	int Total;

	private double binaryLog(double a)
	{
		if (a == 0.0) return 0.0;
		//System.out.println("a="+ a + "log="+ Math.log(a));

		return Math.log(a)/Math.log(2.0);
	}

	private double GetEntropy(Instance[] instances, int s, int e)
	{
		int cnt = e - s + 1;
		int trueNum = 0, falseNum = 0;

		for (int i = s; i <= e; i++)
		{
			if (instances[i].getLabel()) trueNum++;
			else
				falseNum++;
		}

		//System.out.println("cnt="+ cnt + "trueNum="+ trueNum +
	//			"falseNum="+falseNum + "binlog(t)"+ binaryLog(1.0*trueNum/cnt));

		return -(1.0*trueNum/cnt)*binaryLog(1.0*trueNum/cnt)
				- (1.0*falseNum/cnt)*binaryLog(1.0*falseNum/cnt);
	}

	//get the cut postion between[s, e]
	private int cutByEntropy(Instance[] instances, int s, int e)
	{
		int pos = s;
		double minEntropy = 2.0;
		int cnt = e - s + 1;

		// e - s > 0
		if (cnt == 1) return s;

		for (int i = s; i < e; i++)
		{
			double entropy = 3.0;

			double leftE, rightE;

			leftE = GetEntropy(instances, s, i);
			rightE = GetEntropy(instances, i+1, e);

			//System.out.println(leftE + "---" + rightE);

			entropy = (1.0*(i - s + 1)/cnt) * leftE;
			entropy += (1.0*(e - i)/cnt) * rightE;

			//System.out.println(entropy + "==");
			if (entropy < minEntropy)
			{
				pos = i;
				minEntropy = entropy;
			}
		}

		return pos;
	}

	// all be 'true' or 'false'
	private boolean isSameClass(Instance[] instances, int s, int e)
	{
		boolean start = instances[s].getLabel();
		for (int i = s+1; i <= e; i++)
		{
			if (instances[i].getLabel() != start)
				return false;
		}

		return true;
	}

	private DTNode buildDeciTree(Instance[] instances, int s, int e)
	{
		int cnt = e - s + 1;

		// leaf node ( cannot be cut )
		if (cnt == 1 || isSameClass(instances, s, e))
		{
			return new DTNode(instances, s, e);
		}

		//System.out.println("s=" + s + "e=" + e);
		int pos = cutByEntropy(instances, s, e);
		//System.out.println("pos=" + pos);

		double cutoffValue = (instances[pos].getAttribute() +
			instances[pos+1].getAttribute()) / 2;

		DTNode r = new DTNode(cutoffValue, instances, s, e);
		r.left = buildDeciTree(instances, s, pos);
		r.right = buildDeciTree(instances, pos+1, e);

		return r;
	}
    
    public DecisionTree(Instance[] instances) {
	
	// You need to insert code here
		Total = instances.length;
		root = buildDeciTree(instances, 0, instances.length - 1);
    }

	void pruneDeciTree(DTNode r, int sl)
	{
		if (null == r) return;

		DTNode left = r.left, right = r.right;

		if ((null != left && left.testValue == 0.0 && left.a.length <= sl)
		|| (null != right && right.testValue == 0.0 && right.a.length <= sl))
		{
			r.left = null;
			r.right = null;
			r.testValue = 0.0;
		}

		pruneDeciTree(r.left, sl);
		pruneDeciTree(r.right, sl);
	}

    public void prune(int sl) {
	// Insert your code here
		pruneDeciTree(root, sl);
    }

	boolean classifyDeciTree(DTNode r, double input)
	{
		if (null == r) return false;

		//reach leaf node
		if (r.testValue == 0.0)
		{
			return r.a[0].getLabel();
		}

		if (r.testValue < input)
			return classifyDeciTree(r.right, input);
		else
			return classifyDeciTree(r.left, input);
	}

    public boolean classify(double input) {
	// Insert your code here
		return classifyDeciTree(root, input);
    }

	void printDeciTree(DTNode r, int level)
	{
		if (null == r) return;

		for (int i = 0; i < level; i++)
			System.out.print("  ");

		if (r.testValue != 0.0) {
			System.out.println("InternalNode, cutoff value = " + r.testValue);
		}
		else {
			System.out.println("LeafNode,class = " +
					r.a[0].getLabel() + ", fraction = " +
					1.0*r.a.length/Total);
		}

		printDeciTree(r.left, level+1);
		printDeciTree(r.right, level+1);
	}

    public void print() {
	// Insert your code here
		printDeciTree(root, 0);
    }
}
