class MainClass {
    public static void main(String[] a){
	      System.out.println(new Plus().plus(1, 2));
    }
}

class Plus {
  public int plus(int a, int b) {
    int result;
    result = a + b;
    return result;
  }
}
