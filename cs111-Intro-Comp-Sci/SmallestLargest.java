public class SmallestLargest {

    public static void main(String[] args) {

        System.out.println("Enter 0 to terminate. ")

        double min = Double.MAX_VALUE;
        double max = Double.MIN_VALUE;

        double current;

        current = IO.readDouble();

        while (current != 0) {

            if (current < min) {
                min = current;
            }

            if (current > max) {
                max = current;
            }

            current = IO.readDouble();

        }

        System.out.println(min);
        System.out.println(max);

    }

}
