import java.util.TimeZone;
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

class Print {
    public static void print(String str) {
        System.out.print(getCurrentTimeAsIso() + ": " + str);
    }

    public static void println(String str) {
        System.out.println(getCurrentTimeAsIso() + ": " + str);
    }

    private static String getCurrentTimeAsIso() {
        TimeZone tz = TimeZone.getTimeZone("UTC");
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'"); // Quoted "Z" to indicate UTC, no timezone offset
        df.setTimeZone(tz);
        String nowAsISO = df.format(new Date());

        return nowAsISO;
    }
}
