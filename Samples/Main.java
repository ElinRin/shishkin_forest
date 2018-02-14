class MainClass {
    public static void main(String[] a){
	      System.out.println(new Order().set(0));
    }
}

class Order {
  int a;
  
  public int set(int _a) {
    a = _a;
    return 0;
  }
}
