package DecisionTree;

public class Instance {
    
    double attribute;
    boolean label;

    public Instance(double a, boolean c) {
	attribute = a;
	label  = c;
    }

    public double getAttribute() {
	return attribute;
    }

    public void setAttribute(double a) {
	attribute = a;
    }

    public boolean getLabel() {
	return label;
    }

    public void setLabel(boolean c) {
	label = c;
    }
}