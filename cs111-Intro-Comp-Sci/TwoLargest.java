public class TwoLargest {

    public static void main(String[] args) {

        System.out.println("Enter 0 to terminate. ");

        double secondLargest;
        double firstLargest;

        firstLargest = IO.readDouble();
        secondLargest = firstLargest;

        double current = IO.readDouble();

        while (current != 0) {

            if (current > firstLargest) {
                secondLargest = firstLargest;
                firstLargest = current;
            } else if (current > secondLargest) {
                secondLargest = current;
            }

            current = IO.readDouble();

        }

        System.out.println(firstLargest);
        System.out.println(secondLargest);

    }

}
