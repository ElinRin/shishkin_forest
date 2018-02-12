class MainClass {
    public static void main(String[] a){
	      System.out.println(new Order().set(0));
    }
}

class Order {
  int a;
  
  public int test(Order e) {
    int[] c;
    a = 1 + 2 * 3;
    c[0] = e.set(1 * 2 + 3);
    c[2] = 10;
    return 0;
  } 
  
  public int set(int _a) {
    a = _a;
    return 0;
  }
}
