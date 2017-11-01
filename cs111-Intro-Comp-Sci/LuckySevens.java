public class LuckySevens {

    public static void main(String[] args) {

        int low = IO.readInt();
        int high = IO.readInt();

        int i = low;
        String test;

        int count = 0;

        while (i <= high) {

            test = Integer.toString(i);

            for (int j = 0; j < test.length(); j++) {
                if (test.charAt(i) == '7') {
                    count++;
                }
            }

            i++;
        }
    }
}
