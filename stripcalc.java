

public final class const {
    public static final  double PI = 3.1415926535;
    public static final  double EPS0 = 8.854E-12;
    public static final  double MU0 = 1.23456;
    public static final  double C = 3.0E8;
    public static final  double N0 = 120*PI;
}


public class Parameter {
    public String param;
    public String helptext;
    public double value;
    public double default;
    public boolean variable;

    public double getval() { return(value); }
    public double setval(double x) { value=x; return(value); }
    public int rangecheck();
}

public class TransmissionLine {
    String paramNames[];
    double Zo, Eeff;
}


public class Microstrip extends TransmissionLine {
    //Constructor    
    public Microstrip() {
	public Parameter w("w","Width of strip",635.0,true);
	public Parameter eps("Er","Dielectric constant of substrate",10.0,true);
	public Parameter h("h","Substrate height",635.0,true);
    
    }
    //Calculate Zo
    public double calcZo() {
    }
    //Calculate Eeff
    public double calcEeff() {
    }

    //Range check parameters
    public int rangecheck() {
    }
}
